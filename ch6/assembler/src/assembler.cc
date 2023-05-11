#include <bitset>
#include <fstream>
#include <iostream>

#include "code.h"
#include "parser.h"
#include "symbol_table.h"

std::string dec2bin(int dec)
{
    return std::bitset<15>(dec).to_string();
}

bool is_number(const std::string &s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

int main(int argc, char **argv)
{
    int             cur_insr = 0;
    SymTable        table;
    int             cur_ram_writable = 16;
    std::string     outbuf;
    std::ofstream   outfile("out.hack");

    if (argc < 2) {
        printf("[*] Usage: %s <asm file>", argv[0]);
        return 0;
    }

    Parser parser(argv[1]);

    // first pass, build symtable
    while (parser.hasMoreCommands() && parser.status == Parser::Status::PARSER_WORKING) {
        parser.advance();
        if (parser.commandType() == Parser::CommandType::L_COMMAND)
            table.addEntry(parser.symbol(), cur_insr);
        else
            cur_insr++;
    }

    // reset
    parser = Parser(argv[1]);

    // second pass, write output
    while (parser.hasMoreCommands() && parser.status == Parser::Status::PARSER_WORKING) {
        parser.advance();
        if (parser.commandType() == Parser::CommandType::A_COMMAND) {
            // if direct number, just write
            if (is_number(parser.symbol())) {
                outbuf = outbuf + "0" + dec2bin(std::stoi(parser.symbol())) + "\n";
                continue;
            }
            
            // actual symbol
            if (table.contains(parser.symbol()))
                outbuf = outbuf + "0" + dec2bin(table.getAddress(parser.symbol())) + "\n";
            else {
                table.addEntry(parser.symbol(), cur_ram_writable);
                outbuf = outbuf + "0" + dec2bin(cur_ram_writable) + "\n";
                cur_ram_writable++;
            }
        }
        else if (parser.commandType() == Parser::CommandType::C_COMMAND) {
            std::string compbin = Code::comp(parser.comp());
            std::string destbin = Code::dest(parser.dest());
            std::string jumpbin = Code::jump(parser.jump());

            outbuf = outbuf + "111" + compbin + destbin + jumpbin + "\n";
        }
    }

    if (outbuf.length())
        // strip trailing newline
        outbuf.erase(outbuf.length() - 1);
    
    outfile << outbuf;
    outfile.close();

    std::cout << "done" << std::endl;
    return 0;
}