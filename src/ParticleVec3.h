//
//  ParticleVec3.h
//  Spheres
//
//  Created by Ken Ishii on 2018/08/19.
//
//

#ifndef ParticleVec3_h
#define ParticleVec3_h

#include <stdio.h>
#include "ofMain.h"

class ParticleVec3 {
    
public:
    ParticleVec3();
    void update();
    void draw();
    
    void addForce(float forceX, float forceY, float forceZ);
    
    void bounceOffWalls();
    void throughOffWalls();
    
    void addAttractionForce(float x, float y, float z, float radius, float scale);
    void addRepulsionForce(float x, float y, float z,float radius, float scale);
    
    ofVec3f position;
    ofVec3f velocity;
    ofVec3f acceleration;
    
    float radius;
    float friction;
    float mass;
    float maxx, maxy, minx, miny, maxz, minz;
    
};


#endif /* ParticleVec3_h */
