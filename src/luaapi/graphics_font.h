/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#pragma once

#include "../3rdparty/lua/lua.h"
#include "../graphics/font.h"
#include <stdbool.h>

void l_graphics_font_register(lua_State* state);
bool l_graphics_isFont(lua_State* state, int index);
int l_graphics_newFont(lua_State* state);
graphics_Font* l_graphics_toFont(lua_State* state, int index);
