#pragma once

#include <string>
#include <list>

enum class CommandType
{
    A_COMMAND,
    C_COMMAND,
    L_COMMAND,
};

enum class Status
{
    PARSER_WORKING,
    PARSER_ERR,
    PARSER_EOF,
};

class Parser
{
public:
    explicit Parser(std::string &fileName);
    bool hasMoreCommands(void);
    void advance(void);

    CommandType commandType(void) { return m_command_type; };
    std::string symbol(void) { return m_symbol; };
    std::string dest(void) { return m_dest; };
    std::string comp(void) { return m_comp; };
    std::string jump(void) { return m_jump; };

    Status m_status;

 private:
    std::list<std::string> m_commands;
    CommandType m_command_type;
    std::string m_symbol;
    std::string m_dest;
    std::string m_comp;
    std::string m_jump;
};