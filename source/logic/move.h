//
// Created by vitali on 15.03.23.
//
#pragma once
#include <memory>
#include "gvector.h"

#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

class Piece;

class Move {
protected:
    std::shared_ptr<Piece> moved_piece = nullptr;
    geometry::Point from = geometry::Point();
    geometry::Point to = geometry::Point();
    std::shared_ptr<Piece> captured = nullptr;
public:
    inline std::shared_ptr<Piece> getMovedPiece() const { return moved_piece; }

    inline const geometry::Point& getFrom() const { return from; }

    inline const geometry::Point& getTo() const { return to; }

    inline std::shared_ptr<Piece> getCaptured() const { return captured; }

    virtual void makeMove() const;

    virtual void rollbackMove() const;

    Move(std::shared_ptr<Piece>, const geometry::Point&, const geometry::Point&, std::shared_ptr<Piece>);

    Move() = default;

    virtual ~Move() = default;
};

class PromotionMove : public Move {
    std::shared_ptr<Piece> new_piece;

public:
    inline std::shared_ptr<Piece> getNewPiece() const { return new_piece; }

    PromotionMove(const Move&, std::shared_ptr<Piece>);

    ~PromotionMove() override = default;
};

class CastleMove : public Move {
    std::shared_ptr<Piece> king;
    std::shared_ptr<Piece> rook;
    geometry::Point king_from;
    geometry::Point king_to;
    geometry::Point rook_from;
    geometry::Point rook_to;

    CastleMove(std::shared_ptr<Piece> king,
               std::shared_ptr<Piece> rook,
               geometry::Point king_from,
               geometry::Point king_to,
               geometry::Point rook_from,
               geometry::Point rook_to) : king(king), rook(rook), king_from(king_from), king_to(king_to),
                                          rook_from(rook_from), rook_to(rook_to) {}

    void makeMove() const override;

    void rollbackMove() const override;

    friend class Position;

public:

    geometry::Point getKingFrom() const { return king_from; }

    geometry::Point getKingTo() const { return king_to; }

    geometry::Point getRookFrom() const { return rook_from; }

    geometry::Point getRookTo() const { return rook_to; }

    friend class FollowingPosition;

    friend class Board;

    ~CastleMove() override = default;
};

#endif //CHESS_MOVE_H
