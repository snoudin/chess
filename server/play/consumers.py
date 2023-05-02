import json

from channels.generic.websocket import AsyncWebsocketConsumer

queue = list()

class MoveConsumer(AsyncWebsocketConsumer):
    async def connect(self):
        await self.accept()
        if queue:
            self.pair = queue[-1]
            queue[-1].pair = self
            queue[-1].color = "white"
            self.color = "black"
            queue.pop()
            await self.pair.send(text_data=json.dumps({"type": "opponent found", "color": "white"}))
            await self.send(text_data=json.dumps({"type": "opponent found", "color": "black"}))
        else:
            queue.append(self)

    async def disconnect(self, close_code):
        if queue and queue[-1] == self:
            queue.pop()
        await self.pair.send(text_data=json.dumps({"type": "disconnect"}))

    async def receive(self, text_data):
        text_data_json = json.loads(text_data)
        move = text_data_json["move"]
        print(move)
        valid = True # TODO: use library
        if valid:
            promotion = False # TODO: use library
            if move[-1] not in ['Q', 'N', 'R', 'B'] and promotion: # check if need promotion
                await self.send(text_data=json.dumps({"type": "promotion_query"}))
                return
            await self.send(text_data=json.dumps({"type": "move", "move": move, "color": self.color}))
            await self.pair.send(text_data=json.dumps({"type": "move", "move": move, "color": self.color}))
        
