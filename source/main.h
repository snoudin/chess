#pragma once

#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>
#include <vector>
#include "../headers/board.h"

std::unordered_map<std::string, PieceType> from_string_to_piecetype = {
        {"Pawn", PieceType::pawn},
        {"King", PieceType::king},
        {"Queen", PieceType::queen},
        {"Bishop", PieceType::bishop},
        {"Knight", PieceType::knight},
        {"Rook", PieceType::rook}
};

std::string out(bool code) {
    return std::string() + (code ? '1' : '0');
}

std::string process_query(std::unordered_map<size_t, size_t>& position_count,
                          Board& game, std::basic_istream<char>& cin) {
    std::string s;
    int x_from, y_from, x_to, y_to;
    cin >> s >> x_from >> y_from >> x_to >> y_to;
    geometry::Point from(x_from, y_from);
    geometry::Point to(x_to, y_to);
    std::vector<std::shared_ptr<Move>> moves = game.get_all_moves();
    if (s == "check") {
        auto it = std::find_if(moves.begin(), moves.end(), [from, to](const auto& move) {
            auto ptr = std::dynamic_pointer_cast<CastleMove>(move);
            if (ptr) {
                return ptr->getKingFrom() == from && ptr->getKingTo() == to;
            }
            return move->getFrom() == from && move->getTo() == to;
        });
        return out(it != moves.end());
    } else if (s == "is_promotion") {
        auto it = std::find_if(moves.begin(), moves.end(), [from, to](const auto& move) {
            auto ptr = dynamic_cast<PromotionMove*>(&(*move));
            if (ptr) {
                return ptr->getFrom() == from && ptr->getTo() == to;
            }
            return false;
        });
        return out(it != moves.end());
    } else if (s == "is_castle") {
        auto it = std::find_if(moves.begin(), moves.end(), [from, to](const auto& move) {
            auto ptr = dynamic_cast<const CastleMove*>(&(*move));
            if (ptr) {
                return ptr->getKingFrom() == from && ptr->getKingTo() == to;
            }
            return false;
        });
        return out(it != moves.end());
    } else if (s == "make_move") {
        std::string name;
        cin >> name;
        auto it = std::find_if(moves.begin(), moves.end(), [from, to, name](const auto& move) {
            auto ptr = dynamic_cast<const PromotionMove*>(&(*move));
            if (ptr) {
                return ptr->getFrom() == from && ptr->getTo() == to &&
                       (name == "none" || ptr->getNewPiece()->getName() == from_string_to_piecetype[name]);
            }
            auto ptr2 = dynamic_cast<const CastleMove*>(&(*move));
            if (ptr2) {
                return ptr2->getKingFrom() == from && ptr2->getKingTo() == to;
            }
            return move->getFrom() == from && move->getTo() == to;
        });
        if (it == moves.end()) return "shit!!!";
        game.make_move(*it);
        size_t hash = game.get_hash(game.get_all_moves().size());
        if (position_count.find(hash) == position_count.end()) position_count[hash] = 0;
        ++position_count[hash];
        return "";
    } else if (s == "sth_taken") {
        auto it = std::find_if(moves.begin(), moves.end(), [from, to](const auto& move) {
            return move->getFrom() == from && move->getTo() == to;
        });
        if (it == moves.end()) return "none";
        else if (!(*it)->getCaptured()) return "none";
        else
            return
            std::to_string((*it)->getCaptured()->getCoordinates().get_x())
            + " "
            + std::to_string((*it)->getCaptured()->getCoordinates().get_y());
    } else if (s == "castle") {
        auto it = std::find_if(moves.begin(), moves.end(), [from, to](const auto& move) {
            auto ptr = dynamic_cast<const CastleMove*>(&(*move));
            if (ptr) {
                return ptr->getKingFrom() == from && ptr->getKingTo() == to;
            }
            return false;
        });
        if (it == moves.end()) {
            return "shit!!!";
        } else {
            auto mv = dynamic_cast<CastleMove*>(&**it);
            return
            std::to_string(mv->getRookFrom().get_x())
            + " "
            + std::to_string(mv->getRookFrom().get_y())
            + " "
            + std::to_string(mv->getRookTo().get_x())
            + " "
            + std::to_string(mv->getRookTo().get_y());
        }
    } else if (s == "result") {
        if (moves.size()) {
            size_t hash = game.get_hash(moves.size());
            if (position_count.find(hash) != position_count.end() && position_count[hash] >= 3) return "0";
            if (game.dead_position()) return "0";
            if (game.fifty_moves()) return "0";
            return "none";
        } else {
            if (game.is_mate()) {
                if (game.get_color()) return "-1";
                return "1";
            } else {
                return "0";
            }
        }
    }
    return "Invalid query";
}
