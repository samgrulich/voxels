#include <iostream>
#include <format>

#include <GL/glew.h>

#include "glcommon.h"


void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* file, const char* func, int line) {
    GLenum error;
    while ((error = glGetError()) != GL_NO_ERROR) {
        std::cerr << "OpenGLError: " << std::format("({}) {}:{}::{}", error, file, line, func) << std::endl;
        return false;
    }
    return true;
}

unsigned int GLSizeof(GLenum type) {
    switch (type) {
        case (GL_FLOAT): return 4;
        case (GL_UNSIGNED_INT): return 4;
        case (GL_UNSIGNED_BYTE): return 1;
        default: return 0;
    }
}
