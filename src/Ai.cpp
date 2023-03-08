/*
** EPITECH PROJECT, 2022
** gomoku
** File description:
** Ai
*/

#include "Ai.hpp"

Ai::Ai(std::shared_ptr<Protocol> protocol)
{
    _size = 0;
    _turn = 0;
    isinGame = false;
    _protocol = protocol;
    std::srand(time(NULL));
}

Ai::~Ai(){}

void Ai::createMap(int size)
{
    _map.clear();
    for (int i = 0; i < size * size; i++) {
        _map.push_back(0);
    }
    _size = size;
}

std::vector<int> Ai::getMapCopy(std::vector<int> map, int x, int y, int player)
{
    std::vector<int> copy(map);

    copy[x + y * _size] = player;
    return copy;
}

void Ai::updateMap(int x, int y, int player)
{
    _map[(y * _size) + x] = player;
}

void Ai::turn(int x, int y)
{
    if (x != -1 && y != -1) {
        updateMap(x, y, Player::OPPONENT);
        _turn++;
    }
    node_t move = get_next_move(3);
    updateMap(move.x, move.y, Player::PLAYER);
    _turn++;
    std::cout << move.x << "," << move.y << std::endl;
}

void Ai::begin()
{
    int posX = 9;
    int posY = 9;

    updateMap(posX, posY, 1);
    _turn++;
    std::cout << posX << "," << posY << std::endl;
}

void Ai::beginAfterBoard()
{
    node_t move = get_next_move(3);

    updateMap(move.x, move.y, Player::PLAYER);
    _turn++;
    std::cout << move.x << "," << move.y << std::endl;
}

bool Ai::check_win(int player)
{
    for (int j = 0; j <_size - 4 ; j++ ){
        for (int i = 0; i < _size; i++){
            if (_map[i * _size + j] == player && _map[i * _size + j + 1] == player && _map[i * _size + j + 2] == player && _map[i * _size + j + 3] == player && _map[i * _size + j + 4] == player)
                return true;
            if (_map[i * _size + j] == player && _map[(i + 1) * _size + j] == player && _map[(i + 2) * _size + j] == player && _map[(i + 3) * _size + j] == player && _map[(i + 4) * _size + j] == player)
                return true;
        }
    }

    for (int i = 4; i < _size; i++){
        for (int j = 0; j < _size; j++){
            if (j < _size - 4 && _map[i * _size + j] == player && _map[(i - 1) * _size + j + 1] == player && _map[(i - 2) * _size + j + 2] == player && _map[(i - 3) * _size + j + 3] == player && _map[(i - 4) * _size + j + 4] == player)
                return true;
            if (j >= 4 && _map[i * _size + j] == player && _map[(i - 1) * _size + j - 1] == player && _map[(i - 2) * _size + j - 2] == player && _map[(i - 3) * _size + j - 3] == player && _map[(i - 4) * _size + j - 4] == player)
                return true;
        }
    }
    return false;
}


void Ai::print_map(std::vector<int> map)
{
    for (int i = 0; i < _size; i++) {
        for (int j = 0; j < _size; j++) {
            std::cout << map[i * _size + j] << " ";
        }
        std::cout << std::endl;
    }
}

void Ai::board()
{
    if (this->isinGame)
        return;
    while (1)
    {
        std::string line = this->_protocol->readTerminal();
        std::regex command("([0-9]+),([0-9]+),([1-3]+)");
        std::regex done("DONE");
        std::smatch match;

        if (std::regex_match(line, match, command))
        {
            int x = std::stoi(match[1]);
            int y = std::stoi(match[2]);
            int player = std::stoi(match[3]);
            this->updateMap(x, y, player);
        }
        else if (std::regex_match(line, match, done))
        {
            this->isinGame = true;
            this->beginAfterBoard();
            break;
        }
    }
}

void Ai::end()
{
    exit(0);
}

void Ai::restart()
{
    _map.clear();
    _turn = 0;
    std::cout << "OK" << std::endl;
}

int Ai::evaluate(std::vector<int> map, bool opponentTurn, int player)
{
    int result = 0;

    result += eval_horizontal(map, opponentTurn, player);
    result += eval_vertical(map, opponentTurn, player);
    result += eval_diagonal(map, opponentTurn, player);
    return result;
}

