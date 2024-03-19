#pragma once 

#include <cstdint>
#include <functional>
#include <vector>
#include <memory>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL // Need this to use the glm hashes
#include "glm/gtx/hash.hpp" // Include hash maps for unordered_map
#include <GL/glew.h>
#include "VBO.h"
#include "VAO.h"
#include "WorldConstants.h"
#include "Block.h"



class ChunkManager; 
class Chunk;


class ChunkMetadata {
    private:
        bool toGenerate_;
        bool toMesh_;
        bool active_;
        glm::ivec3 position_;
        glm::ivec3 offset_;
        std::shared_ptr<Chunk> chunk_;
    public:
        ChunkMetadata();
        ChunkMetadata(Chunk& chunk);
        ChunkMetadata(std::weak_ptr<Chunk> chunk);
        ChunkMetadata(glm::ivec3 position);

        // set the chunk state to generate stage
        void setToGenerate();
        // set the chunk state to mesh stage
        void setToMesh();
        // set the chunk state to render stage
        void setToActive();
        // prepare the chunk for unloading (disable all other states)
        void setToUnload();

        // returns true if chunk is ready for generation
        bool isGeneratable();
        // returns true if chunk is ready for meshing
        bool isMeshable();
        // returns true if chunk is active
        bool isActive();

        // get weak poitner to the chunk
        std::weak_ptr<Chunk> getWeak();
        // get shared poitner to the chunk
        std::shared_ptr<Chunk> getShared();
        // get position
        glm::ivec3 position();
};


class Chunk {
    public:
        // offset
        glm::vec3 offset_;
        // position 
        glm::ivec3 position_;
        ChunkMetadata metadata_;
        // block data
        Block blocks_[World::CHUNK_SIZE_POW3]; 

    private:
        VAO opaqueVAO_;
        VBO opaqueVBO_;
        std::vector<GLuint> opaqueVertices_;

        int faces = 0;

    public: 
        Chunk(GLint posX, GLint posY, GLint posZ);
        ~Chunk();
        // Generate all blocks in the chunk
        void generate(GLuint seed);
        // rebuild the mesh
        void remesh();

        // get block by position
        Block& getBlock(GLint x, GLint y, GLint z);
        // get block by position
        Block& getBlock(glm::ivec3 position);
        // set block value
        void setBlock(glm::ivec3 position, BlockType block);
        // Upload the mesh to the GPU
        void uploadChunk(GLuint shaderProgramID);
        // draw the chunk
        void draw(GLuint shaderProgramID);

    private:
        // pack block vertices and add them to the vertices list
        void generateMesh();
        // setblock, set position
        void generateFace(Block& block, GLuint faceIndex);
};

inline std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> chunks_;
