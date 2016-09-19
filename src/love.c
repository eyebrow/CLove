/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "love.h"

love_Version const * love_getVersion(void) {
  static const love_Version version = {
    .major = 0,
    .minor = 5,
    .revision = 2,
    .codename = "TouchingTheLimits"
  };

  return &version;
}
