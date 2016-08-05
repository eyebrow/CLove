/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include <stdlib.h>
#include "../math/util.h"
#include <tgmath.h>
#include <stdlib.h>
#include "geometry.h"
#include "graphics.h"
#include "shader.h"
#include "matrixstack.h"
#include "vertex.h"
#include <stdio.h>

static struct {
  GLuint dataVBO;
  GLuint dataIBO;
  int currentDataSize;
  int currentIndexSize;
  float *data;
  uint16_t *index;
  graphics_Shader plainColorShader;
  float lineWidth;
} moduleData;

void graphics_geometry_init(void) {

  glGenBuffers(1, &moduleData.dataVBO);
  glBindBuffer(GL_ARRAY_BUFFER, moduleData.dataVBO);

  glGenBuffers(1, &moduleData.dataIBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, moduleData.dataIBO);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), 0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(2*sizeof(float)));
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(4*sizeof(float)));

  graphics_Shader_new(&moduleData.plainColorShader, NULL,
                      "vec4 effect(vec4 color, Image texture, vec2 texture_coords, vec2 screen_cords ) {\n"
                      "  return color;\n"
                      "}\n");

  moduleData.lineWidth = 1.0f;
}

void graphics_geometry_free () {
  glDeleteBuffers(1, &moduleData.dataIBO);
  glDeleteBuffers(1, &moduleData.dataVBO);
  graphics_Shader_free(&moduleData.plainColorShader);
}
static void growBuffers(int vertices, int indices) {
  int datasize = (vertices) * 8 * sizeof(float);
  if(moduleData.currentDataSize < datasize) {
      free(moduleData.data);
      moduleData.data = (float*)malloc(datasize);
      moduleData.currentDataSize = datasize;
    }

  int indexsize = (indices) * sizeof(uint16_t);
  if(moduleData.currentIndexSize < indexsize) {
      free(moduleData.index);
      moduleData.index = (uint16_t*)malloc(indexsize);
      moduleData.currentIndexSize = indexsize;
    }
}

static void drawBuffer(int vertices, int indices, GLenum type) {
  glBindBuffer(GL_ARRAY_BUFFER, moduleData.dataVBO);
  glBufferData(GL_ARRAY_BUFFER, moduleData.currentDataSize, moduleData.data, GL_STREAM_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, moduleData.dataIBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, moduleData.currentIndexSize, moduleData.index, GL_STREAM_DRAW);


  graphics_Shader *shader = graphics_getShader();
  graphics_setShader(&moduleData.plainColorShader);

  mat4x4 tr;
  m4x4_newIdentity(&tr);
  graphics_Quad quad = {0,0,1,1};

  graphics_drawArray(&quad, &tr, moduleData.dataIBO, indices,
                     type, GL_UNSIGNED_SHORT, graphics_getColor(), 1, 1);

  graphics_setShader(shader);
}

static void drawBufferSpecial(int vertices, int indices,
			      float x, float y, float z,
			      float r, float rx, float ry, float rz,
			      float w, float h,
			      float sx, float sy, float sz,
			      float ox, float oy, GLenum type) {
  glBindBuffer(GL_ARRAY_BUFFER, moduleData.dataVBO);
  glBufferData(GL_ARRAY_BUFFER, moduleData.currentDataSize, moduleData.data, GL_STREAM_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, moduleData.dataIBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, moduleData.currentIndexSize, moduleData.index, GL_STREAM_DRAW);

  graphics_Shader *shader = graphics_getShader();
  graphics_setShader(&moduleData.plainColorShader);

  mat4x4 tr;
  m4x4_newTransform3d(&tr, vec3_new(x, y, z), r, vec3_new(rx, ry, rz), vec3_new(sx, sy, sz), ox, oy, 0, 0);
  graphics_Quad quad = {0,0,1,1};

  graphics_drawArray(&quad, &tr, moduleData.dataIBO, indices,
                     type, GL_UNSIGNED_SHORT, graphics_getColor(), w, h);

  graphics_setShader(shader);
}

void graphics_geometry_drawCircle(float x, float y, float radius, int segments) {
  growBuffers(segments+1,segments+2);

  float step = 2*LOVE_PI / segments;
  float ang = 0;
  float* buf = moduleData.data;

  moduleData.index[0] = 1;
  moduleData.index[segments+1] = 1;
  for(int i = 0; i < segments; ++i, ang -= step) {
      buf[8*(i+1)+0] = sin(ang) * radius + x;
      buf[8*(i+1)+1] = cos(ang) * radius + y;

      buf[8*(i+1)+2] = 0.0f;
      buf[8*(i+1)+3] = 0.0f;

      buf[8*(i+1)+4] = 1.0f;
      buf[8*(i+1)+5] = 1.0f;
      buf[8*(i+1)+6] = 1.0f;
      buf[8*(i+1)+7] = 1.0f;
      moduleData.index[i+1] = i+1;
    }
  drawBuffer(segments+1,segments+2,GL_LINE_STRIP);
}


