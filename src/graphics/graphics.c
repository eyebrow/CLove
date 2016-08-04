/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include <stdint.h>

#include "../math/vector.h"
#include "../mouse.h"
#include "../keyboard.h"
#include "../luaapi/event.h"

#include "matrixstack.h"
#include "font.h"
#include "batch.h"
#include "quad.h"
#include "shader.h"
#include "geometry.h"
#include "graphics.h"
#include <stdio.h>

typedef struct {
  float red;
  float green;
  float blue;
  float alpha;
} graphics_Color;

static struct {
#ifndef EMSCRIPTEN
#ifndef WINDOWS
  SDL_Window* window;
  SDL_GLContext context;
#endif
#endif
#ifndef WINDOWS
  SDL_Surface* surface;
#endif
#ifdef WINDOWS
  const GLFWvidmode* mode;
  GLFWwindow* window;
#endif
  graphics_Color backgroundColor;
  graphics_Color foregroundColor;

  bool colorMask[4];
  graphics_BlendMode blendMode;
  int scissorBox[4];
  bool scissorSet;

  mat4x4 projectionMatrix;
  int isCreated;
  int width;
  int height;
  const char* title;
  int x;
  int y;
} moduleData;

#ifndef EMSCRIPTEN
#ifdef UNIX
SDL_Window* graphics_getWindow(void) {
  return moduleData.window;
}
#endif
#ifdef WINDOWS
GLFWwindow* graphics_getWindow(void) {
  return moduleData.window;
}
#endif
#endif

