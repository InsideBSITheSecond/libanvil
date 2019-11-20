#pragma once

#include <string>
#include <array>

class Block {

public:

    Block() = default;

    Block(std::string const& name);

    Block(std::string const& name, std::array<int32_t, 3> const& pos)
        : m_Name(name), m_Pos(pos) {
    }

    [[nodiscard]] std::string const& getName() const;

    void setPos(int32_t x, int32_t y, int32_t z) {
        m_Pos = {x, y, z};
    }

    [[nodiscard]] std::array<int32_t, 3> getPos() const {
        return m_Pos;
    }

private:
    std::string m_Name;

    std::array<int32_t, 3> m_Pos = {0, 0, 0};  // XYZ
};

