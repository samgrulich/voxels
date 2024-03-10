#include <filesystem>
#include <fstream>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "glcommon.h"
#include "shader.h"

#define SHADER_BUFFER_INIT_SIZE 1024
#define ERR_MSG_BUFFER_SIZE 1024


int create_shader_program(unsigned int type, const std::string &filepath) {
    GLCall(unsigned int id = glCreateShader(type));
    std::string shader = read_file(filepath);
    const char* shader_str = shader.c_str();
    GLCall(glShaderSource(id, 1, (const char* const*)(&shader_str), NULL));
    GLCall(glCompileShader(id));
    shader.clear();

    int is_compiled;
    glGetShaderiv(id, GL_COMPILE_STATUS, &is_compiled);
    if (is_compiled != GL_TRUE) {
        int log_length = 0;
        char message[ERR_MSG_BUFFER_SIZE];
        glGetShaderInfoLog(id, ERR_MSG_BUFFER_SIZE, &log_length, message);
        glDeleteShader(id);
        throw std::format("Failed to compile shader! ({})::({})", filepath, message);
    }
    return id;
}

ShaderProgram::ShaderProgram(const std::string &vertex_path, const std::string &fragment_path) 
    : m_uniforms(std::map<std::string, unsigned int>()), id(glCreateProgram()), 
        m_vertex_path(vertex_path), m_fragment_path(fragment_path)
{
    m_last_modified = get_latest_files_timestamp(vertex_path, fragment_path);
    compile_and_link();
}

ShaderProgram::~ShaderProgram() {
    // todo
    glDeleteProgram(id);
}

int ShaderProgram::refresh() {
    if (!shader_changed())
        return 0;
    
    GLCall(glDetachShader(id, m_vertex));
    GLCall(glDetachShader(id, m_fragment));
    // GLCall(glDeleteShader(m_vertex));
    // GLCall(glDeleteShader(m_fragment));

    try {
        compile_and_link(); 
    } catch (std::string msg) {
        std::cerr << "Warning: " << msg << std::endl;
        return 1;
    }
    return 0;
}

void ShaderProgram::bind() {
    GLCall(glUseProgram(id));
}

void ShaderProgram::unbind() {
    GLCall(glUseProgram(0));
}

void ShaderProgram::set(std::string name, int val) {
    int loc = get_location(name);
    bind();
    GLCall(glUniform1i(loc, val));
}

void ShaderProgram::set(std::string name, float val) {
    int loc = get_location(name);
    bind();
    GLCall(glUniform1f(loc, val));
}

void ShaderProgram::set(std::string name, float val1, float val2) {
    int loc = get_location(name);
    bind();
    GLCall(glUniform2f(loc, val1, val2));
}

void ShaderProgram::set(std::string name, float val1, float val2, float val3) {
    int loc = get_location(name);
    bind();
    GLCall(glUniform3f(loc, val1, val2, val3));
}

void ShaderProgram::set(std::string name, float val1, float val2, float val3, float val4) {
    int loc = get_location(name);
    bind();
    GLCall(glUniform4f(loc, val1, val2, val3, val4));
}

void ShaderProgram::set(std::string name, float* mat) {
    int loc = get_location(name);
    bind();
    GLCall(glUniformMatrix4fv(loc, 1, GL_FALSE, mat));
}

void ShaderProgram::set(std::string name, glm::vec2 vec) {
    int loc = get_location(name);
    bind();
    GLCall(glUniform2f(loc, vec.x, vec.y));
}

void ShaderProgram::set(std::string name, glm::vec3 vec) {
    int loc = get_location(name);
    bind();
    GLCall(glUniform3f(loc, vec.x, vec.y, vec.z));
}

void ShaderProgram::set(std::string name, glm::vec4 vec) {
    int loc = get_location(name);
    bind();
    GLCall(glUniform4f(loc, vec.x, vec.y, vec.z, vec.w));
}

void ShaderProgram::set(std::string name, glm::mat4 mat) {
    int loc = get_location(name);
    bind();
    GLCall(glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat)));
}

int ShaderProgram::get_location(std::string &name) {
    int loc;
    if(!m_uniforms.contains(name)) {
        GLCall(loc = glGetUniformLocation(id, name.c_str()));
        m_uniforms.emplace(name, loc);
    } else {
        loc = m_uniforms[name];
    }
    return loc;
}

void ShaderProgram::compile_and_link() {
    m_vertex = create_shader_program(GL_VERTEX_SHADER, m_vertex_path);
    m_fragment = create_shader_program(GL_FRAGMENT_SHADER, m_fragment_path);

    glAttachShader(id, m_vertex);
    glAttachShader(id, m_fragment);
    glLinkProgram(id);
    glValidateProgram(id);

    int is_linked, is_valid;
    glGetProgramiv(id, GL_LINK_STATUS, &is_linked);
    glGetProgramiv(id, GL_VALIDATE_STATUS, &is_valid);
    
    if (is_linked != GL_TRUE || is_valid != GL_TRUE) {
        int log_length = 0;
        char message[ERR_MSG_BUFFER_SIZE];
        glGetProgramInfoLog(id, ERR_MSG_BUFFER_SIZE, &log_length, message);
        const char *type = is_linked != GL_TRUE ? "link" : "validate";
        throw std::format("Failed to {} program! ({})", type, message);
    }

    glDeleteShader(m_vertex);
    glDeleteShader(m_fragment);
}

bool ShaderProgram::shader_changed() {
    long time = get_latest_files_timestamp(m_vertex_path, m_fragment_path);
    return time != m_last_modified;
}

int get_file_size(FILE *f) {
    fseek(f, 0L, SEEK_END); // from stackoverflow https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
    long sz = ftell(f);
    rewind(f);
    return sz;
}

// function ---------------
std::string read_file(const std::string &filepath) {
    std::ifstream t(filepath.c_str());
    std::string str((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
    return str;
}

// function --------------
long get_file_timestamp(const std::string &path) {
    auto time = std::filesystem::last_write_time(path);
    return time.time_since_epoch().count();
}

// function --------------
long get_latest_files_timestamp(const std::string &p1, const std::string &p2) {
    auto a = get_file_timestamp(p1);
    auto b = get_file_timestamp(p2);
    return a > b ? a : b;
}

// function --------------
bool check_file_change(const std::string &path, time_t last_time) {
    long current = get_file_timestamp(path);
    return current > last_time;
}

