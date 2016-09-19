/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#pragma once

#include "graphics.h"

typedef enum {
  graphics_DrawMode_fill,
  graphics_DrawMode_line
}graphics_DrawMode;

void graphics_geometry_init();

void graphics_geometry_fillRectangle(int type, float x, float y,
                                     float w, float h,
                                     float rotation,
                                     float sx, float sy,
                                     float ox, float oy);

void graphics_geometry_fillCircle(float x,float y, float radius, int segments);
void graphics_geometry_drawCircle(float x,float y, float radius, int segments);

void graphics_geometry_vertex(int type, float x, float y, int vertices[], int count);
void graphics_geometry_points(float x, float y);

void graphics_geometry_free();

void graphics_geometry_setLineWidth(float width);
float graphics_geometry_getLineWidth();
