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
#include "../3rdparty/openal/include/AL/al.h"

#include "source.h"
#include "vorbis_decoder.h"

typedef struct {
  audio_vorbis_DecoderData* decoder;
  //audio_SourceCommon common;
  ALuint buffers[2];
  ALuint source;
  bool looping;
} audio_StreamSource;

void audio_streamsource_init(void);
bool audio_openStream(audio_StreamSource *source, char const * filename);
void audio_StreamSource_play(audio_StreamSource *source);
void audio_StreamSource_setLooping(audio_StreamSource *source, bool loop);
void audio_updateStreams(void);
