#include <string>
#include <functional>
#include "board.h"

Vector castle_direction[2] = {Vector(-1, 0), Vector(1, 0)};

bool Board::valid(std::shared_ptr<Move> mv) {
    cur_pos = cur_pos->makeMove(mv);
    bool ok = true;
    std::shared_ptr<Piece> king;

    for (auto piece : start.all_pieces) {
        if (piece->getName() == PieceType::king && piece->getColor() != start.which_turn) king = piece;
    }

    for (auto piece : start.all_pieces) {
        if (piece->getColor() != start.which_turn || piece->getIsCaptured()) continue;
        if (piece->canBeCaptured(*cur_pos, &Position::get_piece, king->getCoordinates())) ok = false;
    }

    auto cur = dynamic_cast<FollowingPosition*>(cur_pos);
    cur_pos = cur->rollbackMove();
    delete cur;
    return ok;
}

std::vector<std::shared_ptr<Move>> Board::get_all_moves() {
    std::vector<std::shared_ptr<Move>> vec = get_castle_moves();
    for (auto piece : start.all_pieces) {
        if (piece->getColor() != start.which_turn || piece->getIsCaptured()) continue;
        std::vector<std::shared_ptr<Move>> new_moves =
             piece->getMoves(*cur_pos, &Position::get_piece, &Position::get_last_move);
        vec.insert(vec.end(), new_moves.begin(), new_moves.end());
    }
    std::vector<std::shared_ptr<Move>> right_moves;
    std::copy_if(vec.begin(), vec.end(), std::back_inserter(right_moves), [this](const auto& mv) {
        return valid(mv);
    });
    return right_moves;
}

std::vector<std::shared_ptr<Move>> Board::get_castle_moves() {
    std::shared_ptr<Piece> king;
    std::vector<std::shared_ptr<Piece>> rooks;
    std::vector<std::shared_ptr<Move>> castle_moves;
    for (auto piece : start.all_pieces) {
        if (piece->getColor() != start.which_turn || piece->getIsCaptured()) continue;
        if (piece->getName() == PieceType::king) king = piece;
        if (piece->getName() == PieceType::rook) rooks.push_back(piece);
    }
    if (king->getCnt() != 0) return {};
    for (auto rook : rooks) {
        if (rook->getCnt() != 0) continue;
        for (auto dir : castle_direction) {
            if (!rook->getCoordinates().onRay(king->getCoordinates(), dir)) continue;
            if (!rook->canBeCaptured(*cur_pos, &Position::get_piece, king->getCoordinates())) continue;
            bool is_free = true;
            for (auto piece : start.all_pieces) {
                if (piece->getColor() == king->getColor() || piece->getIsCaptured()) continue;
                for (value_type i = 0; i <= 2; ++i) {
                    if (piece->canBeCaptured(*cur_pos, &Position::get_piece,
                                             king->getCoordinates() + dir * i)) is_free = false;
                }
            }
            if (!is_free) continue;
            castle_moves.push_back(std::shared_ptr<Move>(dynamic_cast<Move*>(
                    new CastleMove(king, rook, king->getCoordinates(),
                                   king->getCoordinates() + dir * 2, rook->getCoordinates(),
                     king->getCoordinates() + dir))));
        }
    }
    return castle_moves;
}

Board::~Board() {
    while (cur_pos != &start) {
        auto cur = dynamic_cast<FollowingPosition*>(cur_pos);
        cur_pos = cur->prev;
        delete cur;
    }
}

bool Board::is_mate() {
    std::shared_ptr<Piece> king;

    for (auto piece : start.all_pieces) {
        if (piece->getName() == PieceType::king && piece->getColor() != start.which_turn) king = piece;
    }

    for (auto piece : start.all_pieces) {
        if (piece->getColor() == start.which_turn || piece->getIsCaptured()) continue;
        if (piece->canBeCaptured(*cur_pos, &Position::get_piece, king->getCoordinates())) return true;
    }

    return false;
}

bool Board::dead_position() {
    int cnt_of_light[2] = {0, 0};
    for (auto piece : start.all_pieces) {
        if (piece->getIsCaptured()) continue;
        if (piece->getName() == PieceType::queen) return false;
        if (piece->getName() == PieceType::rook) return false;
        if (piece->getName() == PieceType::pawn) return false;
        if (piece->getName() == PieceType::knight) ++cnt_of_light[piece->getColor()];
        if (piece->getName() == PieceType::bishop) ++cnt_of_light[piece->getColor()];
    }
    return cnt_of_light[0] <= 1 && cnt_of_light[1] <= 1;
}

size_t Board::get_hash(size_t count_of_moves) {
    std::string field = "";
    for (value_type i = 0; i < geometry::board_size; ++i) {
        for (value_type j = 0; j < geometry::board_size; ++j) {
            if (!start.used[i][j]) {
                field += ".";
                continue;
            }
            field += start.used[i][j]->getName();
            field += '0' + start.used[i][j]->getColor();
        }
    }
    return std::hash<std::string>()(field + std::to_string(count_of_moves)
                                    + std::to_string(static_cast<bool>(start.which_turn)));
}

bool Board::fifty_moves() {
    size_t count = 0;
    auto pos = cur_pos;
    while (count < 50 && pos != &start) {
        auto following_pos = dynamic_cast<FollowingPosition*>(pos);
        if (following_pos->mv->getCaptured()) return false;
        if (following_pos->mv->getMovedPiece() && following_pos->mv->getMovedPiece()->getName() == PieceType::pawn) return false;
        pos = following_pos->prev;
        ++count;
    }
    return count >= 50;
}

void Board::rollback_move() {
    cur_pos = dynamic_cast<FollowingPosition*>(cur_pos)->rollbackMove();
}
