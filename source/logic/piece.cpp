//
// Created by vitali on 12.03.23.
//
#include "piece.h"
#include "move.h"
#include "gvector.h"
#include <algorithm>
#include <memory>

std::vector<std::shared_ptr<Move>> WalkingPiece::getMoves(Position& board, get_piece_func_type get_piece, last_move_func_type) {
    std::vector<std::shared_ptr<Move>> return_moves;
    for (Vector v: get_directions()) {
        Point to = coordinates;
        for (value_type i = 1; i <= board_size; ++i) {
            to += v;
            if (to.outOfBounds()) break;
            auto move_candidate = std::make_shared<Move>(shared_from_this(), coordinates, to, (board.*get_piece)(to));
            if (move_candidate->getCaptured()) {
                if (move_candidate->getCaptured()->getColor() != color) return_moves.push_back(move_candidate);
                break;
            } else {
                return_moves.push_back(move_candidate);
            }
        }
    }
    return return_moves;
}

std::vector<std::shared_ptr<Move>> JumpingPiece::getMoves(Position& board, get_piece_func_type get_piece, last_move_func_type) {
    std::vector<std::shared_ptr<Move>> return_moves;
    for (Vector v: get_moves()) {
        Point to = coordinates + v;
        if (to.outOfBounds()) continue;
        auto move_candidate = std::make_shared<Move>(shared_from_this(), coordinates, to, (board.*get_piece)(to));
        if (move_candidate->getCaptured()) {
            if (move_candidate->getCaptured()->getColor() != color) return_moves.push_back(move_candidate);
        } else {
            return_moves.push_back(move_candidate);
        }
    }
    return return_moves;
}

bool WalkingPiece::canBeCaptured(Position& board, get_piece_func_type get_piece, const Point& position) {
    for (Vector v: get_directions()) {
        if (!position.onRay(coordinates, v)) continue;
        Point to = coordinates;
        for (value_type i = 1; i <= board_size; ++i) {
            to += v;
            if (to == position) return true;
            if ((board.*get_piece)(to)) return false;
        }
        return false;
    }
    return false;
}

bool JumpingPiece::canBeCaptured(Position& board, get_piece_func_type get_piece, const Point& position) {
    return std::any_of(get_moves().begin(), get_moves().end(), [&](Vector v) {
        return coordinates + v == position;
    });
}

std::vector<std::shared_ptr<Move>> Pawn::getMoves(Position& board, get_piece_func_type get_piece, last_move_func_type last_move) {
    std::vector<std::shared_ptr<Move>> return_moves;
    Point to = coordinates + one_step[color];
    auto this_shared = shared_from_this();
    if (!to.outOfBounds() && !(board.*get_piece)(to)) {
        auto mv = std::make_shared<Move>(this_shared, coordinates, to, nullptr);
        if (to.get_y() == promotion_y[color]) {
            return_moves.push_back(std::shared_ptr<Move>(dynamic_cast<Move*>(new PromotionMove(*mv, std::shared_ptr<Piece>(new Knight(to, color))))));
            return_moves.push_back(std::shared_ptr<Move>(dynamic_cast<Move*>(new PromotionMove(*mv, std::shared_ptr<Piece>(new Bishop(to, color))))));
            return_moves.push_back(std::shared_ptr<Move>(dynamic_cast<Move*>(new PromotionMove(*mv, std::shared_ptr<Piece>(new Rook(to, color))))));
            return_moves.push_back(std::shared_ptr<Move>(dynamic_cast<Move*>(new PromotionMove(*mv, std::shared_ptr<Piece>(new Queen(to, color))))));
        } else {
            return_moves.push_back(mv);
        }
        if (cnt_of_moves == 0 && !(board.*get_piece)(to + one_step[color])) {
            return_moves.push_back(std::make_shared<Move>(this_shared, coordinates, to + one_step[color], nullptr));
        }
    }
    for (value_type i = 0; i < 2; ++i) {
        to = coordinates + attack_moves[color][i];
        if (to.outOfBounds()) continue;
        auto piece = (board.*get_piece)(to);
        std::shared_ptr<Move> last_mv = (board.*last_move)();
        if (piece && piece->getColor() != color) {
            Move mv = Move(shared_from_this(), coordinates, to, piece);
            if (to.get_y() == promotion_y[color]) {
                return_moves.push_back(std::shared_ptr<Move>(dynamic_cast<Move*>(new PromotionMove(mv, std::shared_ptr<Piece>(new Knight(to, color))))));
                return_moves.push_back(std::shared_ptr<Move>(dynamic_cast<Move*>(new PromotionMove(mv, std::shared_ptr<Piece>(new Bishop(to, color))))));
                return_moves.push_back(std::shared_ptr<Move>(dynamic_cast<Move*>(new PromotionMove(mv, std::shared_ptr<Piece>(new Rook(to, color))))));
                return_moves.push_back(std::shared_ptr<Move>(dynamic_cast<Move*>(new PromotionMove(mv, std::shared_ptr<Piece>(new Queen(to, color))))));
            } else return_moves.push_back(std::make_shared<Move>(mv));
        } else if (last_mv
                   && last_mv->getTo() == coordinates + enpassant_target[i]
                   && last_mv->getMovedPiece()->getName() == "Pawn"
                   && abs(last_mv->getTo().get_y() - last_mv->getFrom().get_y()) == 2) { // 2 means that pawn did long move
            return_moves.push_back(std::make_shared<Move>(shared_from_this(), coordinates, to, last_mv->getMovedPiece()));
        }
    }
    return return_moves;
}

bool Pawn::canBeCaptured(Position& board, get_piece_func_type get_piece, const Point& position) {
    if (coordinates + attack_moves[color][0] == position) return true;
    if (coordinates + attack_moves[color][1] == position) return true;
    return false;
}

const std::vector<Vector>
        Rook::directions = {Vector(1, 0), Vector(-1, 0), Vector(0, 1), Vector(0, -1)};
const std::vector<Vector>
        Bishop::directions = {Vector(1, 1), Vector(1, -1), Vector(-1, 1), Vector(-1, -1)};
const std::vector<Vector>
        Queen::directions = {Vector(1, 0), Vector(-1, 0), Vector(0, 1), Vector(0, -1),
                             Vector(1, 1), Vector(1, -1), Vector(-1, 1), Vector(-1, -1)};
const Point Pawn::one_step[2] = {Vector(0, -1), Vector(0, 1)};
const Point Pawn::attack_moves[2][2] = {{Vector(1, -1), Vector(-1, -1)},
                                        {Vector(1, 1),  Vector(-1, 1)}};
const Point Pawn::enpassant_target[2] = {Vector(1, 0), Vector(-1, 0)};
const value_type Pawn::promotion_y[2] = {0, 7};
const std::vector<Vector> King::moves = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {-1, -1}, {-1, 1}, {1, -1}, {1, 1}};
const std::vector<Vector> Knight::moves = {{1, 2}, {1, -2}, {2, 1}, {2, -1}, {-1, 2}, {-1, -2}, {-2, 1}, {-2, -1}};