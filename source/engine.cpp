#include "eval.h"
#include "main.h"
#include <sstream>
#include <memory>

double evaluate(Board& position, int depth_left, Color current, double white_best = -100, double black_best = 100) {
    if (depth_left == 0) return evaluate(position);
    std::vector<std::pair<double, std::shared_ptr<Move>>> options;
    for (auto move : position.get_all_moves()) {
        position.make_move(move);
        options.emplace_back(evaluate(position), move);
        position.rollback_move();
    }
    std::sort(options.begin(), options.end(), [current](auto a, auto b) -> bool {
        if (current == white) return a.first > b.first;
        return a.first < b.first;
    });
    double best = current == white ? -100 : 100;
    for (auto [_, move] : options) {
        position.make_move(move);
        if (current == white) {
            double next = evaluate(position, depth_left - 1, black, std::max(white_best, best), black_best);
            if (next > black_best) return next;
            best = std::max(best, next);
        } else {
            double next = evaluate(position, depth_left - 1, white, white_best, std::min(black_best, best));
            if (next < white_best) return next;
            best = std::min(best, next);
        }
        position.rollback_move();
    }
    return best;
}

std::shared_ptr<Move> get_best_move(Board& position) {
    std::vector<std::shared_ptr<Move>> moves = position.get_all_moves();
    std::vector<double> evaluation;
    Color current = position.get_color();
    for (auto move : moves) {
        position.make_move(move);
        evaluation.push_back(evaluate(position, 0, current));
        position.rollback_move();
    }
    auto it = evaluation.end();
    if (current == white) {
        it = std::max_element(evaluation.begin(), evaluation.end());
    } else {
        it = std::min_element(evaluation.begin(), evaluation.end());
    }
    if (it == evaluation.end()) {
        throw std::out_of_range("mo moves");
    }
    return moves[it - evaluation.begin()];
}

int main() {
    Board game;
    std::unordered_map<size_t, size_t> position_count;
    position_count[game.get_hash(20)] = 1;
    std::string type;
    int a, b, c, d;
    while (true) {
        std::cin >> type;
        if (type != "generate") {
            std::stringstream input;
            std::cin >> a >> b >> c >> d;
            std::string to = "";
            if (type == "make_move") {
                std::cin >> to;
            }
            input << type << " " << a << " " << b << " " << c << " " << d << " " << to << "\n";
            if (type != "make_move") {
                std::cout << process_query(position_count, game, input) << "\n";
                std::cout.flush();
            } else {
                process_query(position_count, game, input);
            }
        } else {
            std::shared_ptr<Move> move = get_best_move(game);
            auto promotion_ptr = dynamic_cast<PromotionMove*>(&(*move));
            auto castle_ptr = dynamic_cast<const CastleMove*>(&(*move));
            if (promotion_ptr) {
                geometry::Point from = promotion_ptr->getFrom(), to = promotion_ptr->getTo();
                std::cout << from.get_x() << " " << from.get_y() << " " << to.get_x() << " " << to.get_y() << " " << promotion_ptr->getNewPiece()->getName();
            } else if (castle_ptr) {
                geometry::Point from = castle_ptr->getKingFrom(), to = castle_ptr->getKingTo();
                std::cout << from.get_x() << " " << from.get_y() << " " << to.get_x() << " " << to.get_y() << " none ";
                from = castle_ptr->getRookFrom();
                to = castle_ptr->getRookTo();
                std::cout << from.get_x() << " " << from.get_y() << " " << to.get_x() << " " << to.get_y() << " none";
            } else {
                geometry::Point from = move->getFrom(), to = move->getTo();
                std::cout << from.get_x() << " " << from.get_y() << " " << to.get_x() << " " << to.get_y() << " none";
            }
            std::cout << "\n";
            std::cout.flush();
            game.make_move(move);
        }
    }
}
