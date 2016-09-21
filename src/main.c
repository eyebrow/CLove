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
#include <sys/stat.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "3rdparty/lua/lua.h"
#include "3rdparty/lua/lauxlib.h"
#include "3rdparty/lua/lualib.h"
#include "3rdparty/SDL2/include/SDL.h"
#include "luaapi/audio.h"
#include "luaapi/mouse.h"
#include "luaapi/event.h"
#include "luaapi/graphics.h"
#include "luaapi/graphics_font.h"
#include "luaapi/image.h"
#include "luaapi/love.h"
#include "luaapi/boot.h"
#include "luaapi/keyboard.h"
#include "luaapi/mouse.h"
#include "luaapi/filesystem.h"
#include "luaapi/timer.h"
#include "luaapi/math.h"
#include "luaapi/system.h"
#include "tools/utils.c"
#include "love.h"

#include "graphics/graphics.h"
#include "filesystem/filesystem.h"
#include "audio/audio.h"
#include "keyboard.h"
#include "mouse.h"
#include "timer/timer.h"

#include "3rdparty/physfs/physfs.h"

int lua_errorhandler(lua_State *state) {
  lua_Debug debug;
  int level = 0;
  while(lua_getstack(state, level, &debug)) {
      lua_getinfo(state, "Sl", &debug);
      lua_pushstring(state, debug.short_src);
      lua_pushstring(state, ":");
      lua_pushnumber(state, debug.currentline);
      lua_pushstring(state, "\n");
      ++level;
    }
  
  lua_concat(state, 4*level+1);
  return 1;
}

typedef struct {
  lua_State *luaState;
  int errhand;
} MainLoopData;

void quit_function(lua_State* state)
{
  lua_getglobal(state, "love");
  lua_pushstring(state, "quit");
  lua_rawget(state, -2);
  if(lua_pcall(state, 0, 0, 1)) {
      printf("Error in love.quit: %s\n", lua_tostring(state, -1));
    }
  lua_pop(state, 1);
}

void main_loop(void *data) {
  MainLoopData* loopData = (MainLoopData*)data;
  
  timer_step();
  lua_rawgeti(loopData->luaState, LUA_REGISTRYINDEX, loopData->errhand);
  lua_getglobal(loopData->luaState, "love");
  lua_pushstring(loopData->luaState, "update");

  lua_rawget(loopData->luaState, -2);
  lua_pushnumber(loopData->luaState, timer_getDelta());

  if (swap_At == 1){
      if(luaL_dofile(loopData->luaState, "main.lua")) {
          printf("Error: %s\n", lua_tostring(loopData->luaState, -1));
        }
    }

  if(lua_pcall(loopData->luaState, 1, 0, 0)) {
      printf("Lua error: %s\n", lua_tostring(loopData->luaState, -1));
#ifdef EMSCRIPTEN
      quit_function(loopData->luaState);
      emscripten_force_exit(1);
#else
      exit(1);
#endif
    }

  graphics_clear();
  
  lua_pushstring(loopData->luaState, "draw");
  lua_rawget(loopData->luaState, -2);

  if(lua_pcall(loopData->luaState, 0, 0, 0)) {
      printf("Lua error: %s\n", lua_tostring(loopData->luaState, -1));
#ifdef EMSCRIPTEN
      quit_function(loopData->luaState);
      emscripten_force_exit(1);
      l_running = 0;
#else
      l_running = 0;
#endif
    }

  graphics_swap();

  lua_pop(loopData->luaState, 1);

  SDL_Event event;
  while(SDL_PollEvent(&event)) {
      if (event.type == SDL_WINDOWEVENT) {
          switch (event.window.event) {
            case SDL_WINDOWEVENT_ENTER:
              graphics_setMouseFocus(1);
              break;
            case SDL_WINDOWEVENT_LEAVE:
              graphics_setMouseFocus(0);
              break;
            case SDL_WINDOWEVENT_FOCUS_LOST:
              graphics_setFocus(0);
              break;
            case SDL_WINDOWEVENT_FOCUS_GAINED:
              graphics_setFocus(1);
              break;
            default:
              break;
            }
        }
      switch(event.wheel.type)
        {
        case SDL_MOUSEWHEEL:
          mouse_mousewheel(event.wheel.y);
          int _what = event.wheel.y == 1 ? SDL_BUTTON_WHEEL_UP : SDL_BUTTON_WHEEL_DOWN;
          mouse_mousepressed(event.button.x, event.button.y,
                             _what);
          break;
        default:
          break;
        }
      switch(event.type) {
        case SDL_KEYDOWN:
          keyboard_keypressed(event.key.keysym.sym);
          break;
        case SDL_KEYUP:
          keyboard_keyreleased(event.key.keysym.sym);
          break;
        case SDL_TEXTINPUT:
          keyboard_textInput(event.text.text);
          break;
        case SDL_MOUSEMOTION:
          mouse_mousemoved(event.motion.x, event.motion.y);
          break;
        case SDL_MOUSEBUTTONDOWN:
          mouse_mousepressed(event.button.x, event.button.y,
                             event.button.button);
          break;
        case SDL_MOUSEBUTTONUP:
          mouse_mousereleased(event.button.x, event.button.y,
                              event.button.button);
          break;
#ifndef EMSCRIPTEN
        case SDL_QUIT:
          quit_function(loopData->luaState);
          l_running = 0;
#endif
        }
    }
}

