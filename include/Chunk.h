#pragma once

#include <map>
#include "Block.h"

class Chunk {
public:
    Chunk(std::array<std::int32_t, 2> chunkPos)
        : m_ChunkPos(chunkPos) {
    }

    [[nodiscard]] std::array<int32_t, 2> const& getPos() const {
        return m_ChunkPos;
    }

    void addBlock(Block& block) {
        auto val = block.getPos();
        m_Chunks[val] = block;
    }

    [[nodiscard]] std::map<std::array<int32_t, 3>, Block> const& getBlocks() const {
        return m_Chunks;
    }

private:
    std::array<int32_t, 2> m_ChunkPos;
    std::map<std::array<int32_t, 3>, Block> m_Chunks;
};

