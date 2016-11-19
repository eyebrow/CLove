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
#include "../3rdparty/stb/stb_vorbis.h"

typedef struct {
  stb_vorbis *vorbis;
  ALshort    *readBuffer;
  int         readBufferSize;
  int         preloadedSamples;
  int         channels;
  int         sample_rate;
} audio_vorbis_DecoderData;

int audio_vorbis_load(audio_vorbis_DecoderData* decoder, ALuint buffer, char const *filename);
void audio_vorbis_rewindStream(audio_vorbis_DecoderData* decoderData);
int audio_vorbis_getChannelCount(audio_vorbis_DecoderData* decoderData);
int audio_vorbis_getSampleRate(audio_vorbis_DecoderData* decoderData);
void audio_vorbis_flushBuffer(audio_vorbis_DecoderData* decoderData);
