#pragma once
// Sparse Voxel Octree

#include <glm/glm.hpp>
#include <cstdint>

struct Node {
    bool isLeaf;
    bool isTranspatent;
    uint16_t blockType;
    Node* children;

public:
    Node();
};

struct RootNode : public Node {
    glm::vec3 position;
    Node* children;

public:
    RootNode();
};
