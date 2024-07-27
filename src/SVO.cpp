#include "SVO.h"

Node::Node() {
    isLeaf = true;
    isTranspatent = false;
    blockType = 0;
    children = nullptr;
}

RootNode::RootNode() {
    position = glm::vec3(0.0f);
    children = nullptr;
}
