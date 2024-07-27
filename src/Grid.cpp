#include "Grid.h"
#include <cstddef>
#include <cstdlib>

Grid::Grid(int width, int height, int depth) {
    this->width_ = width;
    this->height_ = height;
    this->depth_ = depth;
    this->cells_ = new unsigned int[width * height * depth];
}

Grid::~Grid() {
    delete[] this->cells_;
}

int Grid::get(int x, int y, int z) {
    return this->cells_[z * this->width_ * this->height_ + y * this->width_ + x];
}

int Grid::set(int x, int y, int z, unsigned int value) {
    size_t index = z * this->width_ * this->height_ + y * this->width_ + x;
    if (index > this->width_ * this->height_ * this->depth_) {
        return -1;
    }
    this->cells_[index] = value;
    return value;
}

Mesh Grid::getMesh() {
    Mesh mesh;
    for (int z = 0; z < this->depth_; z++) {
        for (int y = 0; y < this->height_; y++) {
            for (int x = 0; x < this->width_; x++) {
                if (this->get(x, y, z) == 1) {
                    mesh.addCube({x, y, z});
                }
            }
        }
    }
    mesh.upload();
    return mesh;
}

Grid newFullGrid(int width, int height, int depth) {
    Grid grid = Grid(width, height, depth);
    for (int z = 0; z < depth; z++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                grid.set(x, y, z, 1);
            }
        }
    }
    return grid;
}

Grid newEmptyGrid(int width, int height, int depth) {
    Grid grid = Grid(width, height, depth);
    for (int z = 0; z < depth; z++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                grid.set(x, y, z, 0);
            }
        }
    }
    return grid;
}

Grid newRandomGrid(int width, int height, int depth) {
    Grid grid = Grid(width, height, depth);
    for (int z = 0; z < depth; z++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                grid.set(x, y, z, rand() % 2);
            }
        }
    }
    return grid;
}

Grid newYFilledGrid(int width, int height, int depth, int fillY) {
    Grid grid = Grid(width, height, depth);
    for (int z = 0; z < depth; z++) {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                grid.set(x, y, z, y <= fillY ? 1 : 0);
            }
        }
    }
    return grid;
}
