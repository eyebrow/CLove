/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include <stdio.h>

#include "audio.h"
#include "../3rdparty/openal/include/AL/al.h"
#include "../3rdparty/openal/include/AL/alc.h"

static struct {
  ALCdevice  *device;
  ALCcontext *context;
} moduleData;

void audio_init(void) {
  moduleData.device = alcOpenDevice(0);
  moduleData.context = alcCreateContext(moduleData.device, 0);
  if(!alcMakeContextCurrent(moduleData.context)) {
    printf("%s \n", "Failed to initialite audio context");
  }
}

void audio_setVolume(double value){
  alListenerf(AL_GAIN, value);
}

void audio_close () {
  alcDestroyContext(moduleData.context);
  alcCloseDevice(moduleData.device);
}
