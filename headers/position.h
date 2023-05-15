//
// Created by vitali on 02.04.23.
//
#pragma once

#include <vector>
#include <memory>
#include "piece.h"

class StartingPosition;

class FollowingPosition;

class Position;

class Board;

class Position {
  protected:
    StartingPosition* start;

  public:
    FollowingPosition* makeMove(std::shared_ptr<Move> mv);

    virtual std::shared_ptr<Move> get_last_move() = 0;

    std::shared_ptr<Piece> get_piece(const geometry::Point& p);

    explicit Position(StartingPosition* start) : start(start) {}

    friend class Board;

    friend class FollowingPosition;
};

class StartingPosition : public Position {
    std::vector<std::shared_ptr<Piece>> all_pieces;
    std::shared_ptr<Piece> used[board_size][board_size];
    Color which_turn = white;

  public:
    StartingPosition();

    std::shared_ptr<Move> get_last_move() override;

    friend class Position;

    friend class FollowingPosition;

    friend class Board;

    friend void debug_out_board(Board&);

  public:
    Color get_color() const;
};

class FollowingPosition : public Position {
    Position* prev;
    std::shared_ptr<Move> mv;
  public:
    FollowingPosition(std::shared_ptr<Move> mv, Position* prev, StartingPosition* start)
            : Position(start), mv(mv), prev(prev) {}

    std::shared_ptr<Move> get_last_move() override;

    Position* rollbackMove();

    friend class Position;

    friend class Board;
};
