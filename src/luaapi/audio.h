/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#pragma once

#include <stdbool.h>

#include "../3rdparty/lua/lua.h"
#include "../3rdparty/lua/lauxlib.h"
#include "../audio/audio.h"

#include "../audio/streamsource.h"
#include "../audio/staticsource.h"

typedef enum {
  audio_SourceType_static,
  audio_SourceType_stream
} audio_SourceType;

int l_audio_register(lua_State *state);
bool l_audio_isStaticSource(lua_State* state, int index);
bool l_audio_isStreamSource(lua_State* state, int index);
