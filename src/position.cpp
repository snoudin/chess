#include "position.h"

const int cnt_of_pieces = 32;
const int pawn_horizontal = 1;

const Point KingLocation[2] = {{Point(4, 0)},
                               {Point(4, 7)}};
const std::vector<Point> QueenLocation[2] = {{Point(3, 0)},
                                             {Point(3, 7)}};
const std::vector<Point> BishopLocation[2] = {{Point(2, 0), Point(5, 0)},
                                              {Point(2, 7), Point(5, 7)}};
const std::vector<Point> KnightLocation[2] = {{Point(1, 0), Point(6, 0)},
                                              {Point(1, 7), Point(6, 7)}};
const std::vector<Point> RookLocation[2] = {{Point(0, 0), Point(7, 0)},
                                            {Point(0, 7), Point(7, 7)}};
const std::vector<Point> PawnLocation[2] = {{
                                                    Point(0, 1),
                                                    Point(1, 1),
                                                    Point(2, 1),
                                                    Point(3, 1),
                                                    Point(4, 1),
                                                    Point(5, 1),
                                                    Point(6, 1),
                                                    Point(7, 1),
                                            },
                                            {
                                                    Point(0, 6),
                                                    Point(1, 6),
                                                    Point(2, 6),
                                                    Point(3, 6),
                                                    Point(4, 6),
                                                    Point(5, 6),
                                                    Point(6, 6),
                                                    Point(7, 6),
                                            }};

// here customizing starting position
StartingPosition::StartingPosition(): Position(this) {
    all_pieces.reserve(cnt_of_pieces);
    for (auto position : PawnLocation[0]) all_pieces.push_back(std::shared_ptr<Piece>(new Pawn(position, white)));
    for (auto position : PawnLocation[1]) all_pieces.push_back(std::shared_ptr<Piece>(new Pawn(position, black)));
    for (auto position : RookLocation[0]) all_pieces.push_back(std::shared_ptr<Piece>(new Rook(position, white)));
    for (auto position : RookLocation[1]) all_pieces.push_back(std::shared_ptr<Piece>(new Rook(position, black)));
    for (auto position : BishopLocation[0]) all_pieces.push_back(std::shared_ptr<Piece>(new Bishop(position, white)));
    for (auto position : BishopLocation[1]) all_pieces.push_back(std::shared_ptr<Piece>(new Bishop(position, black)));
    for (auto position : KnightLocation[0]) all_pieces.push_back(std::shared_ptr<Piece>(new Knight(position, white)));
    for (auto position : KnightLocation[1]) all_pieces.push_back(std::shared_ptr<Piece>(new Knight(position, black)));
    for (auto position : QueenLocation[0]) all_pieces.push_back(std::shared_ptr<Piece>(new Queen(position, white)));
    for (auto position : QueenLocation[1]) all_pieces.push_back(std::shared_ptr<Piece>(new Queen(position, black)));
    all_pieces.push_back(std::shared_ptr<Piece>(new King(KingLocation[0], white)));
    all_pieces.push_back(std::shared_ptr<Piece>(new King(KingLocation[1], black)));
    for (auto piece : all_pieces) used[piece->getCoordinates().get_x()][piece->getCoordinates().get_y()] = piece;
}

Color StartingPosition::get_color() const {
    return which_turn;
}

std::shared_ptr<Piece> Position::get_piece(const geometry::Point& p) {
    return start->used[p.get_x()][p.get_y()];
}

std::shared_ptr<Move> FollowingPosition::get_last_move() {
    return mv;
}

FollowingPosition* Position::makeMove(std::shared_ptr<Move> mv) {
    mv->makeMove();
    start->which_turn = Color(!static_cast<bool>(start->which_turn));
    auto ptr_castle = dynamic_cast<CastleMove*>(&*mv);
    if (ptr_castle) {
        start->used[ptr_castle->rook_from.get_x()][ptr_castle->rook_from.get_y()] = nullptr;
        start->used[ptr_castle->king_from.get_x()][ptr_castle->king_from.get_y()] = nullptr;
        start->used[ptr_castle->rook_to.get_x()][ptr_castle->rook_to.get_y()] = ptr_castle->rook;
        start->used[ptr_castle->king_to.get_x()][ptr_castle->king_to.get_y()] = ptr_castle->king;
        return new FollowingPosition(mv, this, start);
    }
    start->used[mv->getFrom().get_x()][mv->getFrom().get_y()] = nullptr;
    if (mv->getCaptured()) {
        auto pt = mv->getCaptured()->getCoordinates();
        start->used[pt.get_x()][pt.get_y()] = nullptr;
    }
    start->used[mv->getTo().get_x()][mv->getTo().get_y()] = mv->getMovedPiece();
    auto ptr = dynamic_cast<PromotionMove*>(&*mv);
    if (ptr) {
        start->all_pieces.push_back(ptr->getNewPiece());
        start->used[mv->getTo().get_x()][mv->getTo().get_y()] = ptr->getNewPiece();
    }
    return new FollowingPosition(mv, this, start);
}

Position* FollowingPosition::rollbackMove() {
    mv->rollbackMove();
    start->which_turn = Color(!static_cast<bool>(start->which_turn));
    auto ptr_castle = dynamic_cast<CastleMove*>(&*mv);
    if (ptr_castle) {
        start->used[ptr_castle->rook_to.get_x()][ptr_castle->rook_to.get_y()] = nullptr;
        start->used[ptr_castle->king_to.get_x()][ptr_castle->king_to.get_y()] = nullptr;
        start->used[ptr_castle->rook_from.get_x()][ptr_castle->rook_from.get_y()] = ptr_castle->rook;
        start->used[ptr_castle->king_from.get_x()][ptr_castle->king_from.get_y()] = ptr_castle->king;
        return prev;
    }
    start->used[mv->getTo().get_x()][mv->getTo().get_y()] = nullptr;
    if (mv->getCaptured()) {
        auto pt = mv->getCaptured()->getCoordinates();
        start->used[pt.get_x()][pt.get_y()] = mv->getCaptured();
    }
    start->used[mv->getFrom().get_x()][mv->getFrom().get_y()] = mv->getMovedPiece();
    auto ptr = dynamic_cast<PromotionMove*>(&*mv);
    if (ptr) {
        start->all_pieces.erase(std::find(start->all_pieces.begin(), start->all_pieces.end(), ptr->getNewPiece()));
    }
    return prev;
}

std::shared_ptr<Move> StartingPosition::get_last_move() {
    return nullptr;
}
