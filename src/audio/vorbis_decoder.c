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

bool audio_vorbis_load(ALuint buffer, char const *filename) {
  	short *data;
  	int channels;
  	int samplingrate;
  	int len = stb_vorbis_decode_filename(filename, &channels, &samplingrate, &data);
	
	audio_vorbis_DecoderData* decoder = malloc(sizeof(audio_vorbis_DecoderData));
	decoder->channels = channels;
	decoder->sample_rate = samplingrate;	
	

  	if(len == -1) {
    	return false;
  	}

  	alBufferData(buffer, channels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, data,
         len * sizeof(ALushort) * channels, samplingrate);

  	free(data);
  	return true;
}

bool audio_vorbis_openStream(audio_vorbis_DecoderData* decoder, char const *filename) { 
	int err;
	decoder->vorbis = stb_vorbis_open_filename(filename, &err, NULL);

   stb_vorbis_info info = stb_vorbis_get_info(decoder->vorbis);
	
	decoder->readBufferSize = info.channels * info.sample_rate + 4096;
	decoder->readBuffer = malloc(sizeof(ALshort) * decoder->readBufferSize);
	decoder->preloadedSamples = 0;

 	//stb_vorbis_close(decoder->vorbis);
  	return true;
}

/*
  must return:  	 
  0 if end of stream reached
  -1 if buffer is full
   n where n > 0 if n samples were preloaded
*/
int audio_vorbis_takeStreamSamples(audio_vorbis_DecoderData* decoderData, int sampleCount) {

	int safeBufferSize = sampleCount * decoderData->channels * 4096;
	if (safeBufferSize > decoderData->readBufferSize) {
		decoderData->readBufferSize = safeBufferSize;
		free(decoderData->readBuffer);
		decoderData->readBuffer = malloc(sizeof(ALshort) * safeBufferSize);
	}	
	int space = decoderData->readBufferSize - decoderData->preloadedSamples - 4096;
  	if(space <= 0) {
    	return -1;
  	}

  	if(space < sampleCount) {
    	sampleCount = space;
  	}

  	int readSamples = 0;
  	while(readSamples < sampleCount) {
    	float **channelData;
    	int samples = stb_vorbis_get_frame_float(decoderData->vorbis, NULL, &channelData);
    	if(samples == 0) {
      	break;
    	}
    	for(int i = 0; i < samples; ++i) {
      	for(int c = 0; c < decoderData->channels; ++c) {
        		decoderData->readBuffer[decoderData->preloadedSamples + readSamples + decoderData->channels * i + c] 
			  		= (ALshort)(channelData[c][i] * 32767);
      	}
    	}

    	readSamples += decoderData->channels * samples;
  	}

  	decoderData->preloadedSamples += readSamples;

	return readSamples;
}

int audio_vorbis_uploadStreamSamples(audio_vorbis_DecoderData* decoderData, ALuint buffer) {
	
	stb_vorbis_info info = stb_vorbis_get_info(decoderData->vorbis);
	int channels = info.channels >= 2 ? 2 : 1; 

	if(decoderData->preloadedSamples < decoderData->readBufferSize / 2) 
   	 audio_vorbis_takeStreamSamples(decoderData, decoderData->readBufferSize / 2);
  	

	alBufferData(buffer, channels == 2 ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16, decoderData->readBuffer,
         decoderData->preloadedSamples * sizeof(ALshort),	info.sample_rate);

	decoderData->preloadedSamples = 0;
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

void audio_static_vorbis_flushBuffer(audio_vorbis_DecoderData* decoderData) {
 	decoderData->preloadedSamples = 0;
}
