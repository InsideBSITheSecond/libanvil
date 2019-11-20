#pragma once

#include <map>
#include "Block.h"
#include "ChunkRegistry.h"
#include "Chunk.h"
#include "region_file_reader.h"

/*!
 * The block registry contains a small subset of relevant blocks, but one should not expect it to be complete.
 * Only serves fast access
 */
class ChunkRegistry {

public:
    ChunkRegistry() = default;

    ChunkRegistry(ChunkRegistry const&) = delete;

    ChunkRegistry(const ChunkRegistry&&) = delete;

    std::shared_ptr<Chunk> getChunk(int32_t x, int32_t z) {
        auto it = loadedChunks.find({ x, z });
        if (it != loadedChunks.end()) {
            return it->second;
        }

        int32_t mcaFileX = x / 32;
        int32_t mcaFileZ = z / 32;
        std::stringstream mcaFileName;
        mcaFileName << "r." << mcaFileX << "." << mcaFileZ << ".mca";
        region_file_reader reader(mcaFileName.str());
        reader.read(true);

        auto chunk = reader.getChunkAt(x - mcaFileX * 32, z - mcaFileZ * 32);

        return chunk;
    }

private:
    std::map<std::array<int32_t, 2>, std::shared_ptr<Chunk>> loadedChunks;

};
