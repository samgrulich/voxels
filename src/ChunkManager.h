#pragma once

#include <unordered_map>
#include <queue>
#include <mutex>
#include <memory>

#include "Chunk.h"
#include "Shader.h"
#include "VAO.h"
#include "VBO.h"



// class World {
//     private:
//         int viewDistance_ = 2;
//         int fullViewDistance_;
//         // std::mutex mtxChunks_;
//         std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> chunks_;
//         std::mutex mtxToGenerate_;
//         std::queue<std::weak_ptr<Chunk>> toGenerate_;
//         std::mutex mtxToMesh_;
//         std::queue<std::weak_ptr<Chunk>> toMesh_;
//         std::mutex mtxToUnload_;
//         std::unordered_map<glm::ivec3, bool> toUnload_;
//         std::mutex mtxActive_;
//         std::unordered_map<glm::ivec3, std::weak_ptr<Chunk>> active_;
//         std::weak_ptr<Chunk> lastChunk_;
//         glm::ivec3 start_;
//         glm::ivec3 end_;
//         glm::ivec3 playerPos_;
//     private:
//         bool isPositionOutside(glm::ivec3 position);
//
//         glm::ivec3 getChunkPosition(glm::ivec3 position);
//         glm::ivec3 getBlockPosition(glm::ivec3 position);
//
//         size_t getChunkIndex(glm::ivec3 position);
//         size_t getBlockIndex(glm::ivec3 position);
//
//         // adds positions of chunks in given X plane
//         void addChunksXPlane(std::vector<glm::ivec3>& chunks, glm::ivec3 origin, int offset);
//         // adds positions of chunks in given Y plane
//         void addChunksYPlane(std::vector<glm::ivec3>& chunks, glm::ivec3 origin, int offset);
//         // adds positions of chunks in given Z plane
//         void addChunksZPlane(std::vector<glm::ivec3>& chunks, glm::ivec3 origin, int offset);
//
//         void loadChunks(std::vector<glm::ivec3>& chunkPositions);
//         void unloadChunks(std::vector<glm::ivec3>& chunkPositions);
//     public:
//         World(glm::ivec3 playerPos);
//         ~World();
//
//         uint8_t getBlock(glm::ivec3 position);
//         void generateChunks();
//         void meshChunks();
//
//         void updateRegion(glm::ivec3 camPos);
//         void draw();
// };

class ChunkManager {
    private:
        std::mutex mtxToGenerate_;
        std::queue<std::shared_ptr<Chunk>> toGenerate_;
        std::mutex mtxToMesh_;
        std::queue<std::shared_ptr<Chunk>> toMesh_;
        glm::ivec3 renderAreaStart_;
        glm::ivec3 renderAreaEnd_;
        glm::vec3 lastCameraPostion_;
        glm::vec3 lastCameraForward_;
    private:
        // loads chunk with extra checks
        void loadChunks  (std::vector<glm::ivec3>& chunkPositions);
        // unloads chunk with extra checks
        void unloadChunks(std::vector<glm::ivec3>& chunkPositions);
    public:
        ChunkManager();

        // takes chunks from toGenerate queue -> genereates chunks -> 
        // adds them to the mesh queue 
        void generateChunks();
        // generate meshes of "size" chunks waiting in queue for mesh generation
        void meshChunks(unsigned int size);
        // draw chunks 
        void drawChunks(glm::vec3 cameraPosition, glm::vec3 cameraForward, ShaderProgram& shaderProgram);
};
