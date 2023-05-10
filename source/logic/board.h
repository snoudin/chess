//
// Created by vitali on 07.05.23.
//
#pragma once
#include "position.h"
#include "move.h"

#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

class Board {
    StartingPosition start;
    Position* cur_pos = &start;

    bool valid(std::shared_ptr<Move> mv);

    std::vector<std::shared_ptr<Move>> get_castle_moves();

public:
    Color get_color() { return start.get_color(); }

    std::vector<std::shared_ptr<Move>> get_all_moves();

    void make_move(std::shared_ptr<Move> mv) { cur_pos = cur_pos->makeMove(mv); }

    bool is_mate();

    bool dead_position();

    bool fifty_moves();

    size_t get_hash(size_t);

    void rollback_move();

    ~Board();

    friend void debug_out_board(Board&);
};

#endif //CHESS_BOARD_H
