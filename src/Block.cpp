#include "../include/Block.h"

Block::Block(const std::string &name)
        : name(name) {}

std::string Block::getName() const {
    return name;
}
