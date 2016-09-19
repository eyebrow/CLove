/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#pragma once

#include "../3rdparty/openal/include/AL/al.h"

typedef struct {
  ALshort    *readBuffer;
  int         readBufferSize;
  int         preloadedSamples;
} audio_vorbis_DecoderData;

int audio_vorbis_load(ALuint buffer, char const *filename);
void audio_vorbis_rewindStream(void *decoderData);
int audio_vorbis_getChannelCount(void *decoderData);
int audio_vorbis_getSampleRate(void *decoderData);
void audio_vorbis_flushBuffer(void *decoderData);