int main(int argc, char* argv[]) {
  lua_State *lua = luaL_newstate();
  luaL_openlibs(lua);

  love_Config config;

  l_love_register(lua);
  l_audio_register(lua);
  l_event_register(lua);
  l_graphics_register(lua);
  l_image_register(lua);
  l_keyboard_register(lua);
  l_mouse_register(lua);
  l_filesystem_register(lua);
  l_timer_register(lua);
  l_math_register(lua);
  l_system_register(lua);

  l_boot(lua, &config);

  keyboard_init();
  graphics_init(config.window.width, config.window.height);
  l_running = 1;
  audio_init();

  bool use_argv = false;
  PHYSFS_file* myfile;
  char myBuf[2048] = {0};   //TODO check too see when this fails(if ever)
#ifdef CLOVE_MACOSX || CLOVE_LINUX
  PHYSFS_init(argv[1]);
#elif CLOVE_WINDOWS
  PHYSFS_init(NULL);
#endif
  use_argv = true;
  if(PHYSFS_addToSearchPath(argv[1], 1)){
      if(PHYSFS_exists(argv[2]))
        {
          myfile = PHYSFS_openRead(argv[2]);
          PHYSFS_sint64 fileLngth = PHYSFS_fileLength(myfile);
          PHYSFS_read (myfile, myBuf, 1, fileLngth);
          if(luaL_loadbuffer(lua, myBuf, strlen(myBuf), "line") || lua_pcall(lua, 0, 0, 0)){
              printf("Error loading from argv: %s\n", lua_tostring(lua, -1));
              l_no_game(lua,&config);
            }

        }
    }
  else // look for main.lua ;)
    {
      if(luaL_dofile(lua,"main.lua")){
          printf("Error: %s\n", lua_tostring(lua, -1));
          l_no_game(lua,&config);
        }
    }


  love_Version const * version = love_getVersion();
  printf("%s %s %d.%d.%d \n", "Love version name: ",version->codename,version->major,version->minor,version->revision);

  lua_pushcfunction(lua, lua_errorhandler);
  lua_getglobal(lua, "love");
  lua_pushstring(lua, "load");
  lua_rawget(lua, -2);
  if(lua_pcall(lua, 0, 0, 1)) {
      printf("Error in love.load: %s\n", lua_tostring(lua, -1));
    }
  lua_pop(lua, 1);

  lua_pushcfunction(lua, lua_errorhandler);
  MainLoopData mainLoopData = {
    .luaState = lua,
    .errhand = luaL_ref(lua, LUA_REGISTRYINDEX)
  };

  timer_init();

#ifdef EMSCRIPTEN
  //TODO find a way to quit(love.event.quit) love on web?
  emscripten_set_main_loop_arg(main_loop, &mainLoopData, 0, 1);
#else
  while(l_event_running()) {
      main_loop(&mainLoopData);
    }
  if(!l_event_running())
    quit_function(lua);
#endif
  graphics_destroyWindow();
  audio_close ();
  lua_close(lua);

  PHYSFS_close(myfile);
  if(use_argv)
    PHYSFS_deinit();
  return 1;
}
