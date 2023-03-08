/*
** EPITECH PROJECT, 2022
** gomoku
** File description:
** Ai
*/

#ifndef AI_HPP_
#define AI_HPP_

#include "Protocol.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <regex>
#include <memory>
#include <algorithm>

class Ai {
    public:
        enum {
            HORIZONTAL,
            VERTICAL,
            DIAGONAL_RIGHT,
            DIAGONAL_LEFT
        };
        enum Player{
            EMPTY,
            PLAYER,
            OPPONENT
        };

        typedef struct node_t {
            int x = -1;
            int y = -1;
            int score = -100000;
        } node_t;

        Ai(std::shared_ptr<Protocol>);
        ~Ai();
        void createMap(int size);
        void updateMap(int x, int y, int player);
        std::vector<int> getMapCopy(std::vector<int> map, int x, int y, int player);
        void turn(int x = -1, int y = -1);
        void print_map(std::vector<int> board);
        void begin();
        void beginAfterBoard();
        bool check_win(int player);
        void board();
        void end();
        void restart();

        int evaluate(std::vector<int> map, bool opponentTurn, int player);
        int calcConsecutiveScore(int obstructed, int consecutive, int whoseTurn);
        int eval_horizontal(std::vector<int> map, bool opponentTurn, int player);
        int eval_vertical(std::vector<int> map, bool opponentTurn, int player);
        int eval_diagonal(std::vector<int> map, bool opponentTurn, int player);

        node_t minimax(std::vector<int> board, int depth, int alpha, int beta, int maximazingPlayer);
        std::vector<std::tuple<int, int>> getPossibleMoves(std::vector<int> map);
        node_t get_next_move(int depth);
        int getMax (int a, int b);
        int getMin (int a, int b);
        node_t easywin();
    protected:
    private:
        bool isinGame;
        std::vector<int> _map;
        int _size;
        int _turn;
        std::shared_ptr<Protocol> _protocol;
};

#endif /* !AI_HPP_ */
