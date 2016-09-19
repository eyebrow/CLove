/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include "../physics/physics.h"
#include "tools.h"

static struct {
  lua_State *luaState;
  int physicsDataMT;
} moduleData;


//WORLD
static int l_physics_newWorld(lua_State* state) {
  float grav_x = luaL_checknumber(state, 1);
  float grav_y = luaL_checknumber(state, 2);
  int sleep = luaL_checknumber(state, 3);
  
  physics_World* world = lua_newuserdata(state, sizeof(physics_World));
  
  physics_newWorld(world, grav_x, grav_y, sleep);
  return 1;
}

static int l_physics_update(lua_State* state) { 
  physics_World* world = (physics_World*)lua_touserdata(state, 1);
  float dt = luaL_checknumber(state, 2);
  physics_updateWorld(world, dt);
  return 1;
}

// SHAPE
static int l_physics_newShape(lua_State* state) {
	physics_Shape* shape = lua_newuserdata(state, sizeof(physics_Shape));

	physics_World* world = (physics_World*)lua_touserdata(state, 1);
	physics_Body* body = (physics_Body*)lua_touserdata(state, 2);
	float w = luaL_checknumber(state, 3);
	float h = luaL_checknumber(state, 4);
	
   physics_newShape(world, body, shape, w, h);

   lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.physicsDataMT);
   lua_setmetatable(state, -2);	
	return 1;
}

static int l_physics_shape_setFriction(lua_State* state) {
	physics_Shape* shape = lua_touserdata(state, 1);
	
	float v = luaL_checknumber(state, 2);
	
	physics_setShapeFriction(shape, v);
	return 1;
}

static int l_physics_shape_setElasticity(lua_State* state) {
	physics_Shape* shape = lua_touserdata(state, 1);
	
	float v = luaL_checknumber(state, 2);
	
	physics_setShapeElasticity(shape, v);
	return 1;
}

// BODY
static int l_physics_newBody(lua_State *state) {
  physics_Body *body = lua_newuserdata(state, sizeof(physics_Body));  
  
  physics_World* world = (physics_World*)lua_touserdata(state, 1);
  
  const char* type = luaL_checkstring(state, 2);
  physics_newBody(world, body, type); 
 
  lua_rawgeti(state, LUA_REGISTRYINDEX, moduleData.physicsDataMT);
  lua_setmetatable(state, -2);
  return 1;
}

static int l_physics_body_setMass(lua_State* state) {
	physics_Body* body = (physics_Body*)lua_touserdata(state, 1);
	float v = luaL_checknumber(state, 2);
	physics_setBodyMass(body, v);
	return 1;
}

static int l_physics_body_setMoment(lua_State* state) {
	physics_Body* body = (physics_Body*)lua_touserdata(state, 1);
	float v = luaL_checknumber(state, 2);
	physics_setBodyMoment(body, v);
	return 1;
}

static int l_physics_body_setPosition(lua_State* state) {
  physics_Body* body = (physics_Body*)lua_touserdata(state, 1);
  float x = luaL_checknumber(state, 2);
  float y = luaL_checknumber(state, 3);
  physics_setBodyPosition(body, x, y);
  return 1;
}

static int l_physics_body_getAngle(lua_State* state) {
  physics_Body* body = (physics_Body*)lua_touserdata(state, 1);
  float r = physics_getAngle(body);
  lua_pushnumber(state,r);
  return 1;
}

static int l_physics_body_setAngle(lua_State* state) { 
  physics_Body* body = (physics_Body*)lua_touserdata(state, 1);
  float a = luaL_checknumber(state, 2);
  physics_setAngle(body, a);
}

static int l_physics_body_getX(lua_State* state) { 
  physics_Body* body = (physics_Body*)lua_touserdata(state, 1);
  lua_pushnumber(state,physics_getBodyX(body));
  return 1;
}

static int l_physics_body_getY(lua_State* state) { 
  physics_Body* body = (physics_Body*)lua_touserdata(state, 1);
  lua_pushnumber(state,physics_getBodyY(body));
  return 1;
}

static int l_physics_body_getVelX(lua_State* state) { 
  physics_Body* body = (physics_Body*)lua_touserdata(state, 1);
  //lua_pushnumber(state,physics_getBodyVelocityX(body));
  return 1;
}

static int l_physics_body_getVelY(lua_State* state) { 
  physics_Body* body = (physics_Body*)lua_touserdata(state, 1);
  //lua_pushnumber(state,physics_getBodyVelocityY(body));
  return 1;
}

static int l_physics_body_setVel(lua_State* state) {
  physics_Body* body = (physics_Body*)lua_touserdata(state, 1);
  float x = luaL_checknumber(state,2);
  float y = luaL_checknumber(state,3);
  physics_setBodyVel(body, x, y);
  return 1;
}

static int l_physics_body_setMomentum(lua_State* state) {
  physics_Body* body = (physics_Body*)lua_touserdata(state, 1);
  float m = luaL_checknumber(state,2);
  physics_setBodyMomentum(body, m);
}

// Garbage collector

static int l_physics_body_gc(lua_State* state) {
  physics_Body* body = (physics_Body*)lua_touserdata(state, 1);
  physics_freeBody(body);
  
  return 1;
}

static int l_physics_free(lua_State* state) { 
  physics_World* world = (physics_World*)lua_touserdata(state, 1);
  physics_freeWorld(world);
  
  return 1;
}

static luaL_Reg const SourceMetatableFuncs[] = {
  {"__gc", l_physics_free},
 	
  // BODY
  {"setPosition", l_physics_body_setPosition},
  {"setMass", l_physics_body_setMass},
  {"setMoment", l_physics_body_setMoment},
  {"getX", l_physics_body_getX},
  {"getY", l_physics_body_getY},
  {"setVelocity", l_physics_body_setVel},
  {"setMomentum", l_physics_body_setMomentum},
  {"getAngle", l_physics_body_getAngle},
  {"setAngle", l_physics_body_setAngle},
	
  // SHAPE
  {"setElasticity", l_physics_shape_setElasticity},	
  {"setFriction", l_physics_shape_setFriction},

  //{"getVelocityX", l_physics_body_getVelX},
  //{"getVelocityY", l_physics_body_getVelY},
  {"gc",   l_physics_body_gc},
  //{"gc",   l_physics_shape_gc},
  {NULL, NULL}
};

static luaL_Reg const regFuncs[] = {
  {"newWorld", l_physics_newWorld},
  {"newBody", l_physics_newBody},
  {"newShape", l_physics_newShape},
  {"update", l_physics_update},
  {NULL,NULL}
};

int l_physics_register(lua_State* state) {
  l_tools_registerModule(state, "physics", regFuncs);
   moduleData.physicsDataMT = l_tools_makeTypeMetatable(state, SourceMetatableFuncs);
  
  return 0;
}
