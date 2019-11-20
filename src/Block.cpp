#include "../include/Block.h"

Block::Block(const std::string& name)
    : m_Name(name) {}

std::string const& Block::getName() const {
    return m_Name;
}
