#include <fstream>
#include <algorithm>

#include "parser.h"

Parser::Parser(std::string &fileName)
{
    std::ifstream file_stream(fileName);
    std::string   cur_line;

    if (file_stream.fail()) {
        m_status = Status::PARSER_ERR;
        return;
    }
    
    while (std::getline(file_stream, cur_line))
        m_commands.push_back(cur_line);

    m_status = Status::PARSER_WORKING;
    return;
}

bool Parser::hasMoreCommands(void)
{
    return !m_commands.empty();
}

void Parser::advance(void)
{
    std::string cur_line;
    size_t      startpos = 0, endpos;

    m_symbol = m_dest = m_jump = m_comp = "";

    for (;;) {
        // keep advancing if cur_line is empty str or comment
        if (m_commands.empty()) {
            m_status = Status::PARSER_EOF;
            return;
        }
        
        cur_line = m_commands.front();
        m_commands.pop_front();

        if (cur_line.size() != 0 && cur_line.substr(0, 2) != "//")
            break;
    }

    // remove all white spaces
    cur_line.erase(std::remove_if(cur_line.begin(), cur_line.end(), isspace), cur_line.end());

    // process cur_line
    switch (cur_line[0])
    {
        case '(':
            // Labels
            m_command_type = CommandType::L_COMMAND;
            endpos = cur_line.find(')');
            if (endpos == std::string::npos) {
                // malformed syntax
                m_status = Status::PARSER_ERR;
                return;
            }
            m_symbol = cur_line.substr(1, endpos - 1);
            return;
        
        case '@':
            // A-Instructions
            m_command_type = CommandType::A_COMMAND;
            m_symbol = cur_line.substr(1);
            return;

        default:
            break;
    }

    // C-Instruction
    m_command_type = CommandType::C_COMMAND;

    endpos = cur_line.find('=');
    if (endpos != std::string::npos) {
        // dest exists

        // extract dest
        m_dest = cur_line.substr(startpos, endpos - startpos);
        // increment pos to point after '='
        endpos++;
    }
    else
        endpos = 0;

    if (endpos >= cur_line.size()) {
        // malformed syntax
        m_status = Status::PARSER_ERR;
        return;
    }

    startpos = endpos;
    endpos = cur_line.find(';');
    // extract comp
    m_comp = cur_line.substr(startpos, endpos == std::string::npos ? endpos : endpos - startpos);

    if (endpos != std::string::npos) {
        // jump exists
        startpos = endpos + 1;
        if (startpos >= cur_line.size()) {
            // malformed syntax
            m_status = Status::PARSER_ERR;
            return;
        }

        // extract jump
        m_jump = cur_line.substr(startpos);
    }

    return;
}
