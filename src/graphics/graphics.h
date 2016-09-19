/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#pragma once

#ifdef _WIN32
#define WINDOWS
#endif

#ifdef _WIN32
   #define WINDOWS32
   #ifdef _WIN64
      #define WINDOWS
   #endif
#endif

#ifdef __APPLE__
#define UNIX
#endif
#ifdef __linux__
#define UNIX
#endif

#include "image.h"
#include "quad.h"
#include "../math/vector.h"

#ifdef __linux__
#define LINUX
#endif

#ifdef __APPLE__
#define APPLE
#endif

#ifdef UNIX
#include "../3rdparty/SDL2/include/SDL.h"
#endif

#include "graphics.h"

#ifdef EMSCRIPTEN
#include <GLES2/gl2.h>
#else
#include "../3rdparty/glew/include/GL/glew.h"
#endif

#ifdef WINDOWS
#include "../3rdparty/glfw/include/GLFW/glfw3.h"
#endif

void graphics_init(int width, int height);

#ifdef UNIX
void graphics_destroySDLWindow();
#endif

typedef enum {
  graphics_BlendMode_additive,
  graphics_BlendMode_alpha,
  graphics_BlendMode_subtractive,
  graphics_BlendMode_multiplicative,
  graphics_BlendMode_premultiplied,
  graphics_BlendMode_replace,
  graphics_BlendMode_screen
} graphics_BlendMode;


void graphics_setBackgroundColor(float red, float green, float blue, float alpha);
void graphics_setColor(float red, float green, float blue, float alpha);
float* graphics_getColor(void);
float* graphics_getBackgroundColor(void);
void graphics_clear(void);
void graphics_swap(void);
void graphics_drawArray(graphics_Quad const* quad, mat4x4 const* tr2d, GLuint ibo, GLuint count,
                        GLenum type, GLenum indexType, float const * useColor, float ws, float hs);
mat4x4 *graphics_getProjectionMat();

//Window
int mouse_focus;
int graphics_hasMouseFocus();
int graphics_setMouseFocus(int value);
int focus;
int graphics_hasFocus();
int graphics_setFocus(int value);
int graphics_getWidth(void);
int graphics_getHeight(void);
int graphics_setTitle(const char* title);
int graphics_setMode(int width, int height);
int graphics_setFullscreen(int value, const char* mode);
int graphics_isCreated();
const char* graphics_getTitle();
int graphics_setPosition(int x, int y);

#ifndef EMSCRIPTEN
#ifdef UNIX
SDL_Window* graphics_getWindow(void);
#endif
#ifdef WINDOWS
GLFWwindow* graphics_getWindow(void);
#endif
#endif

void graphics_setColorMask(bool r, bool g, bool b, bool a);
void graphics_getColorMask(bool *r, bool *g, bool *b, bool *a);
graphics_BlendMode graphics_getBlendMode();
void graphics_setBlendMode(graphics_BlendMode mode);
void graphics_clearScissor(void);
void graphics_setScissor(int x, int y, int w, int h);
bool graphics_getScissor(int *x, int *y, int *w, int *h);
void graphics_reset(void);
