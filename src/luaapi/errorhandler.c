/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#ifdef EMSCRIPTEN
# include <emscripten.h>
#endif
#include <stdio.h>

#include "../3rdparty/lua/lua.h"

#include "../graphics/graphics.h"
#include "../graphics/font.h"

#include "errorhandler.h"

int errorhandler(lua_State *state) {
  lua_Debug debug;
  int level = 0;
  int count = 0;
  lua_pushstring(state, "\n\nStack trace:\n");
  while(lua_getstack(state, level, &debug)) {
    lua_getinfo(state, "Sl", &debug);
    if(strcmp(debug.what, "C")) {
      lua_pushstring(state, debug.short_src);
      lua_pushstring(state, ":");
      lua_pushnumber(state, debug.currentline);
      lua_pushstring(state, "\n");
      ++count;
    }
    ++level;
  }
  lua_concat(state, 4*count+2);
  return 1;
}