void graphics_geometry_fillCircle(float x, float y, float radius, int segments) {
  growBuffers(segments+1, segments+2);

  float step = 2*LOVE_PI / segments;
  float ang = 0;
  float* buf = moduleData.data;

  buf[0] = x;
  buf[1] = y;
  buf[2] = 0.0f;
  buf[3] = 0.0f;
  buf[4] = 1.0f;
  buf[5] = 1.0f;
  buf[6] = 1.0f;
  buf[7] = 1.0f;

  moduleData.index[0] = 0;
  moduleData.index[segments+1] = 1;
  for(int i = 0; i < segments; ++i, ang -= step) {
      buf[8*(i+1)+0] = sin(ang) * radius + x;
      buf[8*(i+1)+1] = cos(ang) * radius + y;

      buf[8*(i+1)+2] = 0.0f;
      buf[8*(i+1)+3] = 0.0f;

      buf[8*(i+1)+4] = 1.0f;
      buf[8*(i+1)+5] = 1.0f;
      buf[8*(i+1)+6] = 1.0f;
      buf[8*(i+1)+7] = 1.0f;
      moduleData.index[i+1] = i+1;
    }
  drawBuffer(segments+1, segments+2, GL_TRIANGLE_FAN);
}

void graphics_geometry_fillRectangle(int type,
                                     float x, float y, float z,
                                     float w, float h,
                                     float rotation, float rx, float ry, float rz,
                                     float sx, float sy, float sz,
                                     float ox, float oy) {

  growBuffers(32, 7);

  bool special = rotation != 0 || sx != 1 || sy != 1;

  float *buf = moduleData.data;

  if(!special) {
      buf[0] = x + w * sx; //0
      buf[1] = y;

      buf[8] = x; //1
      buf[9] = y;

      buf[16] = x; //2
      buf[17] = y + h * sy;

      buf[24] = x + w * sx; //3
      buf[25] = y + h * sy;
    }else {

      buf[0] = 1.0f; //0
      buf[1] = 0.0f;

      buf[8] = 0.0f; // 1
      buf[9] = 0.0f;

      buf[16] = 0.0f;// 2
      buf[17] = 1.0f;

      buf[24] = 1.0f; // 3
      buf[25] = 1.0f;
    }

  moduleData.index[0] = 0;
  moduleData.index[1] = 1;
  moduleData.index[2] = 2;
  moduleData.index[3] = 3;
  moduleData.index[4] = 0;
  moduleData.index[5] = 1;
  moduleData.index[6] = 2;


  buf[2] = 0.0f;
  buf[3] = 0.0f;

  buf[4] = 1.0f;
  buf[5] = 1.0f;
  buf[6] = 1.0f;
  buf[7] = 1.0f;

  buf[10] = 0.0f;
  buf[11] = 0.0f;

  buf[12] = 1.0f;
  buf[13] = 1.0f;
  buf[14] = 1.0f;
  buf[15] = 1.0f;

  buf[18] = 0.0f;
  buf[19] = 0.0f;

  buf[20] = 1.0f;
  buf[21] = 1.0f;
  buf[22] = 1.0f;
  buf[23] = 1.0f;

  buf[26] = 0.0f;
  buf[27] = 0.0f;

  buf[28] = 1.0f;
  buf[29] = 1.0f;
  buf[30] = 1.0f;
  buf[31] = 1.0f;



  if (type == 1) {
      if (special)
        drawBufferSpecial(32, 7, x, y, z, rotation, rx, ry, rz, w, h, sx, sy, sz, ox, oy, GL_TRIANGLE_STRIP);
      else
        drawBuffer(32, 7, GL_TRIANGLE_STRIP);

    }else {
      if (special)
        drawBufferSpecial(32, 7, x, y, z, rotation, rx, ry, rz, w, h, sx, sy, sz, ox, oy, GL_LINE_STRIP);
      else
        drawBuffer(32, 7, GL_LINE_STRIP);
    }
}


void graphics_geometry_points(float x, float y) {
  growBuffers(8,1);
  float* buf = moduleData.data;
  buf[0] = x;
  buf[1] = y;
  buf[2] = 0.0f;
  buf[3] = 0.0f;
  buf[4] = 1.0f;
  buf[5] = 1.0f;
  buf[6] = 1.0f;
  buf[7] = 1.0f;
  moduleData.index[0] = 0;
  drawBuffer(8,1, GL_POINTS);
}

void graphics_geometry_vertex(int type, float x, float y, int vertices[], int count) {
  growBuffers(count,count);
  float* buf = moduleData.data;

  for(int i = 0; i < count; i++) {
      buf[8*i+0] = x + vertices[i];
      buf[8*i+1] = y + vertices[i+1];
      buf[8*i+2] = 0.0f;
      buf[8*i+3] = 0.0f;
      buf[8*i+4] = 1.0f;
      buf[8*i+5] = 1.0f;
      buf[8*i+6] = 1.0f;
      buf[8*i+7] = 1.0f;
      moduleData.index[i] = i;
    }

  if (type == 1)
    drawBuffer(count,count,GL_TRIANGLE_FAN);
  if(type == 0)
    drawBuffer(count,count,GL_LINE_STRIP);
}












