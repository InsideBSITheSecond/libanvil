#pragma once

#include <string>

class Block {

public:
    Block(std::string const& name);

    std::string getName() const;

private:
    std::string name;
};

