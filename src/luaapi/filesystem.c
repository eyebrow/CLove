/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include <stdlib.h>
#include "../3rdparty/lua/lauxlib.h"
#include "filesystem.h"
#include "tools.h"
#include "../filesystem/filesystem.h"

#include "../3rdparty/physfs/physfs.h"

static int l_filesystem_read(lua_State* state) {
  // TODO implement max length
  char const* filename = l_tools_toStringOrError(state, 1);
  char* data = NULL;
  int len = filesystem_read(filename, &data);
  if(len < 0) {
      lua_pushstring(state, "could not read file");
      return lua_error(state);
    }

  lua_pushstring(state, data);
  free(data);
  lua_pushnumber(state, len);
  return 2;
}

static int l_filesystem_exists(lua_State* state)
{
  const char* filename = l_tools_toStringOrError(state, 1);
  return filesystem_exists(filename);
}

static int l_filesystem_write(lua_State* state)
{
  const char* filename = l_tools_toStringOrError(state, 1);
  const char* data = l_tools_toStringOrError(state, 2);
  filesystem_write(filename, data);
  return 2;
}

static int l_filesystem_append(lua_State* state)
{
  const char* filename = l_tools_toStringOrError(state, 1);
  const char* data = l_tools_toStringOrError(state, 2);
  filesystem_append(filename, data);
  return 2;
}

static int l_filesystem_load(lua_State* state) {
  char const* filename = l_tools_toStringOrError(state, 1);
  char* data = NULL;
  int len = filesystem_read(filename, &data);
  if(len < 0) {
      lua_pushstring(state, "could not read file");
      return lua_error(state);
    }

  luaL_loadstring(state, data);
  free(data);
  return 1;
}

static int l_filesystem_remove(lua_State* state) {
  char const* file = l_tools_toStringOrError(state, 1);
  filesystem_remove(file);
  return 0;
}

static int l_filesystem_enumerate(lua_State* state) {
  int n = lua_gettop(state);

  if( n != 1 )
    return luaL_error(state, "Function requires a single parameter.");

  const char * dir = l_tools_toStringOrError(state, 1);
  char **rc = PHYSFS_enumerateFiles(dir);
  char **i;
  int index = 1;

  lua_newtable(state);

  for (i = rc; *i != NULL; i++)
    {
      lua_pushinteger(state, index);
      lua_pushstring(state, *i);
      lua_settable(state, -3);
      index++;
    }

  PHYSFS_freeList(rc);
  return 1;
}

static luaL_Reg const regFuncs[] = {
  {"load", l_filesystem_load},
  {"enumerate",l_filesystem_enumerate},
  {"remove", l_filesystem_remove},
  {"read", l_filesystem_read},
  {"exists", l_filesystem_exists},
  {"write", l_filesystem_write},
  {"append", l_filesystem_append},
  {NULL, NULL}
};

int l_filesystem_register(lua_State* state) {
  l_tools_registerModule(state, "filesystem", regFuncs);
  return 0;
}
