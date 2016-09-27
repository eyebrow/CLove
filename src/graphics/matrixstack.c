/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "../math/vector.h"
#include "matrixstack.h"
#include <string.h>

#include <stdlib.h>
#include <stdio.h>

const static int stackSize = 32;

static struct {
  int head;
#if _WIN32 || _WIN64
  mat4x4* stack;
#else
  mat4x4 stack[stackSize];
#endif

} moduleData;

inline mat4x4* matrixstack_head() {
  return &moduleData.stack[moduleData.head];
}

void matrixstack_init(void) {
#if _WIN32 || _WIN64
  moduleData.stack = malloc(sizeof(stackSize));
#endif
  moduleData.head = 0;
  m4x4_newIdentity(matrixstack_head());
}

int matrixstack_push(void) {
  if(moduleData.head == stackSize - 1) {
      return 1;
    }

  memcpy(matrixstack_head() + 1, matrixstack_head(), sizeof(mat4x4));
  ++moduleData.head;
  return 0;
}

void matrixstack_origin(void) {
  m4x4_newIdentity(matrixstack_head());
}

int matrixstack_pop(void) {
  if(moduleData.head == 0) {
      return 1;
    }

  --moduleData.head;
  return 0;
}

void matrixstack_translate(float x, float y) {
  m4x4_translate(matrixstack_head(), x, y, 0.0f);
}

void matrixstack_scale(float x, float y) {
 m4x4_scale(matrixstack_head(), x, y, 0.0f);
}

void matrixstack_rotate(float a) {
  m4x4_rotateZ(matrixstack_head(), a);
}

void matrixstack_multiply(mat4x4 const* matrix) {
  mat4x4 m;
  memcpy(&m, matrixstack_head(), sizeof(mat4x4));
  m4x4_mulM4x4(matrixstack_head(), matrix, &m);
}

