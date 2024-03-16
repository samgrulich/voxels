#pragma once

#include <chrono>
#include <ctime>
#include <map>
#include <string>
#include <glm/matrix.hpp>


class ShaderProgram {
    public:
        unsigned int id;
    private:
        std::string vertexPath_;
        std::string fragmentPath_;
        unsigned int vertex_;
        unsigned int fragment_;
        long lastModified_;
        std::map<std::string, unsigned int> uniforms_;
    public:
        ShaderProgram(const std::string &vertexPath, const std::string &fragmentPath);
        ~ShaderProgram();
        int refresh();
        void bind();
        void unbind();

        void set(std::string name, int val);
        void set(std::string name, float val);
        void set(std::string name, float val1, float val2);
        void set(std::string name, float val1, float val2, float val3);
        void set(std::string name, float val1, float val2, float val3, float val4);
        void set(std::string name, float* mat);
        void set(std::string name, glm::vec2 vec); 
        void set(std::string name, glm::vec3 vec); 
        void set(std::string name, glm::vec4 vec); 
        void set(std::string name, glm::mat4 mat);

        int getLocation(std::string &name);
        
    private:
        void compileAndLink();
        bool shaderChanged();
};

// fetches file metadata for last timestamp
long getFileTimestamp(const std::string &path);
// fetches both files and returns the latest timestamp
long getFilesTimestamp(const std::string &p1, const std::string &p2);
// fethces file metadata for last timestamp and compares it with the last_time provided
bool fileChanged(const std::string &path, time_t latestTime);
// searches the EOF for the file size 
int getFileSize(FILE *f);
// Loads the file contents as a dynamicly allocated char array, 
// if anything fails the the program exits
std::string readFile(const std::string &filepath);

