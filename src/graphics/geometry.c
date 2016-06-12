/*
#   love
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include <stdlib.h>
#include "../math/util.h"
#include <math.h>
#include <stdlib.h>
#include "geometry.h"
#include "graphics.h"
#include "shader.h"
#include "matrixstack.h"
#include "vertex.h"

static struct {
  graphics_Shader plainColorShader;
  mat4x4 tr2d;
  GLuint vbo;
  GLuint ibo;
  int currentDataSize;
  int currentIndexSize;
  float *data;
  GLubyte *index;
} moduleData;

void graphics_geometry_init(void) {

  glGenBuffers(1, &moduleData.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, moduleData.vbo);
 
  glGenBuffers(1, &moduleData.ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, moduleData.ibo);
 
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(2*sizeof(float)));
  graphics_Shader_new(&moduleData.plainColorShader, NULL,
                      "vec4 effect(vec4 color, Image texture, vec2 texture_coords, vec2 screen_cords ) {\n"
                      "  return color;\n"
                      "}\n");

}

void graphics_geometry_free () {
  glDeleteBuffers(1, &moduleData.ibo);
  glDeleteBuffers(1, &moduleData.vbo);
  graphics_Shader_free(&moduleData.plainColorShader);
}

static void growBuffers(int vertices, int indices) {
  int datasize = vertices * 6 * sizeof(float);
  if(moduleData.currentDataSize < datasize) {
    free(moduleData.data);
    moduleData.data = (float*)malloc(datasize);
    moduleData.currentDataSize = datasize;
  }

  int indexsize = indices * sizeof(uint16_t);
  if(moduleData.currentIndexSize < indexsize) {
    free(moduleData.index);
    moduleData.index = (GLubyte*)malloc(indexsize);
    moduleData.currentIndexSize = indexsize;
  }
}

static void drawBuffer(float x, float y, float w, float h, float rot, float sx, float sy, float ox, float oy, int vertices, int indices, GLenum type) {
  glBindBuffer(GL_ARRAY_BUFFER, moduleData.vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices * 8 *sizeof(float), moduleData.data, GL_STREAM_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, moduleData.ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices*sizeof(uint16_t), moduleData.index, GL_STREAM_DRAW);

  graphics_Shader *shader = graphics_getShader();
  graphics_setShader(&moduleData.plainColorShader);
  mat4x4 tr;
  m4x4_newIdentity(&tr);
  //m4x4_newTransform2d(&tr, x, y, rot, sx, sy, ox, oy, 0, 0);
 
  graphics_Quad quad = {0,0,1,1};

  graphics_drawArray(&quad, &tr, moduleData.ibo, indices, type, GL_UNSIGNED_SHORT, graphics_getColor(), quad.w * w, quad.h * h);
  graphics_setShader(shader);
}

//This piece of shit does not want to work
void graphics_geometry_drawCircle(float x, float y, float radius, int segments) {
  
  int datasize = (segments+1) * 6 * sizeof(float);
  if(moduleData.currentDataSize < datasize) {
    free(moduleData.data);
    moduleData.data = (float*)malloc(datasize);
    moduleData.currentDataSize = datasize;
  }

  int indexsize = (segments + 2) * sizeof(GLubyte);
  if(moduleData.currentIndexSize < indexsize) {
    free(moduleData.index);
    moduleData.index = (GLubyte*)malloc(indexsize);
    moduleData.currentIndexSize = indexsize;
  }
	 
	mat4x4 tr;
   m4x4_newIdentity(&tr);

  #define PI 3.14159265  
  int step = PI*2 / segments;
  int ang = 0;
  moduleData.index[0] = 0;
  moduleData.index[segments+1] = 1;
  
  for(int i = 0; i < segments; i++, ang -= step) {
    moduleData.data[6*(i)  ] = sin(ang)* radius + x;
    moduleData.data[6*(i)+1] = cos(ang) * radius + y;
    moduleData.data[6*(i)+2] = 0.0f;
    moduleData.data[6*(i)+3] = 0.0f;
    moduleData.data[6*(i)+4] = 0.0f;
    moduleData.data[6*(i)+5] = 0.0f;
    
	 moduleData.index[i] = i;
		 //m4x4_rotateZ(&tr,sin(ang*(PI/180)));
  }
  	
	glBindBuffer(GL_ARRAY_BUFFER, moduleData.vbo);
   glBufferData(GL_ARRAY_BUFFER, datasize, moduleData.data, GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, moduleData.ibo);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexsize, moduleData.index, GL_DYNAMIC_DRAW);
 	
	graphics_Shader *shader = graphics_getShader();
   graphics_setShader(&moduleData.plainColorShader);

  	graphics_Quad quad = {0,0,1,1};
	

   graphics_drawArray(&quad, &tr,moduleData.ibo, segments, GL_LINE_LOOP, GL_UNSIGNED_BYTE, graphics_getColor(), radius, radius);
  	graphics_setShader(shader);
 }


void graphics_geometry_fillCircle(float x, float y, float radius, int segments) {
}

void graphics_geometry_fillRectangle(int type, float x, float y, float w, float h, float rotation, 
    float sx, float sy, float ox, float oy) {
 
  growBuffers(32, 5);
  
  float *buf = moduleData.data;

  buf[0] = 1.0f; //0
  buf[1] = 0.0f;
  
  buf[2] = 0.0f; 
  buf[3] = 0.0f;
  
  buf[4] = 1.0f; 
  buf[5] = 1.0f;
  buf[6] = 1.0f;
  buf[7] = 1.0f;

  buf[8] = 0.0f; //1
  buf[9] = 0.0f;

  buf[10] = 0.0f;
  buf[11] = 0.0f;
  
  buf[12] = 1.0f;
  buf[13] = 1.0f;
  buf[14] = 1.0f;
  buf[15] = 1.0f;

  buf[16] = 0.0f; //2
  buf[17] = 1.0f;
  
  buf[18] = 0.0f;
  buf[19] = 0.0f;
  
  buf[20] = 1.0f;
  buf[21] = 1.0f;
  buf[22] = 1.0f;
  buf[23] = 1.0f;

  buf[24] = 1.0f; //3
  buf[25] = 1.0f;
  
  buf[26] = 0.0f;
  buf[27] = 0.0f;
  
  buf[28] = 1.0f;
  buf[29] = 1.0f;
  buf[30] = 1.0f;
  buf[31] = 1.0f;
  
  moduleData.index[0] = 0;
  moduleData.index[1] = 1;
  moduleData.index[2] = 3;
  moduleData.index[3] = 2;
  
  if (type == 1) 
    drawBuffer(x,y,w,h,rotation,sx,sy,ox,oy, 32, 5, GL_TRIANGLE_STRIP);
  else 
    drawBuffer(x,y,w,h,rotation,sx,sy,ox,oy, 32, 5, GL_LINE_STRIP);
}

void graphics_geometry_points(float x, float y) {
  growBuffers(2,2);
  drawBuffer(x,y,1,1,0,1,1,0,0,2,2,GL_POINTS);
}

//TODO not 100% functional 
void graphics_geometry_vertex(float x, float y, int vertices[], int count) {
  growBuffers(count*8, count*2);
  
  float *buf = moduleData.data;
     
  if (count != 1) {   
     for (int i = 0; i < count*2; i++) { 
         //printf("%d %d \n", vertices[i], vertices[i+1]);
         buf[0] = vertices[i]; 
         buf[1] = vertices[i+1]; 
     
         buf[2] = 0.0f;
         buf[3] = 0.0f;

         buf[4] = 1.0f;
         buf[5] = 1.0f;
         buf[6] = 1.0f;
         buf[7] = 1.0f; 
     }
   }
    
  for(int i = 0; i < count; i++) {
    //printf("%d \n",i);
    moduleData.index[i] = i;
  }

  drawBuffer(x, y, 50, 50, 0, 1, 1, 0, 0, count*8, count*2, GL_LINE_LOOP);
}












