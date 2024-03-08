#pragma once

#include <signal.h>
#include <GL/glew.h>

#define ASSERT(x) if (!(x)) raise(SIGTRAP);
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(__FILE__, #x, __LINE__))

void GLClearError();
bool GLLogCall(const char* file, const char* func, int line);

unsigned int GLSizeof(GLenum type);
