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

void audio_init(void);
void audio_setVolume(double value);
void audio_close(void);
