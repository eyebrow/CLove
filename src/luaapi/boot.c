/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "../3rdparty/lua/lauxlib.h"

#include "boot.h"

static char const bootScript[] =
  "package.path = '?.lua;?/init.lua'\n"
  "love.update = function(dt) if love.keyboard.isDown('esc') then love.event.quit() end end\n"
  "love.draw = function() end \n"
  "love.load = function() end \n"
  "love.mousepressed = function() end\n"
  "love.mousereleased = function() end\n"
  "love.keypressed = function() end\n"
  "love.keyreleased = function() end\n"
  "love.quit = function() end\n"
  "love.textinput = function() end\n"
  "love.wheelmoved = function() end\n"
  "local conf = {\n"
  "  window = {\n"
  "    width = 800,\n"
  "    height = 600\n"
  "  }\n"
  "}\n"
  "local confFunc = loadfile(\"conf.lua\")\n"
  "if confFunc then\n"
  "  confFunc()\n"
  "  love.conf(conf)\n"
  "end\n"
  "return conf\n"
;

int l_boot(lua_State* state, love_Config *config) {
  if(luaL_dostring(state, bootScript)) {
    return 1;
  }

  lua_pushstring(state, "window");
  lua_rawget(state, -2);

  lua_pushstring(state, "width");
  lua_rawget(state, -2);

  config->window.width = lua_tointeger(state, -1);
  lua_pop(state, 1);

  lua_pushstring(state, "height");
  lua_rawget(state, -2);
  config->window.height = lua_tointeger(state, -1);

  lua_pop(state, 3);

  return 0;
}

static char const no_game_Script[] =
  "package.path = '?.lua;?/init.lua'\n"
  "love.update = function(dt) if love.keyboard.isDown('esc') then love.event.quit end  end\n"
  "love.draw = function() love.graphics.setBackgroundColor(0x84, 0xca, 0xff) "
  "love.graphics.print('No main.lua found',love.window.getWidth()/2 - 190,love.window.getHeight()/2) end \n"
  "love.load = function() end \n"
  "love.mousepressed = function() end\n"
  "love.mousereleased = function() end\n"
  "love.keypressed = function() end\n"
  "love.keyreleased = function() end\n"
  "love.quit = function() end\n"
  "love.textinput = function() end\n"
  "love.wheelmoved = function() end\n"
  "local conf = {\n"
  "  window = {\n"
  "    width = 800,\n"
  "    height = 600\n"
  "  }\n"
  "}\n"
  "local confFunc = loadfile(\"conf.lua\")\n"
  "if confFunc then\n"
  "  confFunc()\n"
  "  love.conf(conf)\n"
  "end\n"
  "return conf\n"
;

int l_no_game(lua_State* state, love_Config *config) {
  if(luaL_dostring(state, no_game_Script)) {
    return 1;
  }

  lua_pushstring(state, "window");
  lua_rawget(state, -2);

  lua_pushstring(state, "width");
  lua_rawget(state, -2);

  config->window.width = lua_tointeger(state, -1);
  lua_pop(state, 1);

  lua_pushstring(state, "height");
  lua_rawget(state, -2);
  config->window.height = lua_tointeger(state, -1);

  lua_pop(state, 3);

  return 0;
}
