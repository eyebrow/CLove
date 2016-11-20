/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include <stdlib.h>
#include "streamsource.h"

// loaders
#include "wav_decoder.h"
#include "vorbis_decoder.h"

#include "source.h"

static struct {
	audio_StreamSource **playingStreams;
  	int playingStreamSize;
	int playingStreamCount;
} moduleData;

void audio_streamsource_init(void) {
	moduleData.playingStreamCount = 0;
  	moduleData.playingStreamSize  = 16;
	moduleData.playingStreams = malloc(sizeof(audio_StreamSource*) * 16);
}

bool audio_openStream(audio_StreamSource* source, char const * filename) {

	source->decoder = malloc(sizeof(audio_vorbis_DecoderData));
	bool ok = audio_vorbis_openStream(source->decoder, filename); 
	if (!ok) {
		printf("%s %s \n","Could not open stream: ", filename);
		return false;
	}

	//audio_SourceCommon_init(&source->common);
  	alGenSources(1, &source->source);
  	alSourcef( source->source, AL_PITCH,    1.0f);
  	alSourcef( source->source, AL_GAIN,     1.0f);
  	alSource3f(source->source, AL_POSITION, 0.0f, 0.0f, 0.0f);
  	alSource3f(source->source, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	alSourcei( source->source, AL_LOOPING, AL_FALSE);

	alGenBuffers(2, source->buffers);

	for(int i = 0; i < 2; ++i) {
    	audio_vorbis_takeStreamSamples(source->decoder, 44100);
    	audio_vorbis_uploadStreamSamples(source->decoder, source->buffers[i]);
  	}

	source->looping = true;
	return ok;
}

void audio_StreamSource_play(audio_StreamSource* source) {

	printf("called3\n");
  	alSourceQueueBuffers(source->source, 2, source->buffers);

  	if(moduleData.playingStreamCount == moduleData.playingStreamSize) {
    	moduleData.playingStreamSize = 2 * moduleData.playingStreamSize;
    	moduleData.playingStreams = realloc(moduleData.playingStreams, moduleData.playingStreamSize*sizeof(audio_StreamSource*));
  	}

  	moduleData.playingStreams[moduleData.playingStreamCount] = source;
  	++moduleData.playingStreamCount;


	alSourcePlay(source->source);
}

void audio_StreamSource_setLooping(audio_StreamSource* source, bool loop) {
	source->looping = loop; 
}

void audio_updateStreams(void) { 
	//printf("%d \n", moduleData.playingStreamCount);

	for(int i = 0; i < moduleData.playingStreamCount; ++i) {
    	audio_StreamSource const* source = moduleData.playingStreams[i];

		int loaded = audio_vorbis_takeStreamSamples(source->decoder, 8000);
    	if(loaded == 0) {
      	// We don't read anything,continue next frame
      	if (source->looping)
				audio_vorbis_rewindStream(source->decoder);
    	}

    	ALuint src = source->source;
    	ALint count;
    	ALint queued;
    	ALint state;
    	alGetSourcei(src, AL_BUFFERS_PROCESSED, &count);
    	alGetSourcei(src, AL_BUFFERS_QUEUED, &queued);
    	alGetSourcei(src, AL_SOURCE_STATE, &state);
  		printf("%d buffers free, %d queued, state=%d\n", count, queued, state);

		for(int j = 0; j < count; ++j) {
   		ALuint buf;
      	alSourceUnqueueBuffers(src, 1, &buf);
      	int uploaded = audio_vorbis_uploadStreamSamples(source->decoder, buf);
      	if (uploaded)
				alSourceQueueBuffers(src, 1, &buf);
		}

		alGetSourcei(src, AL_BUFFERS_QUEUED, &queued);
    	if(state == AL_STOPPED && queued == 0) {
      	--moduleData.playingStreamCount;
      	moduleData.playingStreams[i] = moduleData.playingStreams[moduleData.playingStreamCount];
    	} else {
      	++i;
    	}
	}

}

