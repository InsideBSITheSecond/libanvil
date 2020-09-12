#include "../include/ChunkRegistry.h"

ChunkRegistry::ChunkRegistry(const std::string pathToRegionFolder)
        : m_PathToRegionFolder(pathToRegionFolder) {}

std::shared_ptr<Chunk> ChunkRegistry::getChunkByBlockCoord(int32_t x, int32_t z) {
    return getChunk(x / 16, z / 16);
}

std::shared_ptr<Chunk> ChunkRegistry::getChunk(int32_t x, int32_t z) {
    auto it = loadedChunks.find({x, z});
    if (it != loadedChunks.end()) {
        return it->second;
    }

    int32_t mcaFileX = x / 32;
    int32_t mcaFileZ = z / 32;
    std::stringstream mcaFileName;
    mcaFileName << m_PathToRegionFolder << "/r." << mcaFileX << "." << mcaFileZ << ".mca";
    region_file_reader reader(mcaFileName.str());
    reader.read(true);

    auto chunk = reader.getChunkAt(x - mcaFileX * 32, z - mcaFileZ * 32);
    loadedChunks[{x, z}] = chunk;

    return chunk;
}

std::optional<Block> ChunkRegistry::getBlock(const std::array<int32_t, 3> &coord) const {
    auto it = loadedChunks.find({coord[0] / 16, coord[2] / 16});

    if (it == loadedChunks.end()) {
        return {};
    }

    return it->second->getBlock(coord);
}
