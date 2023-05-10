#include "move.h"
#include "piece.h"
#include "gvector.h"

Move::Move(std::shared_ptr<Piece> moved_piece, const geometry::Point& from, const geometry::Point& to, std::shared_ptr<Piece> captured):
moved_piece(std::move(moved_piece)), from(from), to(to), captured(std::move(captured)) {}

PromotionMove::PromotionMove(const Move& mv, std::shared_ptr<Piece> new_piece): Move(mv), new_piece(std::move(new_piece)) {}

void Move::makeMove() const {
    ++moved_piece->cnt_of_moves;
    moved_piece->coordinates = to;
    if (captured) captured->is_captured = true;
}

void Move::rollbackMove() const {
    --moved_piece->cnt_of_moves;
    moved_piece->coordinates = from;
    if (captured) captured->is_captured = false;
}

void CastleMove::makeMove() const {
    ++king->cnt_of_moves;
    ++rook->cnt_of_moves;
    king->coordinates = king_to;
    rook->coordinates = rook_to;
}

void CastleMove::rollbackMove() const {
    --king->cnt_of_moves;
    --rook->cnt_of_moves;
    king->coordinates = king_from;
    rook->coordinates = rook_from;
}