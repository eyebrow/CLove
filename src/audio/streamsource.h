
#pragma once 

#include <stdbool.h>
#include "../3rdparty/openal/include/AL/al.h"

#include "source.h"
#include "vorbis_decoder.h"

typedef struct {
	audio_vorbis_DecoderData* decoder;
  	ALuint source;
  	ALuint buffers[2];
  	bool   looping;
} audio_StreamSource;

bool audio_openStream(audio_StreamSource *source, char const * filename);
void audio_StreamSource_play(audio_StreamSource *source);
void audio_StreamSource_setLooping(audio_StreamSource *source, bool loop);
void audio_updateStreams(void);
void audio_streamsource_init(void);
