//
// Created by vitali on 12.03.23.
//
#pragma once
#include <vector>
#include <string>
#include <memory>
#include <utility>
#include "gvector.h"
#include "move.h"

using namespace geometry;

enum Color : bool {
    white = true,
    black = false
};

enum PieceType: char {
    pawn = 'P',
    knight = 'N',
    king = 'K',
    queen = 'Q',
    bishop = 'B',
    rook = 'R'
};

class Position;

class Piece : public std::enable_shared_from_this<Piece> {
protected:
    using get_piece_func_type = std::shared_ptr<Piece> (Position::*)(const Point&);
    using last_move_func_type = std::shared_ptr<Move> (Position::*)();

// function get_piece(const Point&) is used to get_piece located on certain Point
// function last_move() is used to get previous move in game

    const PieceType name;
    geometry::Point coordinates;
    Color color = white;
    bool is_captured = false;
    int cnt_of_moves = 0;

  public:
    virtual std::vector<std::shared_ptr<Move>> getMoves(Position&, get_piece_func_type, last_move_func_type) = 0;

    inline const Point& getCoordinates() const { return coordinates; }

    inline Color getColor() const { return color; }

    inline bool getIsCaptured() const { return is_captured; }

    inline void switchCaptured() { is_captured ^= 1; }

    virtual bool canBeCaptured(Position&, get_piece_func_type, const Point&) = 0;

    inline PieceType getName() const { return name; }

    inline int getCnt() const { return cnt_of_moves; }

    virtual ~Piece() = default;

  protected:
    Piece(geometry::Point coordinates, Color color, PieceType name):
          coordinates(coordinates), color(color), name(name) {}

    friend class Move;
    friend class PromotionMove;
    friend class CastleMove;
};

class WalkingPiece : public Piece {
    inline virtual const std::vector<Vector>& get_directions() const = 0;

  public:
    std::vector<std::shared_ptr<Move>> getMoves(Position&, get_piece_func_type, last_move_func_type) override;

    bool canBeCaptured(Position&, get_piece_func_type, const Point&) override;

    virtual ~WalkingPiece() = default;

  protected:
    template<typename... Args>
    explicit WalkingPiece(Args&& ... args): Piece(std::forward<Args>(args)...) {}
};

class JumpingPiece : public Piece {
    inline virtual const std::vector<Vector>& get_moves() const = 0;

  public:
    std::vector<std::shared_ptr<Move>> getMoves(Position&, get_piece_func_type, last_move_func_type) override;

    bool canBeCaptured(Position&, get_piece_func_type, const Point&) override;

    virtual ~JumpingPiece() = default;

  protected:
    template<typename... Args>
    explicit JumpingPiece(Args&& ... args): Piece(std::forward<Args>(args)...) {}
};

class Rook : public WalkingPiece {
    static const std::vector<Vector> directions;

    inline const std::vector<Vector>& get_directions() const override { return directions; }

  public:
    template<typename... Args>
    explicit Rook(Args&& ... args): WalkingPiece(std::forward<Args>(args)..., PieceType::rook) {}
};

class Knight : public JumpingPiece {
    static const std::vector<Vector> moves;

    inline const std::vector<Vector>& get_moves() const override { return moves; }

  public:
    template<typename... Args>
    explicit Knight(Args&& ... args): JumpingPiece(std::forward<Args>(args)..., PieceType::knight) {}
};

class Bishop : public WalkingPiece {
    static const std::vector<Vector> directions;

    inline const std::vector<Vector>& get_directions() const override { return directions; }
  public:
    template<typename... Args>
    explicit Bishop(Args&& ... args): WalkingPiece(std::forward<Args>(args)..., PieceType::bishop) {}
};

class King : public JumpingPiece {
    static const std::vector<Vector> moves;

    inline const std::vector<Vector>& get_moves() const override { return moves; }

  public:
    template<typename... Args>
    explicit King(Args&& ... args): JumpingPiece(std::forward<Args>(args)..., PieceType::king) {}
};

class Queen : public WalkingPiece {
    static const std::vector<Vector> directions;

    inline const std::vector<Vector>& get_directions() const override { return directions; }

  public:
    template<typename... Args>
    explicit Queen(Args&& ... args): WalkingPiece(std::forward<Args>(args)..., PieceType::queen) {}
};

class Pawn : public Piece {
    static const Vector one_step[2];
    static const Vector attack_moves[2][2];
    static const Vector enpassant_target[2];
    static const value_type promotion_y[2];
  public:
    std::vector<std::shared_ptr<Move>> getMoves(Position&, get_piece_func_type, last_move_func_type) override;

    bool canBeCaptured(Position&, get_piece_func_type, const Point&) override;

  public:
    template<typename... Args>
    explicit Pawn(Args&& ... args): Piece(std::forward<Args>(args)..., PieceType::pawn) {}
};

