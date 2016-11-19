/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "staticsource.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// loaders
#include "wav_decoder.h"
#include "vorbis_decoder.h"

static const char* get_filename_ext(const char *filename) {
  const char *dot = strrchr(filename, '.');
  if(!dot || dot == filename) return "";
  return dot+1;
}

int audio_loadStatic(audio_StaticSource *source, char const * filename) {
  	int err = 1;

	audio_SourceCommon_init(&source->common);
  	alGenBuffers(1, &source->buffer);
  
  	if(strncmp(get_filename_ext(filename),"wav", 3) == 0){
   	 err = audio_wav_load(source->buffer, filename);
  	}else if((strncmp(get_filename_ext(filename), "ogg", 3)) == 0){
		 err = audio_vorbis_openStream(source->buffer, filename);
  	}else 
   	 err = -1; //Unknow file type :(
  
  	alSourcei(source->common.source, AL_BUFFER, source->buffer);
 	 return err;
}

void audio_StaticSource_play(audio_StaticSource *source) {
  if(source->common.state != audio_SourceState_playing) {
    audio_SourceCommon_play(&source->common);
  }
}

void audio_StaticSource_setLooping(audio_StaticSource *source, bool loop) {
  alSourcei(source->common.source, AL_LOOPING, loop);
}

void audio_StaticSource_stop(audio_StaticSource *source) {
  audio_SourceCommon_stop(&source->common);
  audio_StaticSource_rewind(source);
}

void audio_StaticSource_rewind(audio_StaticSource *source) {
  alSourceRewind(source->common.source);

  if(source->common.state == audio_SourceState_playing) {
    audio_SourceCommon_play(&source->common);
  }
}

void audio_StaticSource_pause(audio_StaticSource *source) {
  audio_SourceCommon_pause(&source->common);
}


void audio_StaticSource_resume(audio_StaticSource *source) {
  audio_SourceCommon_resume(&source->common);
}
