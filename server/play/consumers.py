import json
from channels.generic.websocket import AsyncWebsocketConsumer
from random import choice
from asyncio import create_subprocess_exec, subprocess
from pathlib import Path

paths_file = Path(__file__).resolve().parent.parent / "binary_paths"
paths = eval(open(paths_file, "r").read())

queue = []


def get_coords(move_string):
    res = [0] * 4
    res[0] = ord(move_string[0]) - ord('a')
    res[2] = ord(move_string[3]) - ord('a')
    res[1] = int(move_string[1]) - 1
    res[3] = int(move_string[4]) - 1
    return list(map(str, res))


async def communicate(process, data, wait_for_result=True):
    process.stdin.write((data + "\n").encode('ascii'))
    await process.stdin.drain()
    if wait_for_result:
        res = await process.stdout.readline()
        res = res.decode('ascii').strip()
        return res


class MultiplayerConsumer(AsyncWebsocketConsumer):
    async def connect(self):
        await self.accept()
        self.pair = None
        if queue:
            self.pair = queue[-1]
            queue[-1].pair = self
            queue[-1].color = "white"
            self.color = "black"
            queue.pop()
            await self.pair.send(text_data=json.dumps({"type": "opponent found", "color": "white"}))
            await self.send(text_data=json.dumps({"type": "opponent found", "color": "black"}))
            self.validator = await create_subprocess_exec(paths["multiplayer"],
                                                          stdin=subprocess.PIPE,
                                                          stdout=subprocess.PIPE)
            self.pair.validator = self.validator
        else:
            queue.append(self)

    async def disconnect(self, close_code):
        if queue and queue[-1] == self:
            queue.pop()
        await self.pair.send(text_data=json.dumps({"type": "disconnect"}))
        if self.pair:
            self.pair.validator = None
            self.validator.stdin.close()
            self.validator.terminate()

    async def receive(self, text_data):
        text_data_json = json.loads(text_data)
        move = text_data_json["move"]
        valid = int(await communicate(self.validator, "check " + " ".join(get_coords(move))))
        if valid:
            promotion = int(await communicate(self.validator, "is_promotion " + " ".join(get_coords(move))))
            if move[-1] not in ['Q', 'N', 'R', 'B'] and promotion:  # check if need promotion
                await self.send(text_data=json.dumps({"type": "promotion_query"}))
                return
            castling = int(await communicate(self.validator, "is_castle " + " ".join(get_coords(move))))
            taken = await communicate(self.validator, "sth_taken " + " ".join(get_coords(move)))
            taken_coords = []
            if taken != "none":
                taken = taken.split(' ')
                taken_coords = [chr(ord('a') + int(taken[0])) + str(1 + int(taken[1]))]
            message = json.dumps({"type": "moves", "moves": [move], "color": self.color, "taken": taken_coords})
            if castling:
                other = await communicate(self.validator, "castle " + " ".join(get_coords(move)))
                coords = list(map(int, other.split()))
                other = chr(ord('a') + coords[0]) + str(1 + coords[1]) + '-' + \
                    chr(ord('a') + coords[2]) + str(1 + coords[3])
                message = json.dumps({"type": "moves", "moves": [move, other],
                                      "color": self.color, "taken": taken_coords})
            await self.send(text_data=message)
            await self.pair.send(text_data=message)
            full = {'R': 'Rook', 'N': 'Knight', 'Q': 'Queen', 'B': 'Bishop'}
            res_piece = "none"
            if len(move) == 6:
                res_piece = full[move[-1]]
            await communicate(self.validator, "make_move " + " ".join(get_coords(move)) + " " + res_piece, False)
            result = await communicate(self.validator, "result " + " ".join(get_coords(move)))
            if result != "none":
                result_string = "draw"
                if result == '1':
                    result_string = "white won!"
                elif result == '-1':
                    result_string = "black won!"
                result_json = json.dumps({"type": "result", "result": result_string})
                await self.send(text_data=result_json)
                await self.pair.send(text_data=result_json)


