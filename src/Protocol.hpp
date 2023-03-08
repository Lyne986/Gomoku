/*
** EPITECH PROJECT, 2022
** B-AIA-500-MPL-5-1-gomoku-diogo.faria-martins
** File description:
** Protocol
*/

#ifndef PROTOCOL_HPP_
    #define PROTOCOL_HPP_

    #include <iostream>
    #include <vector>

class Protocol {
    public:
        std::string readTerminal();
        bool Start(int size);
        bool Begin();
        Protocol();
        ~Protocol();

    protected:
    private:
        int _size;
};

#endif /* !PROTOCOL_HPP_ */
