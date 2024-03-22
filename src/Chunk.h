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
#include "Shader.h"



class ChunkManager; 
class Chunk;


class ChunkMetadata {
    private:
        bool toGenerate_;
        bool toMesh_;
        bool toUpload_;
        bool isActive_;
        glm::ivec3 position_;
        glm::ivec3 offset_;
    public:
        // create new metadata at pos (0, 0, 0) in generate stage
        ChunkMetadata();
        // create new metadata at given position in generate stage
        ChunkMetadata(glm::ivec3 position);

        // set the chunk state to generate stage
        void setToGenerate();
        // set the chunk state to mesh stage
        void setToMesh();
        // set the chunk state to upload (mesh) stage
        void setToUpload();
        // set the chunk state to render stage
        void setToActive();
        // prepare the chunk for unloading (disable all other states)
        void setToUnload();

        // returns true if chunk is ready for generation
        bool toGenerate();
        // returns true if chunk is ready for meshing
        bool toMesh();
        // returns true if chunk has been meshed and is waiting for the mesh 
        // to be uploaded
        bool toUpload();
        // returns true if chunk is active
        bool isActive();

        // get position
        glm::ivec3 position();
};


class Chunk {
    public:
        // offset
        glm::vec3 offset_;
        // position 
        glm::ivec3 position_;
        std::shared_ptr<ChunkMetadata> metadata_;
        // block data
        Block* blocks_;
        bool visible_;

    private:
        VAO opaqueVAO_;
        VBO opaqueVBO_;
        std::vector<GLuint> opaqueVertices_;

        int faces = 0;

    public: 
        Chunk(GLint posX, GLint posY, GLint posZ);
        Chunk(glm::ivec3 pos);
        ~Chunk();
        // Generate all blocks in the chunk
        // todo
        void generate(GLuint seed);
        // pack block vertices and add them to the vertices list
        void mesh();
        // rebuild the mesh
        void remesh();

        // get block by position
        Block& getBlock(int x, int y, int z);
        // get block by position
        Block& getBlock(glm::ivec3 position);
        // returns true if the block at this position is transparent
        bool isBlockTransparent(int x, int y, int z);
        // set block value
        void setBlock(Block block);
        // draw the chunk
        void draw(ShaderProgram& shaderProgram, bool renderOpaque);

    private:
        // creates face and adds it into the vertices vector
        void generateFace(Block& block, GLuint faceIndex);
};

inline std::unordered_map<glm::ivec3, std::shared_ptr<Chunk>> chunks_;
