/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#pragma once

typedef struct {
  int major;
  int minor;
  int revision;
  char const * codename;
} love_Version;

love_Version const * love_getVersion(void);
