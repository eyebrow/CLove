/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "3rdparty/SDL2/include/SDL.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "luaapi/mouse.h"

#ifndef EMSCRIPTEN
extern SDL_Window* graphics_getWindow(void);
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
  if (strncmp (str,"l",1) == 0)
    res = SDL_BUTTON_LEFT;
  if (strncmp (str,"r",1) == 0)
    res = SDL_BUTTON_RIGHT;
  if (strncmp (str,"m",1) == 0)
    res = SDL_BUTTON_MIDDLE;
  return res;
}

void mouse_mousewheel(int y) {
  moduleData.wheel = y;
  moduleData.buttons[y] = 1;
  l_mouse_wheelmoved(moduleData.wheel);
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

void mouse_mousepressed(int x, int y, int button) {
  if (button == SDL_BUTTON_WHEEL_UP || button == SDL_BUTTON_WHEEL_DOWN) {
      l_mouse_pressed(moduleData.x, moduleData.y, button);
      mouse_mousemoved(moduleData.x, moduleData.y);
    }else{
      l_mouse_pressed(x, y, button);
      mouse_mousemoved(x, y);
    }

  moduleData.buttons[button] = 1;
}


void mouse_mousereleased(int x, int y, int button) {
  mouse_mousemoved(x, y);
  moduleData.buttons[button] = 0;
  l_mouse_released(x, y, button);
}

void mouse_getPosition(int *x, int *y) {
  *x = moduleData.x;
  *y = moduleData.y;
}

int mouse_isDown(const char *str) {
  int x = buttonEnum(str);
  if(x < 0) {
      return -1;
    }
  return moduleData.buttons[x];
}

int mouse_isVisible(void) {
  return moduleData.visible;
}

int mouse_getX(void) {
  return moduleData.x;
}

int mouse_getY(void) {
  return moduleData.y;
}

void mouse_setPosition(int x, int y) {
#ifdef EMSCRIPTEN
  SDL_WarpMouse(x, y);
#else
  SDL_WarpMouseInWindow(graphics_getWindow(), x, y);
#endif
}

void mouse_setVisible(int b) {
  moduleData.visible = !!b;
  SDL_ShowCursor(b ? SDL_ENABLE : SDL_DISABLE);
}

void mouse_setX(int x) {
#ifdef EMSCRIPTEN
  SDL_WarpMouse(x, moduleData.y);
#else
  SDL_WarpMouseInWindow(graphics_getWindow(), x, moduleData.y);
#endif
}

void mouse_setY(int y) {
#ifdef EMSCRIPTEN
  SDL_WarpMouse(moduleData.x, y);
#else
  SDL_WarpMouseInWindow(graphics_getWindow(), moduleData.x, y);
#endif
}
