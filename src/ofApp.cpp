#include "ofApp.h"


//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    //ofSetVerticalSync(true);
    
    gui.setup();
    //light gui settings
    gui.add(light_specular.set("light specular", ofFloatColor(0.5, 0.5, 1.0),
                               ofFloatColor(0.0, 0.0, 0.0),
                               ofFloatColor(1.0, 1.0, 1.0)));
    gui.add(light_diffuse.set("light diffuse", ofFloatColor(1.0, 1.0, 1.0),
                              ofFloatColor(0.0, 0.0, 0.0),
                              ofFloatColor(1.0, 1.0, 1.0)));
    gui.add(light_ambient.set("light ambient", ofFloatColor(1.0, 1.0, 1.0),
                              ofFloatColor(0.0, 0.0, 0.0),
                              ofFloatColor(1.0, 1.0, 1.0)));
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    //material gui settings
    gui.add(material_diffuse.set("material diffuse", ofFloatColor(0.632653, 0.576531, 0.617347),
                                 ofFloatColor(0.0, 0.0, 0.0),
                                 ofFloatColor(1.0, 1.0, 1.0)));
    gui.add(material_ambient.set("material ambient", ofFloatColor(0.489796, 0.153061, 0.2),
                                 ofFloatColor(0.0, 0.0, 0.0),
                                 ofFloatColor(1.0, 1.0, 1.0)));
    gui.add(material_specular.set("material specular", ofFloatColor(0.2, 0.0, 0.0),
                                  ofFloatColor(0.0, 0.0, 0.0),
                                  ofFloatColor(1.0, 1.0, 1.0)));
    gui.add(material_emissive.set("material emissive", ofFloatColor(0.0, 0.0, 0.0),
                                  ofFloatColor(0.0, 0.0, 0.0),
                                  ofFloatColor(1.0, 1.0, 1.0)));
    gui.add(shininess.set("shininess", 0, 0, 128));
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    //background color gui settings
    gui.add(centerColor.set("center color", ofColor(75, 225, 238),
                            ofColor(0, 0, 0),
                            ofColor(255, 255, 255)));
    gui.add(edgeColor.set("edge color", ofColor(93, 193, 251),
                          ofColor(0, 0, 0),
                          ofColor(255, 255, 255)));
    ////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////
    gui.add(testAmount1.setup("testAmount1", 0.1, 0.0, 20.0));
    gui.add(testAmount2.setup("testAmount2", 0.01, -10.0, 10.0));
    
    //light settings
    light.setDirectional();
    light.setPosition(ofVec3f(100.0, -100.0, -100.0));
    ofEnableLighting();
    
    pathLines.setMode(OF_PRIMITIVE_LINE_STRIP);
    
    subCam.setDistance(1000.0);
    
    vboMesh.setMode(OF_PRIMITIVE_POINTS);
    //glPointSize(10.0);
    
    move = true;
    
    showGui = true;
    showGrid = false;
    changeView = true;
    showScene = true;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
    previous = current;
    
    // generate a noisy 3d position over time
    float t = (2 + ofGetElapsedTimef()) * 0.2;
    current.x = ofSignedNoise(t, 0, 0);
    current.y = ofSignedNoise(0, t, 0);
    current.z = ofSignedNoise(0, 0, t);
    current *= 1000; // scale from -1,+1 range to -400,+400
    
    // add the current position to the pathVertices deque
    pathVertices.push_back(current);
    // if we have too many vertices in the deque, get rid of the oldest ones
    while(pathVertices.size() > 200) {
        pathVertices.pop_front();
    }
    // clear the pathLines ofMesh from any old vertices
    pathLines.clear();
    // add all the vertices from pathVertices
    for(unsigned int i = 0; i < pathVertices.size(); i++) {
        pathLines.addVertex(pathVertices[i]);
    }
    
    vboMesh.clear();
    
    while (particles.size() > particleNum) {
        particles.pop_back();
    }
    
    if (move) {
        for (int i = 0; i < particles.size(); i++) {
            particles[i].addAttractionForce(current.x, current.y, current.z, 1500, 0.8);
            particles[i].update();
            vboMesh.addVertex(ofVec3f(particles[i].position.x, particles[i].position.y, particles[i].position.z));
            float distance = ofDist(current.x, current.y, current.z, particles[i].position.x, particles[i].position.y, particles[i].position.z);
            if (distance > 2000) {
                particles[i].position.set(current - ofVec3f(ofRandom(-50.0, -25.0), ofRandom(-100.0, 100.0), ofRandom(-50.0, -25.0)));
            }
        }
        
        for (int i = 0; i < particleNum; i++) {
            ParticleVec3 p;
            p.position = ofVec3f(current.x - 100, current.y, current.z - 100);
            float length = ofRandom(-10.0, 0.0);
            float angle = ofRandom(2.0 * PI);
            ofVec2f velocity = ofVec2f(length* sin(angle), length * cos(angle));
            //ofVec2f velocity = ofVec2f(length, length);
            p.velocity = velocity;
            particles.push_back(p);
            
        }
    }
    
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::drawScene(){
    ofSetColor(0);
    //pathLines.draw();
    //vboMesh.draw();
    for (int i = 0; i < particleNum; i++) {
        ofDrawSphere(particles[i].position.x, particles[i].position.y, particles[i].position.z, 15.0);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(centerColor, edgeColor, OF_GRADIENT_CIRCULAR);
    
    camPos = ofVec3f(current.x, current.y, current.z + 300.0);
    objectPos = ofVec3f(current.x, current.y, current.z);
    ofVec3f tmp = objectPos - camPos;
    tmp.normalize();
    
    cam.setPosition(tmp * 500.0);
    cam.lookAt(ofVec3f(current.x, current.y, current.z));
    light.setPosition(tmp * 500);
    light.lookAt(ofVec3f(current.x, current.y, current.z)*-1);
    
    light.setSpecularColor(light_specular);
    light.setDiffuseColor(light_diffuse);
    light.setAmbientColor(light_ambient);
    
    material.setColors(material_diffuse, material_ambient, material_specular, material_emissive);
    material.setShininess(shininess);
    
    ofEnableDepthTest();
    light.enable();
    material.begin();
    
    if (changeView == true){
        subCam.disableMouseInput();
        cam.begin();
        if (showScene) drawScene();
        cam.end();
    } else {
        subCam.enableMouseInput();
        subCam.begin();
        drawScene();
        drawSubElement(20);
        subCam.end();
    }
    
    material.end();
    light.disable();
    ofDisableDepthTest();
    
    glDisable(GL_LIGHTING);
    glDisable(GL_DEPTH_TEST);
    if (showGui == true) gui.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch (key) {
        case 'f':
            ofToggleFullscreen();
            break;
        case 'g':
            showGui = !showGui;
            break;
        case 'h':
            showGrid = !showGrid;
            break;
        case ' ':
            changeView = !changeView;
            break;
        case '1':
            showScene = !showScene;
            break;
            
        case 'p':
            for (int i = 0; i < 100; i++) {
                
            }
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}

//--------------------------------------------------------------
void ofApp::drawSubElement(int gridSize){
    if (changeView == false) {
        material.end();
        if (showGrid == true) ofDrawGrid(gridSize);
        ofVec3f tmp = objectPos - camPos;
        tmp.normalize();
        cam.setPosition(tmp * 100.0);
        cam.lookAt(ofVec3f(current.x, current.y, current.z));
        ofSetColor(50);
        if (showGrid == true) cam.draw();
        ofSetColor(150);
        if (showGrid == true) light.draw();
        material.begin();
    }
}
