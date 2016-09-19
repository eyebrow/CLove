/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#pragma once

inline int max(int i, int j) {
  return i > j ? i : j;
}

inline int min(int i, int j) {
  return i < j ? i : j;
}

inline int clamp(int a, int i, int k) {
  return a > k ? k : i ? i : a;
}
