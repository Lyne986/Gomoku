/*
** EPITECH PROJECT, 2022
** Untitled (Workspace)
** File description:
** Core
*/

#ifndef CORE_HPP_
#define CORE_HPP_

#include <memory>
#include <regex>
#include "Protocol.hpp"
#include "Ai.hpp"

class Core {
    public:
        Core();
        ~Core();
        void init();
        void run();
        void FindCommand(std::string line);

    protected:
    private:
        std::shared_ptr<Protocol> _protocol;
        std::shared_ptr<Ai> _ai;
};

#endif /* !CORE_HPP_ */
