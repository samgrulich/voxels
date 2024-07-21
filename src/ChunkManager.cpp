#include "ChunkManager.h"
#include "Shader.h"
#include "WorldConstants.h"
#include <memory>


ChunkManager::ChunkManager() {
    toGenerate_ = std::queue<std::shared_ptr<Chunk>>();
    toMesh_     = std::queue<std::shared_ptr<Chunk>>();
    lastCameraPostion_ = {0, 0, 0};
    lastCameraForward_ = {0, 0, -1};
    renderAreaStart_ = glm::vec3(-World::VIEW_DISTANCE);
    renderAreaEnd_   = glm::vec3( World::VIEW_DISTANCE);
}

void ChunkManager::loadChunks(std::vector<glm::ivec3>& chunkPositions) {
    for (glm::ivec3 chunkPos : chunkPositions) {
        if (chunks.contains(chunkPos))
            continue;
        std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(chunkPos);
        chunks[chunkPos] = chunk;
        // load chunk
        toGenerate_.push(chunk);
    }
}

void ChunkManager::unloadChunks(std::vector<glm::ivec3>& chunkPositions) {
    for (glm::ivec3 chunkPos : chunkPositions) {
        if (!chunks.contains(chunkPos))
            continue;
        std::shared_ptr<Chunk> chunk = chunks[chunkPos];
        // unload chunk
        chunk->isActive_  = false;
        chunk->isVisible_ = false;
        chunk->toUpload_  = false;
    }
}

void ChunkManager::generateChunks() {
    while (!toGenerate_.empty()) {
        std::shared_ptr<Chunk> chunk = toGenerate_.front();
        chunk->generate();
        toMesh_.push(chunk);
        toGenerate_.pop();
    }
}

void ChunkManager::meshChunks(unsigned int size) {
    for (int i = 0; i < size && !toMesh_.empty(); i++) {
        std::shared_ptr<Chunk> chunk = toMesh_.front();
        chunk->mesh();
        toMesh_.pop();
    }
}

void ChunkManager::drawChunks(glm::vec3 cameraPosition, glm::vec3 cameraForward, ShaderProgram& shaderProgram) {
    // todo
    for (auto& [_, chunk] : chunks) {
        if (chunk->isVisible_) {
            chunk->draw(shaderProgram);
        }
    }
}
