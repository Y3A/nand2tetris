#include "parser.h"
#include <iostream>

int main(int argc, char *argv[])
{
    std::string fileName = argv[1];
    Parser ps(fileName);

    ps.advance();
    std::cout << "Status: " << static_cast<int>(ps.m_status) << std::endl;
    std::cout << "Symbol: " << ps.symbol() << std::endl;
    ps.advance();
    std::cout << "Status: " << static_cast<int>(ps.m_status) << std::endl;
    std::cout << "Symbol: " << ps.symbol() << std::endl;
    ps.advance();
    std::cout << "Status: " << static_cast<int>(ps.m_status) << std::endl;
    std::cout << "Dest: " << ps.dest() << std::endl;
    std::cout << "Comp: " << ps.comp() << std::endl;
    std::cout << "Jump: " << ps.jump() << std::endl;
    ps.advance();
    std::cout << "Status: " << static_cast<int>(ps.m_status) << std::endl;
    std::cout << "Dest: " << ps.dest() << std::endl;
    std::cout << "Comp: " << ps.comp() << std::endl;
    std::cout << "Jump: " << ps.jump() << std::endl;

    std::cout << "Has More Commands: " << ps.hasMoreCommands() << std::endl;
    ps.advance();
    std::cout << "Status: " << static_cast<int>(ps.m_status) << std::endl;

    return 0;
}