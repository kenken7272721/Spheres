//
//  ParticleVec3.cpp
//  Spheres
//
//  Created by Ken Ishii on 2018/08/19.
//
//

#include <stdio.h>
#include "ParticleVec3.h"

ParticleVec3::ParticleVec3() {
    radius = 5.0;
    friction = 0.01;
    mass = 1.0;
    position = ofVec3f(0.0, 0.0, 0.0);
    velocity = ofVec3f(0.0, 0.0, 0.0);
    minx = -1000;
    miny = -1000;
    minz = -1000;
    maxx = 1000;
    maxy = 1000;
    maxz = 1000;
}

//--------------------------------------------------------------
void ParticleVec3::update() {
    velocity += acceleration;
    position += velocity;
    acceleration.set(0, 0);
}

//--------------------------------------------------------------
void ParticleVec3::draw(){
    ofDrawSphere(position.x, position.y, position.z, radius);
}

//--------------------------------------------------------------
void ParticleVec3::addForce(float forceX, float forceY, float forceZ) {
    acceleration += ofVec3f(forceX, forceY, forceZ) / mass;
}

//--------------------------------------------------------------
void ParticleVec3::bounceOffWalls() {
    if (position.x > maxx){
        position.x = maxx;
        velocity.x *= -1;
    }
    
    if (position.x < minx){
        position.x = minx;
        velocity.x *= -1;
    }
    
    if (position.y > maxy){
        position.y = maxy;
        velocity.y *= -1;
    }
    
    if(position.y < miny){
        position.y = miny;
        velocity.y *= -1;
    }
}

//--------------------------------------------------------------
void ParticleVec3::throughOffWalls() {
    if (position.x < minx) {
        position.x = maxx;
    }
    
    if (position.y < miny) {
        position.y = maxy;
    }
    
    if (position.x > maxx) {
        position.x = minx;
    }
    
    if (position.y > maxy) {
        position.y = miny;
    }
}

//--------------------------------------------------------------
void ParticleVec3::addAttractionForce(float x, float y, float z, float radius, float scale) {
    ofVec3f posOfForce;
    posOfForce.set(x, y, z);
    ofVec3f diff = position - posOfForce;
    float length = diff.length();
    bool bAmCloseEnough = true;
    
    if (radius > 0) {
        if (length > radius) {
            bAmCloseEnough = false;
        }
    }
    
    if (bAmCloseEnough == true) {
        float pct = 1 - (length / radius);
        diff.normalize();
        acceleration = acceleration - diff * scale * pct;
    }
}

//--------------------------------------------------------------
void ParticleVec3::addRepulsionForce(float x, float y, float z, float radius, float scale) {
    ofVec3f posOfForce;
    posOfForce.set(x, y, z);
    ofVec3f diff = position - posOfForce;
    float length = diff.length();
    bool bAmCloseEnough = true;
    
    if (radius > 0) {
        if (length > radius) {
            bAmCloseEnough = false;
        }
    }
    
    if (bAmCloseEnough == true) {
        float pct = 1 - (length / radius);
        diff.normalize();
        acceleration = acceleration + diff * scale * pct;
    }
}

