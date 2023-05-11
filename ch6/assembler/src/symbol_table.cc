#include "symbol_table.h"

void SymTable::addEntry(std::string symbol, int addr)
{
    m_table.insert({symbol, addr});
    return;
}

bool SymTable::contains(const std::string &symbol)
{
    return static_cast<bool>(m_table.count(symbol));
}

int SymTable::getAddress(const std::string &symbol)
{
    return m_table[symbol];
}