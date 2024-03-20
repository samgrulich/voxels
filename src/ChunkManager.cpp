#include "ChunkManager.h"
#include "Shader.h"
#include "math.h"
#include <mutex>


ChunkManager::ChunkManager() 
    : seed_(World::DEFAULT_SEED)
{
    renderAreaStart_ = glm::ivec3(-World::VIEW_DISTANCE);
    renderAreaStop_  = glm::ivec3(World::VIEW_DISTANCE);
    renderAreaStart_.y = -1;
    renderAreaStop_.y = 2;
}

void ChunkManager::loadChunks(std::vector<glm::ivec3>& chunkPositions) {
    std::lock_guard<std::mutex> g(mtxToGenerate_);
    for (auto& chunkPos : chunkPositions) {
        if (!chunks_.contains(chunkPos)) {
            std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(Chunk(chunkPos));
            chunks_[chunkPos] = chunk;
            toGenerate_.push(chunk->metadata_);
            chunk->metadata_->setToGenerate();
        }
    }
}

void ChunkManager::unloadChunks(std::vector<glm::ivec3>& chunkPositions) {
    for (auto& chunkPos : chunkPositions) {
        if (chunks_.contains(chunkPos)) {
            chunks_[chunkPos]->metadata_->setToUnload();
            chunks_.erase(chunkPos);
        }
    }
}

void ChunkManager::generateChunks() {
    std::vector<std::shared_ptr<ChunkMetadata>> toMesh;
    toMesh.reserve(toGenerate_.size());
    mtxToGenerate_.lock();
    while (!toGenerate_.empty()) {
        std::shared_ptr<ChunkMetadata> meta = toGenerate_.front();
        if (meta->toGenerate()) {
            meta->getShared()->generate(seed_);
            meta->setToMesh();
        }
        toGenerate_.pop();
    }
    mtxToGenerate_.unlock();
    std::lock_guard<std::mutex> g2(mtxToMesh_);
    for (auto& meta : toMesh) {
        toMesh.push_back(meta);
    }
}

void ChunkManager::meshChunks(unsigned int size) {
    int i = 0;
    std::lock_guard<std::mutex> g(mtxToMesh_);
    while (!toMesh_.empty() && i < size) {
        std::shared_ptr<ChunkMetadata> meta = toMesh_.front();
        if (meta->toMesh()) {
            meta->getShared()->generate(seed_);
            meta->setToMesh();
        }
        toMesh_.pop();
        i++;
    }
}

void ChunkManager::drawChunks(glm::vec3 camPos, glm::vec3 camForward, ShaderProgram shaderProgram) {
    if (lastCameraPosition_ != camPos || lastCameraForward_ != camForward) {
        // shift camera a chunk backwards 
        camPos.x = (-1 * camForward.x) * World::CHUNK_SIZE;
        camPos.z = (-1 * camForward.z) * World::CHUNK_SIZE;

        // frustum culling
        float rotateAngle = 70.0f;
        // 2nd power of minimal distance to chunk to consider it for drawing
        float minRenderDistance = World::CHUNK_SIZE_POW2; 

        std::vector<glm::ivec3> toLoad;
        std::vector<glm::ivec3> toUnload;
        glm::vec3 camChunkPos = glm::mod(camPos, glm::vec3(World::CHUNK_SIZE));
        glm::ivec3 newRenderAreaStart = camChunkPos - glm::vec3(World::VIEW_DISTANCE);
        glm::ivec3 newRenderAreaStop  = camChunkPos + glm::vec3(World::VIEW_DISTANCE);
        glm::ivec3 renderAreaStart = glm::min(renderAreaStart_, newRenderAreaStart);
        glm::ivec3 renderAreaStop  = glm::max(renderAreaStop_, newRenderAreaStop);
        renderAreaStart_ = newRenderAreaStart;
        renderAreaStop_  = newRenderAreaStop;
        
        for (int y = renderAreaStart.y; y < renderAreaStop.y; y++) {
        for (int z = renderAreaStart.z; z < renderAreaStop.z; z++) {
        for (int x = renderAreaStart.x; x < renderAreaStop.x; x++) {
            glm::vec3 chunkPosition(x*World::CHUNK_SIZE, y*World::CHUNK_SIZE, z*World::CHUNK_SIZE);
            glm::vec3 chunkCenter = chunkPosition + glm::vec3(World::CHUNK_SIZE_HALF);
            glm::vec3 relativeChunkCenter = chunkCenter - camPos;
            float distanceToChunk = glm::dot(relativeChunkCenter, relativeChunkCenter);

            // todo: add frustum culling
            if (!chunks_.contains({x, y, z})) {
                toLoad.push_back({x, y, z});
            } else if (distanceToChunk <= minRenderDistance) {
                chunks_[{x, y, z}]->draw(shaderProgram, true);
                chunks_[{x, y, z}]->visible_ = true;
            } else {
                toUnload.push_back({x, y, z});
                chunks_[{x, y, z}]->visible_ = false;
            }
        }}}

        loadChunks(toLoad);
        unloadChunks(toUnload);

        lastCameraPosition_ = camPos;
        lastCameraForward_  = camForward;
    } else {
        for (int y = renderAreaStart_.y; y < renderAreaStop_.y; y++) {
        for (int z = renderAreaStart_.z; z < renderAreaStop_.z; z++) {
        for (int x = renderAreaStart_.x; x < renderAreaStop_.x; x++) {
            if (chunks_[{x, y, z}]->visible_)
                chunks_[{x, y, z}]->draw(shaderProgram, true);
        }}}
    }

    // todo: render transparent
}