std::vector<std::tuple<int, int>> Ai::getPossibleMoves(std::vector<int> map)
{
    std::vector<std::tuple<int, int>> possibleMoves = std::vector<std::tuple<int, int>>();
    int size_limit = _size - 1;

    for (int i = 0; i < _size; i++) {
        for (int j = 0; j < _size; j++) {
            if (map[i * _size + j] != 0) {
                if (i != 0) {
                    if (map[(i - 1) * _size + j] == 0) { // en haut
                        if (std::find(possibleMoves.begin(), possibleMoves.end(), std::make_tuple(j, i - 1)) == possibleMoves.end())
                            possibleMoves.push_back(std::make_tuple(j, i - 1));
                    }
                }
                if (i < size_limit)
                    if (map[(i + 1) * _size + j] == 0) { // en bas
                        if (std::find(possibleMoves.begin(), possibleMoves.end(), std::make_tuple(j, i + 1)) == possibleMoves.end())
                            possibleMoves.push_back(std::make_tuple(j, i + 1));
                    }
                if (j < size_limit)
                    if (map[i * _size + (j + 1)] == 0) { // a droite
                        if (std::find(possibleMoves.begin(), possibleMoves.end(), std::make_tuple(j + 1, i)) == possibleMoves.end())
                            possibleMoves.push_back(std::make_tuple(j + 1, i));
                    }
                if (j != 0)
                    if (map[i * _size + (j - 1)] == 0) { // a gauche
                        if (std::find(possibleMoves.begin(), possibleMoves.end(), std::make_tuple(j - 1, i)) == possibleMoves.end())
                            possibleMoves.push_back(std::make_tuple(j - 1, i));
                    }
                if (i != 0 && j < size_limit)
                    if (map[(i - 1) * _size + (j + 1)] == 0) { // en haut a droite
                        if (std::find(possibleMoves.begin(), possibleMoves.end(), std::make_tuple(j + 1, i - 1)) == possibleMoves.end())
                            possibleMoves.push_back(std::make_tuple(j + 1, i - 1));
                    }
                if (i != 0 && j != 0)
                    if (map[(i - 1) * _size + (j - 1)] == 0) { // en haut a gauche
                        if (std::find(possibleMoves.begin(), possibleMoves.end(), std::make_tuple(j - 1, i - 1)) == possibleMoves.end())
                            possibleMoves.push_back(std::make_tuple(j - 1, i - 1));
                    }
                if (i < size_limit && j < size_limit)
                    if (map[(i + 1) * _size + (j + 1)] == 0) { // en bas a droite
                        if (std::find(possibleMoves.begin(), possibleMoves.end(), std::make_tuple(j + 1, i + 1)) == possibleMoves.end())
                            possibleMoves.push_back(std::make_tuple(j + 1, i + 1));
                    }
                if (i < size_limit && j != 0)
                    if (map[(i + 1) * _size + (j - 1)] == 0) { // en bas a gauche
                        if (std::find(possibleMoves.begin(), possibleMoves.end(), std::make_tuple(j - 1, i + 1)) == possibleMoves.end())
                            possibleMoves.push_back(std::make_tuple(j - 1, i + 1));
                    }
            }
        }
    }
    return possibleMoves;
}


Ai::node_t Ai::easywin()
{
    std::vector<std::tuple<int, int>> possible_moves = getPossibleMoves(_map);
    node_t pos;

    for (auto move : possible_moves) {
        std::vector<int> newBoard = getMapCopy(_map, std::get<0>(move), std::get<1>(move), 1);
        int evaluate_score = evaluate(newBoard, false, Player::PLAYER);
        if (evaluate_score >= 100000000) {
            pos.x = std::get<0>(move);
            pos.y = std::get<1>(move);
            pos.score = 100000000;
            return pos;
        }
    }
    pos.x = -1;
    pos.y = -1;
    pos.score = -1;
    return pos;
}

Ai::node_t Ai::get_next_move(int depth)
{
    node_t pos = easywin();
    if (pos.x != -1 && pos.y != -1)
        return pos;
    pos = minimax(_map, depth, -1, 100000000, Player::PLAYER);

    return (pos);
}

Ai::node_t Ai::minimax(std::vector<int> board, int depth, int alpha, int beta, int player)
{
    node_t pos;
    if (depth == 0) {
        // int score = evaluate(board, player);
        int score = evaluate(board, false, (player == Player::PLAYER) ? Player::PLAYER : Player::OPPONENT);
        int score2 = evaluate(board, true, (player == Player::OPPONENT) ? Player::PLAYER : Player::OPPONENT);

        pos.score = score2 - score;
        return pos;
    }
    node_t best_pos;
    if (player == Player::PLAYER) {
        std::vector<std::tuple<int, int>> possible_moves = getPossibleMoves(board);
        node_t new_pos;
        for (std::tuple<int, int> move : possible_moves) {
            std::vector<int> newBoard = getMapCopy(board, std::get<0>(move), std::get<1>(move), 1);
            new_pos = minimax(newBoard, depth - 1, alpha, beta, Player::OPPONENT);
            if (new_pos.score > alpha) {
                alpha = new_pos.score;
            }
            if (new_pos.score >= beta) {
                return new_pos;
            }
            if (new_pos.score > best_pos.score) {
                best_pos.score = new_pos.score;
                best_pos.x = std::get<0>(move);
                best_pos.y = std::get<1>(move);
            }
        }
    } else {
        best_pos.score = 100000000;
        std::vector<std::tuple<int, int>> possible_moves = getPossibleMoves(board);
        best_pos.x = std::get<0>(possible_moves[0]);
        best_pos.y = std::get<1>(possible_moves[0]);
        node_t new_pos;
        for (std::tuple<int, int> move : possible_moves) {
            std::vector<int> newBoard = getMapCopy(board, std::get<0>(move), std::get<1>(move), 2);
            new_pos = minimax(newBoard, depth - 1, alpha, beta, Player::PLAYER);
            if (new_pos.score < beta) {
                beta = new_pos.score;
            }
            if (new_pos.score <= alpha) {
                return new_pos;
            }
            if (new_pos.score < best_pos.score) {
                best_pos.score = new_pos.score;
                best_pos.x = std::get<0>(move);
                best_pos.y = std::get<1>(move);
            }
        }
    }
    return best_pos;
}