void graphics_init(int width, int height) {

#ifndef WINDOWS
  if (SDL_Init(SDL_INIT_VIDEO) != 0)
    printf("Error: Could not init SDL \n");
#endif
#ifdef WINDOWS
  if(!glfwInit())
    printf("%s /n", "Error: Could not init glfw(window)");

  moduleData.mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
#endif

  moduleData.isCreated = 0;
#ifdef EMSCRIPTEN
  moduleData.surface = SDL_SetVideoMode(width, height, 0, SDL_OPENGL);
#else
#ifndef WINDOWS
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, 0);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

  moduleData.width = width;
  moduleData.height = height;
  moduleData.x = SDL_WINDOWPOS_UNDEFINED;
  moduleData.y = SDL_WINDOWPOS_UNDEFINED;
  moduleData.title = "Untitled";
  moduleData.window = SDL_CreateWindow(moduleData.title, moduleData.x, moduleData.y, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
  if(moduleData.window == NULL)
    printf("%s \n", "Error: Could not create window :O");
  moduleData.context = SDL_GL_CreateContext(moduleData.window);
  //moduleData.surface = SDL_GetWindowSurface(moduleData.window);
  SDL_GL_SetSwapInterval(1); //limit FPS to 60, this may not work on all drivers
#endif
#ifdef WINDOWS
  glfwWindowHint(GLFW_RED_BITS, 8);
  glfwWindowHint(GLFW_GREEN_BITS, 8);
  glfwWindowHint(GLFW_BLUE_BITS, 8);
  glfwWindowHint(GLFW_ALPHA_BITS, 8);
  glfwWindowHint(GLFW_DEPTH_BITS, 16);
  glfwWindowHint(GLFW_SAMPLES, 2); //antialasing x2
  glfwWindowHint(GLFW_OPENGL_PROFILE,  GLFW_OPENGL_ANY_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

  moduleData.title = "Untitled";
  moduleData.x = -1;
  moduleData.y = -1;
  moduleData.width = width;
  moduleData.height = height;

#ifndef EMSCRIPTEN
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#endif

  moduleData.window = glfwCreateWindow(width, height, moduleData.title, NULL, NULL);
  graphics_setPosition(-1, -1);
  //Init all callbacks required for input
  mouse_setcallback(); //glfw only
  keyboard_setcallback(); //glfw only

  if (!moduleData.window){
      printf("%s \n", "Error: Could not create window");
      glfwTerminate();
    }
#ifndef EMSCRIPTEN
  glfwMakeContextCurrent(moduleData.window);
#endif
  int major, minor, rev;
  glfwGetVersion(&major, &minor, &rev);
  printf("%s %i %s %i %s %i %s \n","GLFW version: ", major , "." , minor , "." , rev , " inited.");
#endif
#endif

  glewExperimental = true;
  GLenum res = glewInit();
  if(res != GLEW_OK)
    printf("Could not init glew.Something must be very wrong, no gpu drivers?");

  printf("%s \n","Debug, OpenGL version: ", glGetString(GL_VERSION));
  printf("%s \n","Debug, GLSL version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));

  glViewport(0, 0, width, height);

  matrixstack_init();

  //m4x4_newTranslation(&moduleData.projectionMatrix, -1.0f, 1.0f, 0.0f);
  //m4x4_scale(&moduleData.projectionMatrix, 2.0f / width, -2.0f / height, 0.0f);
  m4x4_newOrtho(&moduleData.projectionMatrix, 0.0f, moduleData.width, moduleData.height, 0.0f, 0.0f, 100.0f);

  moduleData.isCreated = 1;

  graphics_setColor(1.0f, 1.0f, 1.0f, 1.0f);

  graphics_geometry_init();
  graphics_font_init();
  graphics_batch_init();
  graphics_image_init();
  graphics_shader_init();

  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glPixelStorei(GL_PACK_ALIGNMENT, 1);

  graphics_setColorMask(true, true, true, true);
  graphics_setBlendMode(graphics_BlendMode_alpha);
  glEnable(GL_BLEND);
  graphics_clearScissor();

}

void graphics_setBackgroundColor(float red, float green, float blue, float alpha) {
  moduleData.backgroundColor.red   = red;
  moduleData.backgroundColor.green = green;
  moduleData.backgroundColor.blue  = blue;
  moduleData.backgroundColor.alpha = alpha;
  glClearColor(red, green, blue, alpha);
}

void graphics_setColor(float red, float green, float blue, float alpha) {
  moduleData.foregroundColor.red   = red;
  moduleData.foregroundColor.green = green;
  moduleData.foregroundColor.blue  = blue;
  moduleData.foregroundColor.alpha = alpha;
}

void graphics_clear(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#ifdef WINDOWS
  glfwSwapInterval(1); // limit fps to monitor's refresh rate
#endif
}

void graphics_swap(void) {
#ifdef EMSCRIPTEN
#ifndef WINDOWS
  SDL_GL_SwapBuffers();
#endif
#else
#ifndef WINDOWS
  SDL_GL_SwapWindow(moduleData.window);
#endif
#endif

#ifdef WINDOWS
  // Update love.mousepressed / released(x,y,button) / wheel
  mouse_mousepressed(mouse_getX(), mouse_getY(), mouseButton);
  mouse_mousereleased(mouse_getX(), mouse_getY(), mouseButton);
  mouse_mousewheel(mouseScrollY);

  // Update love.keypressed / released (b) / textinput
  keyboard_keypressed(keyboardButton);
  keyboard_keyreleased(keyboardButton);
  keyboard_textInput(returnGLFWKey(keyboardCodepoint - 32));

  glfwSwapBuffers(graphics_getWindow());
  glfwPollEvents();
#endif
}

void graphics_drawArray(graphics_Quad const* quad, mat4x4 const* tr2d, GLuint ibo, GLuint count, GLenum type, GLenum indexType, float const* useColor, float ws, float hs) {

  mat4x4 tr;
  m4x4_mulM4x4(&tr, tr2d, matrixstack_head());

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (const void*)(2*sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 8*sizeof(float), (const void*)(4*sizeof(float)));

  graphics_Shader_activate(
        &moduleData.projectionMatrix,
        &tr,
        quad,
        useColor,
        ws,
        hs
        );

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glDrawElements(type, count, indexType, (GLvoid const*)0);

  /*glDisableVertexAttribArray(3);
  glDisableVertexAttribArray(2);
  This piece of code does not seem to do anything. Untill proven other
  whise I'll comment it
  glDisableVertexAttribArray(0);*/
}



int graphics_setTitle(const char* title){
#ifndef EMSCRIPTEN
  moduleData.title = title;
#ifdef UNIX
  SDL_SetWindowTitle(moduleData.window,title);
#endif
#ifdef WINDOWS
  glfwSetWindowTitle(moduleData.window, title);
#endif
#endif
  return 1;
}

int graphics_hasMouseFocus(){
  return mouse_focus;
}

int graphics_setMouseFocus(int value){
  mouse_focus = value;
  return 1;
}

int graphics_hasFocus(){
#ifdef UNIX
  return focus;
#endif
#ifdef WINDOWS
  return glfwGetWindowAttrib(moduleData.window, GLFW_FOCUSED);
#endif
}

int graphics_setFocus(int value){
  focus = value;
  return 1;
}

int graphics_setPosition(int x, int y)
{
#ifndef EMSCRIPTEN
#ifdef UNIX
  if(x <= -1) // center x
    x = SDL_WINDOWPOS_CENTERED;
  if(y <= -1) // center y
    y = SDL_WINDOWPOS_CENTERED;
  moduleData.x = x;
  moduleData.y = y;
  SDL_SetWindowPosition(moduleData.window, x, y);
#endif
#ifdef WINDOWS
  moduleData.x = x;
  moduleData.y = y;
  if(x == -1 && y == -1)
    glfwSetWindowPos(moduleData.window, (moduleData.mode->width - moduleData.width)*.5f, (moduleData.mode->height - moduleData.height)*.5f);
  else if(x == -1)
    glfwSetWindowPos(moduleData.window, (moduleData.mode->width - moduleData.width)*.5f, y);
  else if(y == -1)
    glfwSetWindowPos(moduleData.window, x, (moduleData.mode->height - moduleData.height)*.5f);
  else
    glfwSetWindowPos(moduleData.window, x, y);
#endif
#endif
  return 1;
}

int graphics_setMode(int width, int height){
#ifndef EMSCRIPTEN
  moduleData.width = width;
  moduleData.height = height;

#ifdef UNIX
  SDL_SetWindowSize(moduleData.window, width, height);
#endif

  graphics_setPosition(-1, -1);
#else
  //moduleData.surface = SDL_SetVideoMode(width, height, 0, SDL_OPENGL);
  SDL_SetWindowSize(moduleData.window,width,height);
#endif

#ifdef WINDOWS
  glfwSetWindowSize(moduleData.window, width, height);
#endif

  return 1;
}

int graphics_getWidth(void) {
  int w;
  int h;
#ifdef UNIX
  SDL_GetWindowSize(moduleData.window,&w,&h);
#endif
#ifdef WINDOWS
  glfwGetWindowSize(moduleData.window, &w, &h);
#endif
  return w;
}

int graphics_getHeight(void) {
  int w;
  int h;
#ifdef UNIX
  SDL_GetWindowSize(moduleData.window,&w,&h);
#endif
#ifdef WINDOWS
  glfwGetWindowSize(moduleData.window, &w, &h);
#endif
  return h;
}

const char* graphics_getTitle()
{
  return moduleData.title;
}

int graphics_setFullscreen(int value, const char* mode){

#ifndef EMSCRIPTEN
#ifdef UNIX
  if ((strncmp(mode,"desktop", 7) == 0) && value == 1)
    SDL_SetWindowFullscreen(moduleData.window, SDL_WINDOW_FULLSCREEN_DESKTOP);
#endif

#endif
  return 1;
}

int graphics_isCreated()
{
  return moduleData.isCreated;
}

float* graphics_getColor(void) {
  return (float*)(&moduleData.foregroundColor);
}

float* graphics_getBackgroundColor(void) {
  return (float*)(&moduleData.backgroundColor);
}

void graphics_setColorMask(bool r, bool g, bool b, bool a) {
  moduleData.colorMask[0] = r;
  moduleData.colorMask[1] = g;
  moduleData.colorMask[2] = b;
  moduleData.colorMask[3] = a;

  glColorMask(r,g,b,a);
}

void graphics_getColorMask(bool *r, bool *g, bool *b, bool *a) {
  *r = moduleData.colorMask[0];
  *g = moduleData.colorMask[1];
  *b = moduleData.colorMask[2];
  *a = moduleData.colorMask[3];
}

graphics_BlendMode graphics_getBlendMode(void) {
  return moduleData.blendMode;
}

void graphics_setBlendMode(graphics_BlendMode mode) {
  moduleData.blendMode = mode;

  GLenum sfRGB = GL_ONE;
  GLenum dfRGB = GL_ZERO;
  GLenum sfA = GL_ONE;
  GLenum dfA = GL_ZERO;
  GLenum bFunc = GL_FUNC_ADD;

  switch(mode) {
    case graphics_BlendMode_alpha:
      sfRGB = GL_SRC_ALPHA;
      sfA = GL_ONE;
      dfRGB = dfA = GL_ONE_MINUS_SRC_ALPHA;
      break;

    case graphics_BlendMode_subtractive:
      bFunc = GL_FUNC_REVERSE_SUBTRACT;
      // fallthrough
    case graphics_BlendMode_additive:
      sfA = sfRGB = GL_SRC_ALPHA;
      dfA = dfRGB = GL_ONE;
      break;


    case graphics_BlendMode_multiplicative:
      sfA = sfRGB = GL_DST_COLOR;
      dfA = dfRGB = GL_ZERO;
      break;

    case graphics_BlendMode_premultiplied:
      sfA = sfRGB = GL_ONE;
      dfA = dfRGB = GL_ONE_MINUS_SRC_ALPHA;
      break;

    case graphics_BlendMode_screen:
      sfA = sfRGB = GL_ONE;
      dfA = dfRGB = GL_ONE_MINUS_SRC_COLOR;
      break;

    case graphics_BlendMode_replace:
    default:
      // uses default init values
      break;
    }

  glBlendFuncSeparate(sfRGB, dfRGB, sfA, dfA);
  glBlendEquation(bFunc);
}

void graphics_clearScissor(void) {
  moduleData.scissorSet = false;
  glDisable(GL_SCISSOR_TEST);
}

void graphics_setScissor(int x, int y, int w, int h) {
  moduleData.scissorBox[0] = x;
  moduleData.scissorBox[1] = y;
  moduleData.scissorBox[2] = w;
  moduleData.scissorBox[3] = h;
  moduleData.scissorSet = true;
  glScissor(x,y,w,h);
  glEnable(GL_SCISSOR_TEST);
}

bool graphics_getScissor(int *x, int *y, int *w, int *h) {
  if(!moduleData.scissorSet) {
      return false;
    }

  *x = moduleData.scissorBox[0];
  *y = moduleData.scissorBox[1];
  *w = moduleData.scissorBox[2];
  *h = moduleData.scissorBox[3];

  return true;
}

void graphics_reset(void) {
  matrixstack_origin();
  graphics_setColor(1.0f, 1.0f, 1.0f, 1.0f);
  graphics_setBackgroundColor(0.0f, 0.0f, 0.0f, 1.0f);
  graphics_setBlendMode(graphics_BlendMode_alpha);
  graphics_setDefaultShader();
  graphics_setColorMask(true, true, true, true);
  graphics_clearScissor();
}
