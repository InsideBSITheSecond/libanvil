#pragma once

#include <string>
#include <array>

class Block {

public:
    Block(std::string const& name);

    std::string const& getName() const;

    void setPos(int x, int y, int z) {
        pos = {x, y, z};
    }

    std::array<int, 3> getPos() const {
        return pos;
    }

private:
    std::string name;

    std::array<int, 3> pos = {0, 0, 0};  // XYZ
};

