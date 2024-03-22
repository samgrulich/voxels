#pragma once

#include <string>
#include <signal.h>
#include <GL/glew.h>

#define ASSERT(x) if (!(x)) raise(SIGTRAP);
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(__FILE__, #x, __LINE__))

void GLClearError();
bool GLLogCall(const char* file, const char* func, int line);
std::string GLParseErrorCode(unsigned int code);

unsigned int GLSizeof(GLenum type);
