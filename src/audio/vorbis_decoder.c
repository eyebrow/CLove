/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "vorbis_decoder.h"

#include <stdlib.h>

int audio_vorbis_load(audio_vorbis_DecoderData* decoder, ALuint buffer, char const *filename) {
  /*short *data;
  int channels;
  int samplingrate;
  int len = stb_vorbis_decode_filename(filename, &channels, &samplingrate, &data);

  if(len == -1) {
    return 0;
  }
	*/
	int err;
 	decoder->vorbis = stb_vorbis_open_filename(filename, &err, NULL);

   stb_vorbis_info info = stb_vorbis_get_info(decoder->vorbis);
	decoder->channels = info.channels;
	decoder->sample_rate = info.sample_rate;	

	decoder->preloadedSamples = 4096 * 10;
	
	decoder->readBufferSize    = info.channels * info.sample_rate + 4096 * 10;
	decoder->readBuffer = malloc(sizeof(ALshort) * decoder->readBufferSize);

  alBufferData(buffer, info.channels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, decoder->readBuffer,
               decoder->preloadedSamples * sizeof(short) * info.channels,//len * sizeof(short) * channels, 
					decoder->sample_rate);

  stb_vorbis_close(decoder->vorbis);
  //free(data);
  return 1;
}

void audio_vorbis_rewindStream(audio_vorbis_DecoderData* decoderData) {
  stb_vorbis_seek_start(decoderData->vorbis);
}

int audio_vorbis_getChannelCount(audio_vorbis_DecoderData* decoderData) {
  stb_vorbis_info info = stb_vorbis_get_info((stb_vorbis*)decoderData);
  return info.channels;
}

int audio_vorbis_getSampleRate(audio_vorbis_DecoderData* decoderData) {
 return decoderData->sample_rate;
}

void audio_vorbis_flushBuffer(audio_vorbis_DecoderData* decoderData) {
 	decoderData->preloadedSamples = 0;
}
