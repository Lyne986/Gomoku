/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** Core
*/

#include "Core.hpp"

Core::Core()
{
    this->_protocol = std::make_shared<Protocol>();
    this->_ai = std::make_shared<Ai>(this->_protocol);
}

Core::~Core()
{
}

void Core::init()
{
}

void Core::run()
{
    while (true) {
        std::string line = this->_protocol->readTerminal();
        this->FindCommand(line);
    }
    
}

void Core::FindCommand(std::string line) 
{
    std::regex start("START ([0-9]+)");
    std::regex turn("TURN ([0-9]+),([0-9]+)");
    std::regex begin("BEGIN");
    std::regex board("BOARD");
    std::regex end("END");

    std::smatch match;

    if (std::regex_match(line, match, start)) {
        if (this->_protocol->Start(std::stoi(match[1])))
            this->_ai->createMap(std::stoi(match[1]));
    } else if (std::regex_match(line, match, turn)) {
        this->_ai->turn(std::stoi(match[1]), std::stoi(match[2]));
    } else if (std::regex_match(line, match, begin)) {
        this->_ai->begin();
    } else if (std::regex_match(line, match, board)) {
        this->_ai->board();
    } else if (std::regex_match(line, match, end)) {
        this->_ai->end();
    }
}
