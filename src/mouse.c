/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "mouse.h"
#include "luaapi/mouse.h"

#ifndef EMSCRIPTEN
#ifdef UNIX
extern SDL_Window* graphics_getWindow(void);
#endif
#endif

static struct {
  int x, y;
  int dx, dy;
  int visible;
  int buttons[256];
  lua_State* luaState;
  int wheel;
} moduleData;

static int buttonEnum(const char *str) {
  int res = 0;
#ifdef UNIX
  if (strncmp (str,"l",1) == 0)
    res = SDL_BUTTON_LEFT;
  if (strncmp (str,"r",1) == 0)
    res = SDL_BUTTON_RIGHT;
  if (strncmp (str,"m",1) == 0)
    res = SDL_BUTTON_MIDDLE;
  if (strncmp (str,"x1",2) == 0)
    res = SDL_BUTTON_X1;
  if (strncmp (str,"x2",2) == 0)
    res = SDL_BUTTON_X2;
  return res;
#endif
#ifdef WINDOWS
  if (strncmp(str,"l",1) == 0)
    res = GLFW_MOUSE_BUTTON_LEFT;
  if (strncmp(str,"r",1) == 0)
    res = GLFW_MOUSE_BUTTON_RIGHT;
  if (strncmp(str,"m",1) == 0)
    res = GLFW_MOUSE_BUTTON_MIDDLE;
  return res;
#endif
  return 0;
}


int mouse_getwheel() {
  return moduleData.wheel;
}

void mouse_mousemoved(int x, int y) {
  if(moduleData.x == x && moduleData.y == y) {
      return;
    }

  moduleData.dx = x - moduleData.x;
  moduleData.dy = y - moduleData.y;
  moduleData.x = x;
  moduleData.y = y;
}

void mouse_mousewheel(int y) {
  moduleData.wheel = y;
  moduleData.buttons[y] = 1;
  l_mouse_wheelmoved(moduleData.wheel);
#ifdef WINDOWS
  mouseScrollY = 0;
#endif
}

#ifdef WINDOWS

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
  mousePressed[button] = action != GLFW_RELEASE;
  mouseRelased[button] = action != GLFW_PRESS;
  mouseButton = button;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
  mouseScrollY = yoffset;
}

void mouse_setcallback() {
  glfwSetMouseButtonCallback(graphics_getWindow(), mouse_button_callback);
  glfwSetScrollCallback(graphics_getWindow(), scroll_callback);
}

int mouse_getmousepressedGLFW(int v)
{
  if(v <= MAX_MOUSE)
    return mousePressed[v];
  else
    printf("%s \n", "You're trying to get an unknow button");
  return -1;
}

int mouse_getmousereleasedGLFW(int v)
{
  if(v <= MAX_MOUSE)
    return mouseRelased[v];
  else
    printf("%s \n","You're trying to get an unknow button");
  return -1;
}

#endif

int mouse_isDown(const char *str) {
  int x = buttonEnum(str);
  if(x < 0) {
      return -1;
    }
#ifdef UNIX
  return moduleData.buttons[x];
#endif
#ifdef WINDOWS
  return mouse_getmousepressedGLFW(x);
#endif
}

void mouse_mousereleased(int x, int y, int button) {
#ifdef UNIX
  moduleData.buttons[button] = 0;
  l_mouse_released(x, y, button);
  mouse_mousemoved(x, y);
#endif
#ifdef WINDOWS
  /*
   * GLFW will register more than once the key that has been pressed, that has been fixed with a little "hack"
   */
  //printf("%i %s %i \n",mouseButton," ",mouse_getmousereleasedGLFW(button));
  if (mouse_getmousereleasedGLFW(button) && mouseButton == button){
      l_mouse_released(x, y, button);
      mouse_mousemoved(x, y);
      mouseButton = -1;
      button = 0;
    }
#endif
}

void mouse_mousepressed(int x, int y, int button) {
#ifdef UNIX
  if (button == -1 || button == 1) {
      l_mouse_pressed(moduleData.x, moduleData.y, button);
      mouse_mousemoved(moduleData.x, moduleData.y);
    }else{
      l_mouse_pressed(x, y, button);
      mouse_mousemoved(x, y);
    }
  moduleData.buttons[button] = 1;
#endif
#ifdef WINDOWS
  //Same hacks have been done here too beacuse otherwhise GLFW will register more than once the key that has been pressed
  if (mouse_getmousepressedGLFW(button) && mouseButton == button){
      l_mouse_pressed(x, y, button);
      mouse_mousemoved(x, y);
      mouseButton = -1;
      button = 0;
    }
#endif

}

void mouse_getPosition(int *x, int *y) {
  *x = moduleData.x;
  *y = moduleData.y;
}

int mouse_isVisible(void) {
  return moduleData.visible;
}

int mouse_getX(void) {
#ifdef UNIX
  return moduleData.x;
#endif
#ifdef WINDOWS
  double x;
  double y;
  glfwGetCursorPos(graphics_getWindow(),&x, &y);
  moduleData.x = x;
  moduleData.y = y;
  return x;
#endif
}

int mouse_getY(void) {
#ifdef UNIX
  return moduleData.y;
#endif
#ifdef WINDOWS
  double y;
  double x;
  moduleData.x = x;
  moduleData.y = y;
  glfwGetCursorPos(graphics_getWindow(),&x, &y);
  return y;
#endif
}

void mouse_setPosition(int x, int y) {
#ifdef EMSCRIPTEN
  SDL_WarpMouse(x, y);
#else

#ifdef UNIX
  SDL_WarpMouseInWindow(graphics_getWindow(), x, y);
#endif

#ifdef WINDOWS
  glfwSetCursorPos(graphics_getWindow(), x, y);
#endif

#endif
}

void mouse_setVisible(int b) {
  moduleData.visible = !!b;
#ifdef UNIX
  SDL_ShowCursor(b ? SDL_ENABLE : SDL_DISABLE);
#endif
#ifdef WINDOWS
  GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
  if (b == 0) glfwDestroyCursor(cursor);
  if (b == 1) glfwSetCursor(graphics_getWindow(), cursor);
#endif
}

void mouse_setX(int x) {
#ifdef EMSCRIPTEN
  SDL_WarpMouse(x, moduleData.y);
#else

#ifdef UNIX
  SDL_WarpMouseInWindow(graphics_getWindow(), x, moduleData.y);
#endif

#ifdef WINDOWS
  glfwSetCursorPos(graphics_getWindow(), x, moduleData.y);
#endif

#endif
}

void mouse_setY(int y) {
#ifdef EMSCRIPTEN
  SDL_WarpMouse(moduleData.x, y);
#else

#ifdef UNIX
  SDL_WarpMouseInWindow(graphics_getWindow(), moduleData.x, y);
#endif

#ifdef WINDOWS
  glfwSetCursorPos(graphics_getWindow(), moduleData.x, y);
#endif

#endif
}
