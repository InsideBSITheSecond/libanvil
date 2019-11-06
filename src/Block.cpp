#include "../include/Block.h"

Block::Block(const std::string& name)
    : name(name) {}

std::string const& Block::getName() const {
    return name;
}
