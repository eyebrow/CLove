/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "filesystem.h"
#include <stdio.h>
#include <stdlib.h>

void filesystem_init(void) {

}

int filesystem_contain(const char* a, const char* b) {
  if(strstr(a,b))
    return 1;
  else
    return 0;
}

int filesystem_compare(const char* a, const char* b, int l) {
  if(strncmp(a, b, l) == 0)
    return 1;
  else
    return 0;
}

int filesystem_exists(const char* name)
{
  FILE* file = fopen(name,"r");
  if(!file){
      return 0;
    }

  fclose(file);
  return 1;
}

int filesystem_write(const char* name, const char* data)
{
  FILE* file = fopen(name, "w");
  if(!file){
      printf("%s No file named %s",name,"%s creating one");
      return -1;
    }

  fseek(file,0,SEEK_END);
  long size = ftell(file);
  rewind(file);

  fprintf(file, data);
  fclose(file);

  return size;
}

int filesystem_append(const char* name, const char* data)
{
  FILE* file = fopen(name, "a");

  fseek(file,0,SEEK_END);
  long size = ftell(file);
  rewind(file);

  fprintf(file, data);
  fclose(file);

  return size;
}

int filesystem_read(char const* filename, char** output) {
  FILE* infile = fopen(filename, "r");
  if(!infile) {
      return -1;
    }

  fseek(infile, 0, SEEK_END);
  long size = ftell(infile);
  rewind(infile);

  *output = malloc(size+1);
  fread(*output, size, 1, infile);
  fclose(infile);
  (*output)[size] = 0;
  return size;
}


int filesystem_remove(const char* name) {
  remove(name);
  return 0;
}
