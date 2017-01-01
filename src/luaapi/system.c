/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "system.h"
#include "tools.h"
#include <stdlib.h>
#include <stdio.h>

#include "../3rdparty/SDL2/include/SDL.h"

static int l_system_get_os(lua_State* state) {
  lua_pushstring (state, SDL_GetPlatform ());
 return 1;
}

static luaL_Reg const regFuncs[] = {
  {"getOS", l_system_get_os},
  {NULL,NULL}
};

int l_system_register(lua_State* state) {
  l_tools_registerModule(state, "system", regFuncs);
  return 0;
}
