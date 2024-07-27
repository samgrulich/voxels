#pragma once

#include "Mesh.h"
class Grid {
    int width_, height_, depth_;
    unsigned int* cells_;
    
public:
    Grid(int width, int height, int depth);
    ~Grid();

    int get(int x, int y, int z);
    int set(int x, int y, int z, unsigned int value);
    Mesh getMesh();
};

Grid newFullGrid(int width, int height, int depth);
Grid newEmptyGrid(int width, int height, int depth);
Grid newRandomGrid(int width, int height, int depth);
Grid newYFilledGrid(int width, int height, int depth, int fillY);

