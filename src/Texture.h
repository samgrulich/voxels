#pragma once

class Texture {
public:
    unsigned int ID;
private:
    int width_, height_, nrChannels_;
public:
    Texture(const char* path, int format);
    ~Texture();
    void bind(unsigned int slot);
};
