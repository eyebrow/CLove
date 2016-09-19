/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "vorbis_decoder.h"

#include "../3rdparty/stb/stb_vorbis.h"
#include <stdlib.h>

static stb_vorbis *vorbis;

int audio_vorbis_load(ALuint buffer, char const *filename) {
  short *data;
  int channels;
  int samplingrate;
  int len = stb_vorbis_decode_filename(filename, &channels, &samplingrate, &data);

  if(len == -1) {
    return 0;
  }

  alBufferData(buffer, channels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, data,
               len * sizeof(short) * channels, samplingrate);

  stb_vorbis_close(vorbis);
  free(data);
  return 1;
}

void audio_vorbis_rewindStream(void *decoderData) {
  stb_vorbis_seek_start(vorbis);
}

int audio_vorbis_getChannelCount(void *decoderData) {
  stb_vorbis_info info = stb_vorbis_get_info((stb_vorbis*)decoderData);
  return info.channels;
}

int audio_vorbis_getSampleRate(void *decoderData) {
  stb_vorbis_info info = stb_vorbis_get_info((stb_vorbis*)decoderData);
  return info.sample_rate;
}

void audio_vorbis_flushBuffer(void *decoderData) {
  audio_vorbis_DecoderData * data = (audio_vorbis_DecoderData*)decoderData;
  data->preloadedSamples = 0;
}
