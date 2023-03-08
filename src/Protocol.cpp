/*
** EPITECH PROJECT, 2022
** B-AIA-500-MPL-5-1-gomoku-diogo.faria-martins
** File description:
** Protocol
*/

#include "Protocol.hpp"
#include <iostream>

Protocol::Protocol()
{
}

Protocol::~Protocol()
{
}

std::string Protocol::readTerminal()
{
    std::string line;
    std::getline(std::cin, line);
    return line;
}

bool Protocol::Start(int size) {

    if (size <= 0) {
        std::cout << "ERROR message - unsupported size or other error" << std::endl;
        return false;
    }
    this->_size = size;
    std::cout << "OK - everything is good" << std::endl;
    return true;

}

bool Protocol::Begin() {
        std::cout << "BEGIN" << std::endl;
        return true;
        // initialize int array
}
