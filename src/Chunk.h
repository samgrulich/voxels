#pragma once 

#include <cstdint>
#include <functional>
#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <GL/glew.h>
#include "VBO.h"
#include "WorldConstants.h"
#include "VAO.h"


typedef uint16_t BlockType;

class ChunkManager; 
class Chunk;


class ChunkMetadata {
    private:
        bool generated_;
        bool meshed_;
        bool active_;
        glm::ivec3 position_;
        std::shared_ptr<Chunk> chunk_;
    public:
        ChunkMetadata();
        ChunkMetadata(std::weak_ptr<Chunk> chunk);
        ChunkMetadata(glm::ivec3 position);

        // set the chunk state to generate stage
        void setToGenerate();
        // set the chunk state to mesh stage
        void setToMesh();
        // set the chunk state to active
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
        // get reference to the chunk
        Chunk& getRef();
        // get position
        glm::ivec3 position();
};


class Chunk {
    public:
        // offset
        GLfloat offsetX_;
        GLfloat offsetY_;
        GLfloat offsetZ_;

        // position 
        GLint positionX_;
        GLint positionY_;
        GLint positionZ_;
        
        ChunkMetadata metadata_;
        // block data
        BlockType blocks_[World::CHUNK_SIZE_POW3]; 

    private:
        VAO OpaqueVAO_;
        VBO OpaqueVBO_;
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
        BlockType getBlock(GLint x, GLint y, GLint z);
        // get block by position
        BlockType getBlock(glm::ivec3 position);
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
};

inline std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> chunks_();
