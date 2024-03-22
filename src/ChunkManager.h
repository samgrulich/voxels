#pragma once

#include <unordered_map>
#include <queue>
#include <mutex>
#include <memory>

#include "Chunk.h"
#include "Shader.h"



class ChunkManager {
    private:
        float seed_;

        std::mutex mtxToGenerate_;
        std::queue<std::shared_ptr<ChunkMetadata>> toGenerate_;
        std::mutex mtxToMesh_;
        std::queue<std::shared_ptr<ChunkMetadata>> toMesh_;

        glm::ivec3 renderAreaStart_;
        glm::ivec3 renderAreaStop_;
        glm::vec3 lastCameraPosition_;
        glm::vec3 lastCameraForward_;
    private:
        // load thread safely chunks in the list
        void loadChunks(std::vector<glm::ivec3>& chunkPositions);
        // unload safely chunks in the list
        void unloadChunks(std::vector<glm::ivec3>& chunkPositions);
    public:
        ChunkManager();

        // generate terrain
        void generateChunks();
        // generate chunk meshes
        void meshChunks(unsigned int size);
        // draw chunks
        void drawChunks(glm::vec3 cameraPosition, glm::vec3 cameraForward, ShaderProgram& shaderProgram);
};
