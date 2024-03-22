#include <iostream>
#include <format>

#include <GL/glew.h>

#include "GLCommon.h"


void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* file, const char* func, int line) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGLError: " << std::format("({} - {}) {}:{}::{}", GLParseErrorCode(error), error, file, line, func) << std::endl;
        return false;
    }
    return true;
}

std::string GLParseErrorCode(unsigned int code) {
    switch(code) {
        case GL_INVALID_ENUM:
            return "Invalid enum";
        case GL_INVALID_VALUE:
            return "Invalid value";
        case GL_INVALID_OPERATION:
            return "Invalid operation";
        case GL_STACK_OVERFLOW:
            return "Stack overflow";
        case GL_STACK_UNDERFLOW:
            return "Stack underflox";
        case GL_OUT_OF_MEMORY:
            return "Out of memory";
        default:
            return "Not found";
    }
}

unsigned int GLSizeof(GLenum type) {
    switch (type) {
        case (GL_FLOAT): return 4;
        case (GL_UNSIGNED_INT): return 4;
        case (GL_UNSIGNED_BYTE): return 1;
        default: return 0;
    }
}
