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
  stb_vorbis* vorbis;
  ALshort*    readBuffer;
  int         readBufferSize;
  int         preloadedSamples;
  int         channels;
  int         sample_rate;
} audio_vorbis_DecoderData;

int audio_vorbis_load(ALuint buffer, char const* filename); // static
int audio_vorbis_openStream(ALuint buffer, char const* filename); // stream
int audio_vorbis_takeStreamSamples(audio_vorbis_DecoderData* decoderData, int sampleCount); // stream
int audio_vorbis_uploadStreamSamples(audio_vorbis_DecoderData* decoderData, ALuint buffer); // stream
void audio_vorbis_rewindStream(audio_vorbis_DecoderData* decoderData);
int audio_vorbis_getChannelCount(audio_vorbis_DecoderData* decoderData);
int audio_vorbis_getSampleRate(audio_vorbis_DecoderData* decoderData);
void audio_vorbis_flushBuffer(audio_vorbis_DecoderData* decoderData);
