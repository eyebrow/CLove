/*
#   clove
#
#   Copyright (C) 2016 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/

#include <stdio.h>
#include "physics.h"

//TODO garbage collector


/* NOTE
 * Everything from here was done with the help from : https://github.com/slembcke/Chipmunk2D/blob/master/demo 
 * The implementation might not be the best. I just added this module to show
 * that if anyone is interested in physics there is the basics from where to
 * expand, anyway everything will be fixed when problems are discovered ^_^
 */

static cpShapeFilter NOT_GRABBABLE_FILTER;

void physics_init() {

}

// WORLD
void physics_newWorld(physics_World* world, float x, float y, float sleep) { 
  world->space = cpSpaceNew(); //create a new world
  
  //init gravity 
  world->gravity = cpv(x,y);
  cpSpaceSetGravity(world->space, world->gravity); //use the new world that has been created 
 
  cpSpaceSetSleepTimeThreshold(world->space, sleep); //after how many seconds to sleep 
}

void physics_updateWorld(physics_World* world, float dt) {
  cpSpaceStep(world->space, dt);  
}

// SHAPE
void physics_newShape(physics_World* world, physics_Body* body,
 	  	physics_Shape* shape, float w, float h) {
	shape->shape = cpSpaceAddShape(world->space, cpBoxShapeNew(body->body, w, h, 0.0f));
		
}

//TODO find a way to make this work 0_o
void physics_setFilter(physics_Shape* shape, const char* name) {
	//cpShapeSetFilter(shape->shape, )
}

void physics_setShapeElasticity(physics_Shape* shape, float v) {
	cpShapeSetElasticity(shape->shape, v);	
}

void physics_setShapeFriction(physics_Shape* shape, float v) {
	cpShapeSetFriction(shape->shape, v);	
}

void physics_setShapeMass(physics_Shape* shape, float v) {
	cpShapeSetMass(shape->shape, v);
}

// BODY
void physics_newBody(physics_World* world, physics_Body* body, const char* type) {
  if(strncmp(type, "static", 6) == 0)
	body->body = cpSpaceAddBody(world->space, cpBodyNewStatic());
  
  if(strncmp(type, "kinematic", 9) == 0)
	body->body = cpSpaceAddBody(world->space, cpBodyNewKinematic());
  
  if(strncmp(type, "dynamic", 7) == 0)
	body->body = cpSpaceAddBody(world->space, cpBodyNewDynamic());
}

void physics_setBodyMass(physics_Body* body, float v) {
	cpBodySetMass(body->body, v);
}

void physics_setBodyMoment(physics_Body* body, float v) {
	cpBodySetMoment(body->body, v);
}

void physics_setBodyPosition(physics_Body* body, float x, float y) {
  cpVect pos = cpv(x,y);
  cpBodySetPosition(body->body, pos);
}

float physics_getBodyX(physics_Body* body) {
 return cpBodyGetPosition(body->body).x; 
}

float physics_getBodyY(physics_Body* body) { 
 return cpBodyGetPosition(body->body).y; 
}

//TODO ADD THIS IN LUA
bool physics_getBodyIsSleeping(physics_Body* body) {
	return cpBodyIsSleeping(body->body);
}

void physics_setAngle(physics_Body* body,float angle) {
  cpBodySetAngle(body->body, angle);
}

float physics_getAngle(physics_Body* body) {
  return cpBodyGetAngle(body->body);
}

void physics_setBodyVel(physics_Body* body, float x, float y) {
  cpVect v = cpv(x,y);
  cpBodySetVelocity(body->body, v);
}

void physics_setBodyMomentum(physics_Body* body, float momentum) {
  cpBodySetMoment(body->body, momentum);
}


//float physics_getBodyVelX(physics_Body* body) {
 //return body->body->v.x; 
//}

//float physics_getBodyVelY(physics_Body* body) { 
 //return body->body->v.y; 
//}

//garbage collection
void physics_freeBody(physics_Body* body) {
   cpBodyFree(body->body);
}

void physics_freeWorld(physics_World* world) { 
   //cpSpaceFree(world->space);
}

void physics_freeShape(physics_Shape* shape) {
  cpShapeFree(shape->shape);
}