class BotConsumer(AsyncWebsocketConsumer):
    async def make_move(self):
        moves = await communicate(self.bot, "generate")

        def create_move(data):
            res = chr(ord('a') + ord(data[0]) - ord('0')) + chr(ord(data[1]) + 1) + '-'\
                  + chr(ord('a') + ord(data[2]) - ord('0')) + chr(ord(data[3]) + 1)\
                  + ("" if data[-1] == 'none' else data[-1][0])
            if res[-1] == 'K':
                res = res[:-1] + 'N'
            return res

        def parse(moves):
            res = []
            coords = moves.split(' ')
            for i in range(len(coords) // 5):
                res.append(create_move(coords[5 * i: 5 * i + 5]))
            return res

        to_move = moves.split()[:5]
        moves = parse(moves)
        revcolor = "black" if self.color == "white" else "white"
        taken = await communicate(self.bot, "sth_taken " + " ".join(get_coords(moves[0])))
        taken_coords = []
        if taken != "none":
            taken = taken.split(' ')
            taken_coords = [chr(ord('a') + int(taken[0])) + str(1 + int(taken[1]))]
        await self.send(text_data=json.dumps({"type": "moves", "moves": moves,
                                              "color": revcolor, "taken": taken_coords}))
        result = await communicate(self.bot, "result " + " ".join(get_coords(moves[0])))
        if result != "none":
            result_string = "draw"
            if result == '1':
                result_string = "white won!"
            elif result == '-1':
                result_string = "black won!"
            result_json = json.dumps({"type": "result", "result": result_string})
            await self.send(text_data=result_json)
        await communicate(self.bot, "make_move " + " ".join(to_move), False)

    async def connect(self):
        await self.accept()
        self.color = choice(["black", "white"])
        await self.send(text_data=json.dumps({"type": "opponent found", "color": self.color}))
        self.bot = await create_subprocess_exec(paths["bot"], stdin=subprocess.PIPE, stdout=subprocess.PIPE)
        if self.color == "black":
            await self.make_move()

    async def disconnect(self, close_code):
        self.bot.stdin.close()
        self.bot.terminate()

    async def receive(self, text_data):
        text_data_json = json.loads(text_data)
        move = text_data_json["move"]
        valid = int(await communicate(self.bot, "check " + " ".join(get_coords(move))))
        if valid:
            promotion = int(await communicate(self.bot, "is_promotion " + " ".join(get_coords(move))))
            if move[-1] not in ['Q', 'N', 'R', 'B'] and promotion:  # check if need promotion
                await self.send(text_data=json.dumps({"type": "promotion_query"}))
                return
            castling = int(await communicate(self.bot, "is_castle " + " ".join(get_coords(move))))
            message = json.dumps({"type": "moves", "moves": [move], "color": self.color})
            taken = await communicate(self.bot, "sth_taken " + " ".join(get_coords(move)))
            taken_coords = []
            if taken != "none":
                taken = taken.split(' ')
                taken_coords = [chr(ord('a') + int(taken[0])) + str(1 + int(taken[1]))]
            message = json.dumps({"type": "moves", "moves": [move], "color": self.color, "taken": taken_coords})
            if castling:
                other = await communicate(self.bot, "castle " + " ".join(get_coords(move)))
                coords = list(map(int, other.split()))
                other = chr(ord('a') + coords[0]) + str(1 + coords[1]) + \
                    '-' + chr(ord('a') + coords[2]) + str(1 + coords[3])
                message = json.dumps({"type": "moves", "moves": [move, other],
                                      "color": self.color, "taken": taken_coords})
            await self.send(text_data=message)
            full = {'R': 'Rook', 'N': 'Knight', 'Q': 'Queen', 'B': 'Bishop'}
            res_piece = "none"
            if len(move) == 6:
                res_piece = full[move[-1]]
            await communicate(self.bot, "make_move " + " ".join(get_coords(move)) + " " + res_piece, False)
            result = await communicate(self.bot, "result " + " ".join(get_coords(move)))
            if result != "none":
                result_string = "draw"
                if result == '1':
                    result_string = "white won!"
                elif result == '-1':
                    result_string = "black won!"
                result_json = json.dumps({"type": "result", "result": result_string})
                await self.send(text_data=result_json)
                return
            await self.make_move()
