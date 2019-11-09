#include "../include/Block.h"

Block::Block(const std::string& name)
    : name(name) {}

Block::Block(std::string const& name, int x, int y, int z)
    : name(name), pos({x, y, z}) {}

std::string const& Block::getName() const {
    return name;
}
