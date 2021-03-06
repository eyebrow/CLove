/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include <stdlib.h>
#include "../3rdparty/lua/lauxlib.h"
#include "../3rdparty/lua/lua.h"
#include "../3rdparty/lua/lualib.h"
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

static int l_filesystem_getSaveDirectory(lua_State* state) {
    const char* company = luaL_optstring(state, 2, "Clove");
    const char* projName = luaL_optstring(state, 1, "myGame");
    const char* path = filesystem_getSaveDirectory(company, projName);
    lua_pushstring(state, path);
    return 1;
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

static int l_filesystem_getSource(lua_State* state) {
  lua_pushstring(state, PHYSFS_getBaseDir());
  return 1;
}

static int l_filesystem_setSource(lua_State* state) {
  const char* p = l_tools_toStringOrError(state, 1);

  int a = PHYSFS_addToSearchPath(p, 1);
  if(!a)
    printf("%s %s \n", "Couldn't set source", p);

  return 1;
}

static int l_filesystem_set_requireDir(lua_State* state) {
  if(!lua_isstring(state, 1))
    return luaL_error(state, "The argument must be a string.");

  const char* dir = lua_tostring(state, 1);

  int m = PHYSFS_mount(dir, "/", 1);
  if(!m)
    printf("%s %s \n", "No folder/.zip named ", dir);

  return 1;
}

static int l_filesystem_require(lua_State* state) {
  if(!lua_isstring(state, 1))
    return luaL_error(state, "The argument must be a string.");

  const char* filename = lua_tostring(state, 1);

  char myBuf[2048] = {0};

  PHYSFS_file* myfile = PHYSFS_openRead(filename);
  if(!myfile)
    printf("%s %s \n", "No .lua ", filename);
  PHYSFS_sint64 fileLngth = PHYSFS_fileLength(myfile);
  PHYSFS_read (myfile, myBuf, 1, fileLngth);
  int status = 0;

  status = luaL_loadbuffer(state, (const char *)myBuf, fileLngth, filename) || lua_pcall(state, 0,0,0);
  if(status != 0) luaL_error(state,lua_tostring(state,-1));

  PHYSFS_close(myfile);

  switch (status)
    {
    case LUA_ERRMEM:
      return luaL_error(state, "Memory allocation error: %s\n", lua_tostring(state, -1));
    case LUA_ERRSYNTAX:
      return luaL_error(state, "Syntax error: %s\n", lua_tostring(state, -1));
    default: // success
      return 1;
    }

  return 1;
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

  const char * dir = l_tools_toStringOrError(state, 1);
  int error = PHYSFS_mount(dir, "/", 1);

  if (error != 1)
    return printf("%s %s %s \n", "Error: Directory or archive named : ", dir ," does not exist");

  char **rc = PHYSFS_enumerateFiles("/");
  char **i;
  int index = 1;

  if( n != 1 )
    return luaL_error(state, "Function s a single parameter.");

  lua_newtable(state);

  for (i = rc; *i != 0; i++)
    {
      lua_pushinteger(state, index);
      lua_pushstring(state, *i);
      lua_settable(state, -3);
      index++;
    }

  PHYSFS_freeList(rc);
  return 1;
}

static int l_filesystem_contain(lua_State* state)  {
  const char* a = l_tools_toStringOrError(state, 1);
  const char* b = l_tools_toStringOrError(state, 2);
  if ( filesystem_contain(a, b))
    lua_pushboolean(state, 1);
  else
    lua_pushboolean(state, 0);
  return 1;
}

static int l_filesystem_compare(lua_State* state)  {
  const char* a = l_tools_toStringOrError(state, 1);
  const char* b = l_tools_toStringOrError(state, 2);
  int l = l_tools_toNumberOrError(state, 3);
  if ( filesystem_compare(a, b, l))
    lua_pushboolean(state, 1);
  else
    lua_pushboolean(state, 0);
  return 1;
}

static int l_filesystem_getRealDirectory(lua_State* state) {
  if (!PHYSFS_isInit())
    return 0;

  const char *dir = PHYSFS_getRealDir(l_tools_toStringOrError(state,1));
  lua_pushstring(state, dir);
  return 1;
}

static int l_filesystem_isDirectory(lua_State* state) {
  if (!PHYSFS_isInit())
    return 0;

  const char* name = l_tools_toStringOrError(state, 1);
  if(PHYSFS_isDirectory(name))
    lua_pushboolean(state, 1);
  else
    lua_pushboolean(state, 0);
  return 1;
}

static int l_filesystem_isFile(lua_State* state) {
  if (!PHYSFS_isInit())
    return 0;

  const char* name = l_tools_toStringOrError(state, 1);
  if(!PHYSFS_isDirectory(name) && PHYSFS_exists(name))
    lua_pushboolean(state, 1);
  else
    lua_pushboolean(state, 0);
  return 1;
}

static luaL_Reg const regFuncs[] = {
  {"load", l_filesystem_load},
  {"getSaveDirectory", l_filesystem_getSaveDirectory},
  {"setSource",l_filesystem_setSource},
  {"requireDir",l_filesystem_set_requireDir},
  {"getSource", l_filesystem_getSource},
  {"require", l_filesystem_require},
  {"isFile", l_filesystem_isFile},
  {"isDirectory", l_filesystem_isDirectory},
  {"getRealDirectory", l_filesystem_getRealDirectory},
  {"enumerate",l_filesystem_enumerate},
  {"remove", l_filesystem_remove},
  {"read", l_filesystem_read},
  {"exists", l_filesystem_exists},
  {"write", l_filesystem_write},
  {"append", l_filesystem_append},
  {"compare", l_filesystem_compare},
  {"contain", l_filesystem_contain},
  {NULL, NULL}
};

int l_filesystem_register(lua_State* state) {
  l_tools_registerModule(state, "filesystem", regFuncs);
  return 0;
}
