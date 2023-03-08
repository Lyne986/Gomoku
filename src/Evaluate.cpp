/*
** EPITECH PROJECT, 2022
** B-AIA-500-MPL-5-1-gomoku-diogo.faria-martins
** File description:
** Evaluate
*/

#include "Ai.hpp"

int Ai::calcConsecutiveScore(int obstructed, int consecutive, int whoseTurn)
{
    if (obstructed == 2 && consecutive < 5)
        return (0);

    if (consecutive == 1)
        return 1;

    if (consecutive == 5)
        return (100000000);

    if (consecutive == 4) {

        if (whoseTurn) {
            return (1000000);
        } else {
            if (obstructed == 0)
                return (1000000 / 4);
            else
                return (200);

        }
    }

    if (consecutive == 3) {
        if (obstructed == 0) {
            if (whoseTurn) {
                return (50000);
            } else
                return (200);
        } else {
            if(whoseTurn)
                return 10;
            else
                return 5;
        }
    }
    if (consecutive == 2) {
        if (obstructed == 0) {
            if (whoseTurn)
                return 7;
            else
                return 5;
        }
        else {
            return 3;
        }
    }
    return (100000000 * 2);
}

int Ai::eval_horizontal(std::vector<int> map, bool opponentTurn, int player)
{
    int count = 0;
    int trap = 2;
    int score = 0;
    int turn = (opponentTurn && player == Player::OPPONENT ? 1 : 0);

    for (int y = 0; y < _size; y++) {
        for (int x = 0; x < _size; x++) {
            if (map[y * _size + x] == player) {
                count++;
            } else if (map[y * _size + x] == 0) {
                if (count > 0) {
                    score += calcConsecutiveScore(trap - 1, count, turn);
                    trap = 1;
                    count = 0;
                } else {
                    trap = 1;
                }
            } else if (count > 0) {
                score += calcConsecutiveScore(trap, count, turn);
                count = 0;
                trap = 2;
            } else
                trap = 2;
        }
        if (count > 0)
            score += calcConsecutiveScore(trap, count, turn);
        count = 0;
        trap = 2;
    }
    return score;
}

int Ai::eval_vertical(std::vector<int> map, bool opponentTurn, int player)
{
    int count = 0;
    int trap = 2;
    int score = 0;
    int turn = (opponentTurn && player == Player::OPPONENT ? 1 : 0);

    for (int y = 0; y < _size; y++) {
        for (int x = 0; x < _size; x++) {
            if (map[x * _size + y] == player) {
                count++;
            } else if (map[x * _size + y] == 0) {
                if (count > 0) {
                    score += calcConsecutiveScore(trap - 1, count, turn);
                    trap = 1;
                    count = 0;
                } else {
                    trap = 1;
                }
            } else if (count > 0) {
                score += calcConsecutiveScore(trap, count, turn);
                count = 0;
                trap = 2;
            } else
                trap = 2;
        }
        if (count > 0)
            score += calcConsecutiveScore(trap, count, turn);
        count = 0;
        trap = 2;
    }
    return score;
}

int Ai::eval_diagonal(std::vector<int> map, bool opponentTurn, int player) {
    int count = 0;
    int trap = 2;
    int score = 0;
    int turn = (opponentTurn && player == Player::OPPONENT ? 1 : 0);

    for (int k = 0; k <= 2 * (_size - 1); k++) {
        int start = getMax(0, k - _size + 1);
        int end = getMin(_size - 1, k);
        for (int i = start; i <= end; ++i) {
            int j = k - i;
            if (map[i * _size + j] == player) {
                count++;
            } else if (map[i * _size + j] == 0) {
                if(count > 0) {
                    score += calcConsecutiveScore(trap - 1, count, turn);
                    count = 0;
                    trap = 1;
                } else
                    trap = 1;
            } else if (count > 0) {
                score += calcConsecutiveScore(trap, count, turn);
                count = 0;
                trap = 2;
            } else
                trap = 2;
        }
        if (count > 0)
            score += calcConsecutiveScore(trap, count, turn);
        count = 0;
        trap = 2;
    }
    for (int k = 1 - _size; k < _size; k++) {
        int start = getMax(0, k);
        int end = getMin(_size + k - 1, _size-1);
        for (int i = start; i <= end; ++i) {
            int j = i - k;
            if (map[i * _size + j] == player) {
                count++;
            } else if(map[i * _size + j] == 0) {
                if (count > 0) {
                    score += calcConsecutiveScore(trap - 1, count, turn);
                    count = 0;
                    trap = 1;
                } else
                    trap = 1;
            } else if (count > 0) {
                score += calcConsecutiveScore(trap, count, turn);
                count = 0;
                trap = 2;
            } else
                trap = 2;
        }
        if (count > 0)
            score += calcConsecutiveScore(trap, count, turn);
        count = 0;
        trap = 2;
    }
    return score;
}

int Ai::getMax (int a, int b)
{
    return (a > b ? a : b);
}

int Ai::getMin (int a, int b)
{
    return (a < b ? a : b);
}
