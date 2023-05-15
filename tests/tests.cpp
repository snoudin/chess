#include <string>
#include <sstream>
#include <unordered_map>
#include "tests.h"
#include "fstream"


const std::string path_to_tests = "tests_data/";


TEST_F(BoardTestCase, test1) {
    Board game;
    std::unordered_map<size_t, size_t> position_count;
    std::ifstream cin(path_to_tests + "test1.in");
    for (int i = 0; i < 8; ++i) {
        ASSERT_EQ("1", process_query(position_count, game, cin));
    }
}

void usual_move(std::unordered_map<size_t, size_t>& position_count, Board& game, std::ifstream& cin) {
    ASSERT_EQ("1", process_query(position_count, game, cin));
    ASSERT_EQ("0", process_query(position_count, game, cin));
    ASSERT_EQ("0", process_query(position_count, game, cin));
    process_query(position_count, game, cin);
}

TEST_F(BoardTestCase, test2) {
    Board game;
    std::unordered_map<size_t, size_t> position_count;
    std::ifstream cin(path_to_tests + "test2.in");
    usual_move(position_count, game, cin);
    ASSERT_EQ("1", process_query(position_count, game, cin));
}

TEST_F(BoardTestCase, test3) {
    Board game;
    std::unordered_map<size_t, size_t> position_count;
    std::ifstream cin(path_to_tests + "test3.in");
    usual_move(position_count, game, cin);
    ASSERT_EQ("1", process_query(position_count, game, cin));
}

TEST_F(BoardTestCase, test4) {
    Board game;
    std::ifstream cin(path_to_tests + "test4.in");
    std::unordered_map<size_t, size_t> position_count;
    process_query(position_count, game, cin);
    process_query(position_count, game, cin);
    process_query(position_count, game, cin);
    process_query(position_count, game, cin);
    process_query(position_count, game, cin);
    process_query(position_count, game, cin);
    ASSERT_EQ("1", process_query(position_count, game, cin));
    process_query(position_count, game, cin);
    ASSERT_EQ("0", process_query(position_count, game, cin));  // pawn taken en-passant does not exist
}

void check_output(std::unordered_map<size_t, size_t>& position_count, Board& game,
                  std::ifstream& cin, std::ifstream& test_out) {
    std::string t;
    std::getline(test_out, t);
    ASSERT_EQ(t, process_query(position_count, game, cin));
}


// Here real tests starts


void test_while_input(std::unordered_map<size_t, size_t>& position_count, Board& game,
                      std::ifstream& cin, std::ifstream& test_out) {
    std::string request;
    while (cin >> request) {
        if (request.size() == 1) return;
        std::stringstream test_in;
        test_in << request << " ";
        std::string correct = "";
        if (request != "make_move") {
            std::getline(test_out, correct);
            int a, b, c, d;
            cin >> a >> b >> c >> d;
            test_in << a << " " << b << " " << c << " " << d << " ";
        }
        ASSERT_EQ(correct, process_query(position_count, game, test_in));
    }
}


TEST_F(BoardTestCase, test_capture_promotion) {
    Board game;
    std::ifstream cin(path_to_tests + "test5.in");
    std::unordered_map<size_t, size_t> position_count;
    std::ifstream test_out(path_to_tests + "test5.out");
    test_while_input(position_count, game, cin, test_out);
}

TEST_F(BoardTestCase, test_castle) {
    Board game;
    std::ifstream cin(path_to_tests + "test6.in");
    std::ifstream test_out(path_to_tests + "test6.out");
    std::unordered_map<size_t, size_t> position_count;
    test_while_input(position_count, game, cin, test_out);
}

TEST_F(BoardTestCase, test7) {
    Board game;
    std::ifstream cin(path_to_tests + "test7.in");
    std::ifstream test_out(path_to_tests + "test7.out");
    std::unordered_map<size_t, size_t> position_count;
    test_while_input(position_count, game, cin, test_out);
}


TEST_F(BoardTestCase, test8) {
    Board game;
    std::unordered_map<size_t, size_t> position_count;
    std::ifstream cin(path_to_tests + "test8.in");
    std::ifstream test_out(path_to_tests + "test8.out");
    test_while_input(position_count, game, cin, test_out);
}

TEST_F(BoardTestCase, test9) {
    Board game;
    std::unordered_map<size_t, size_t> position_count;
    std::ifstream cin(path_to_tests + "test9.in");
    std::ifstream test_out(path_to_tests + "test9.out");
    test_while_input(position_count, game, cin, test_out);
}


TEST_F(BoardTestCase, test10) {
    Board game;
    std::unordered_map<size_t, size_t> position_count;
    std::ifstream cin(path_to_tests + "test10.in");
    std::ifstream test_out(path_to_tests + "test10.out");
    test_while_input(position_count, game, cin, test_out);
}

TEST_F(BoardTestCase, some_big_test) {
    Board game;
    std::unordered_map<size_t, size_t> position_count;
    std::ifstream cin(path_to_tests + "test11.in");
    std::ifstream test_out(path_to_tests + "test11.out");
    test_while_input(position_count, game, cin, test_out);
}

TEST_F(BoardTestCase, test12) {
    Board game;
    std::unordered_map<size_t, size_t> position_count;
    position_count[game.get_hash(20)] = 1;
    std::ifstream cin(path_to_tests + "test12.in");
    std::ifstream test_out(path_to_tests + "test12.out");
    test_while_input(position_count, game, cin, test_out);
}

void debug_out_board(Board& game) {
    for (value_type j = board_size - 1; j > -1; --j) {
        for (value_type i = 0; i < board_size; ++i) {
            if (!game.start.used[i][j]) {
                std::cout << ".";
                continue;
            }
            char c;
            auto piece_type = game.start.used[i][j]->getName();
            if (piece_type == PieceType::rook) c = 'R';
            if (piece_type == PieceType::king) c = 'K';
            if (piece_type == PieceType::bishop) c = 'B';
            if (piece_type == PieceType::knight) c ='N';
            if (piece_type == PieceType::pawn) c = 'P';
            if (piece_type == PieceType::queen)  c = 'Q';
            if (!game.start.used[i][j]->getColor()) c = tolower(c);
            std::cout << std::string(1, c);
        }
        std::cout << "\n";
    }
    std::cout << "\n\n";
}

TEST_F(BoardTestCase, test13) {
    Board game;
    std::unordered_map<size_t, size_t> position_count;
    position_count[game.get_hash(20)] = 1;
    std::ifstream cin(path_to_tests + "test13.in");
    std::ifstream test_out(path_to_tests + "test13.out");
    test_while_input(position_count, game, cin, test_out);
}

TEST_F(BoardTestCase, test14) {
    Board game;
    std::ifstream cin(path_to_tests + "test14.in");
    std::ifstream test_out(path_to_tests + "test14.out");
    std::unordered_map<size_t, size_t> position_count;
    test_while_input(position_count, game, cin, test_out);
}

TEST_F(BoardTestCase, deadPositionTest) {
    Board game;
    std::unordered_map<size_t, size_t> position_count;
    position_count[game.get_hash(20)] = 1;
    std::ifstream cin(path_to_tests + "test15.in");
    std::ifstream test_out(path_to_tests + "test15.out");
    test_while_input(position_count, game, cin, test_out);
}

