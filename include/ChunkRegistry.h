#pragma once

#include <map>
#include "Block.h"
#include "ChunkRegistry.h"
#include "Chunk.h"
#include "region_file_reader.h"

/*!
 * The chunk registry contains a small subset of relevant blocks, but one should not expect it to be complete.
 * Only serves fast access
 */
class ChunkRegistry {

public:
    ChunkRegistry(std::string const pathToRegionFolder);

    ChunkRegistry(ChunkRegistry const &) = delete;

    ChunkRegistry(const ChunkRegistry &&) = delete;

    std::shared_ptr<Chunk> getChunkByBlockCoord(int32_t x, int32_t z);

    std::shared_ptr<Chunk> getChunk(int32_t x, int32_t z);

    [[nodiscard]]  std::optional<Block> getBlock(std::array<int32_t, 3> const &coord) const;

private:
    std::string m_PathToRegionFolder;

    std::map<std::array<int32_t, 2>, std::shared_ptr<Chunk>> loadedChunks;
};
