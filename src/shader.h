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
        std::string m_vertex_path;
        std::string m_fragment_path;
        unsigned int m_vertex;
        unsigned int m_fragment;
        long m_last_modified;
        std::map<std::string, unsigned int> m_uniforms;
    public:
        ShaderProgram(const std::string &vertex_path, const std::string &fragment_path);
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

        int get_location(std::string &name);
        
    private:
        void compile_and_link();
        bool shader_changed();
};

// fetches file metadata for last timestamp
long get_file_timestamp(const std::string &path);
// fetches both files and returns the latest timestamp
long get_latest_files_timestamp(const std::string &p1, const std::string &p2);
// fethces file metadata for last timestamp and compares it with the last_time provided
bool check_file_change(const std::string &path, time_t last_time);
// searches the EOF for the file size 
int get_file_size(FILE *f);
// Loads the file contents as a dynamicly allocated char array, 
// if anything fails the the program exits
std::string read_file(const std::string &filepath);

