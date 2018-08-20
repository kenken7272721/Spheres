#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ParticleVec3.h"


class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    void drawScene();
    void drawSubElement(int gridSize);
    
    ofVec3f position;
    ofVec3f velocity;
    ofVec3f acceleration;
    
    float radius;
    float friction;
    float mass;
    float maxx, maxy, maxz, minx, miny, minz;
    
    vector<ParticleVec3> particles;
    ofVboMesh vboMesh;
    
    static const int particleNum = 5000;
    
    bool move;
    
    
    
    float width = 100.0;
    float height = 100.0;
    float depth = 2000.0;
    float angle = 0;
    
    ofxPanel gui;
    ofParameter<ofFloatColor> light_specular, light_diffuse, light_ambient,
    material_diffuse, material_ambient, material_specular, material_emissive;
    ofParameter<int> shininess;
    ofParameter<ofColor> centerColor, edgeColor;
    
    ofxFloatSlider testAmount1, testAmount2;
    
    ofVec3f previous, current;
    deque<ofVec3f> pathVertices;
    
    ofMesh pathLines;
    
    ofEasyCam cam, subCam;
    ofLight light;
    ofMaterial material;
    
    ofVec3f camPos, objectPos;

    bool showGui;
    bool showGrid;
    bool changeView;
    bool showScene;
};
