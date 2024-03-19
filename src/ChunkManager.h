#pragma once

#include <unordered_map>
#include <queue>
#include <mutex>
#include <memory>

#include "Chunk.h"



class ChunkManager {
    private:
        std::mutex mtxToGenerate_;
        std::queue<ChunkMetadata> toGenerate_;
        std::mutex mtxToMesh_;
        std::queue<ChunkMetadata> toMesh_;
        std::mutex mtxActive_;
        std::unordered_map<glm::ivec3, ChunkMetadata> active_;
        glm::vec3 lastCameraPosition;
        glm::vec3 lastCameraForward;
    private:
        void loadChunks(std::vector<glm::ivec3>& chunkPositions);
        void unloadChunks(std::vector<glm::ivec3>& chunkPositions);
    public:
        ChunkManager();
        ~ChunkManager();

        void generateChunks();
        void renderChunks();
};
