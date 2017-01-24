/*
#   clove
#
#   Copyright (C) 2016-2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/#pragma once

#include <math.h>
#include "image.h"
#include "quad.h"

#include "../3rdparty/freetype/include/ft2build.h"
#include "../3rdparty/freetype/include/freetype.h"
#include "../3rdparty/freetype/include/ftglyph.h"

typedef struct {
  unsigned int textureid;
  float sizex;
  float sizey;
  float bearingx;
  float bearingy;
  unsigned int advancex;
  unsigned int advancey;
} character;

typedef struct {
  GLuint vbo;
  GLuint ibo;
  GLuint tex;
  int size;
  FT_Face face;
  FT_GlyphSlot glyph;
  FT_Bitmap bitmap;
  char const* text;
  const char* path;
  character characters[300];
  character ch;
} graphics_Font;


void graphics_Font_setFilter(graphics_Font *font, graphics_Filter const* filter);
void graphics_Font_getFilter(graphics_Font *font, graphics_Filter *filter);
void graphics_Font_setWrap(graphics_Font *font, graphics_Wrap const* wrap);
void graphics_Font_getWrap(graphics_Font *font, graphics_Wrap *wrap);

int graphics_font_init(void);

int graphics_Font_new(graphics_Font* font, char const* filename, int ptsize);
int graphics_Font_load(graphics_Font* font);

void graphics_font_freeFT();
void graphics_Font_free(graphics_Font* font);

int graphics_Font_getHeight(graphics_Font* font, char const* line);
int graphics_Font_getWidth(graphics_Font* font, char const* line);
int graphics_Font_getAscent(graphics_Font* font);
int graphics_Font_getDescent(graphics_Font* font);
int graphics_Font_getBaseline(graphics_Font* font);

typedef enum {
  graphics_TextAlign_center,
  graphics_TextAlign_left,
  graphics_TextAlign_right,
  graphics_TextAlign_justify
} graphics_TextAlign;

void graphics_Font_print(graphics_Font* font, char const* text, int x, int y, float r, float sx, float sy, float ox, float oy, float kx, float ky);
void graphics_Font_printf(graphics_Font* font, char const* text, int px, int py, int limit, graphics_TextAlign align, float r, float sx, float sy, float ox, float oy, float kx, float ky);

void graphics_Font_setFilter(graphics_Font *font, graphics_Filter const* filter);
void graphics_Font_getFilter(graphics_Font *font, graphics_Filter *filter);
