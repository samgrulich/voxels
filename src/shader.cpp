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
    std::string shader = readFile(filepath);
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

ShaderProgram::ShaderProgram(const std::string &vertexPath, const std::string &fragmentPath) 
    : uniforms_(std::map<std::string, unsigned int>()), id(glCreateProgram()), 
        vertexPath_(vertexPath), fragmentPath_(fragmentPath)
{
    lastModified_ = getFilesTimestamp(vertexPath, fragmentPath);
    compileAndLink();
}

ShaderProgram::~ShaderProgram() {
    // todo
    glDeleteProgram(id);
}

int ShaderProgram::refresh() {
    if (!shaderChanged())
        return 0;
    
    GLCall(glDetachShader(id, vertex_));
    GLCall(glDetachShader(id, fragment_));
    // GLCall(glDeleteShader(m_vertex));
    // GLCall(glDeleteShader(m_fragment));

    try {
        compileAndLink(); 
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
    int loc = getLocation(name);
    bind();
    GLCall(glUniform1i(loc, val));
}

void ShaderProgram::set(std::string name, float val) {
    int loc = getLocation(name);
    bind();
    GLCall(glUniform1f(loc, val));
}

void ShaderProgram::set(std::string name, float val1, float val2) {
    int loc = getLocation(name);
    bind();
    GLCall(glUniform2f(loc, val1, val2));
}

void ShaderProgram::set(std::string name, float val1, float val2, float val3) {
    int loc = getLocation(name);
    bind();
    GLCall(glUniform3f(loc, val1, val2, val3));
}

void ShaderProgram::set(std::string name, float val1, float val2, float val3, float val4) {
    int loc = getLocation(name);
    bind();
    GLCall(glUniform4f(loc, val1, val2, val3, val4));
}

void ShaderProgram::set(std::string name, float* mat) {
    int loc = getLocation(name);
    bind();
    GLCall(glUniformMatrix4fv(loc, 1, GL_FALSE, mat));
}

void ShaderProgram::set(std::string name, glm::vec2 vec) {
    int loc = getLocation(name);
    bind();
    GLCall(glUniform2f(loc, vec.x, vec.y));
}

void ShaderProgram::set(std::string name, glm::vec3 vec) {
    int loc = getLocation(name);
    bind();
    GLCall(glUniform3f(loc, vec.x, vec.y, vec.z));
}

void ShaderProgram::set(std::string name, glm::vec4 vec) {
    int loc = getLocation(name);
    bind();
    GLCall(glUniform4f(loc, vec.x, vec.y, vec.z, vec.w));
}

void ShaderProgram::set(std::string name, glm::mat4 mat) {
    int loc = getLocation(name);
    bind();
    GLCall(glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat)));
}

int ShaderProgram::getLocation(std::string &name) {
    int loc;
    if(!uniforms_.contains(name)) {
        GLCall(loc = glGetUniformLocation(id, name.c_str()));
        uniforms_.emplace(name, loc);
    } else {
        loc = uniforms_[name];
    }
    return loc;
}

void ShaderProgram::compileAndLink() {
    vertex_ = create_shader_program(GL_VERTEX_SHADER, vertexPath_);
    fragment_ = create_shader_program(GL_FRAGMENT_SHADER, fragmentPath_);

    glAttachShader(id, vertex_);
    glAttachShader(id, fragment_);
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

    glDeleteShader(vertex_);
    glDeleteShader(fragment_);
}

bool ShaderProgram::shaderChanged() {
    long time = getFilesTimestamp(vertexPath_, fragmentPath_);
    return time != lastModified_;
}

int getFileSize(FILE *f) {
    fseek(f, 0L, SEEK_END); // from stackoverflow https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
    long sz = ftell(f);
    rewind(f);
    return sz;
}

// function ---------------
std::string readFile(const std::string &filepath) {
    std::ifstream t(filepath.c_str());
    std::string str((std::istreambuf_iterator<char>(t)),
                     std::istreambuf_iterator<char>());
    return str;
}

// function --------------
long getFileTimestamp(const std::string &path) {
    auto time = std::filesystem::last_write_time(path);
    return time.time_since_epoch().count();
}

// function --------------
long getFilesTimestamp(const std::string &p1, const std::string &p2) {
    auto a = getFileTimestamp(p1);
    auto b = getFileTimestamp(p2);
    return a > b ? a : b;
}

// function --------------
bool fileChanged(const std::string &path, time_t lastestTime) {
    long current = getFileTimestamp(path);
    return current > lastestTime;
}

