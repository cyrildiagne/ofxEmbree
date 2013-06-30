#pragma once

#include "ofMain.h"
#include "ofxAssimpModelLoader.h"
#include "ofxEmbree.h"

class testApp : public ofBaseApp{

public:
    
    void setup();
    void exit();
    void update();
    void draw();
    void keyPressed  (int key);
    void windowResized(int w, int h);
    void mouseDragged(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
    void addGround();
    void addSquirrels();
    void addMesh(Device::RTMaterial material, ofPoint pos);
    
    bool bEmbreeRender;
    
private:
    
    ofxEmbree::Renderer renderer;
    ofxEmbree::Camera cam;
    ofxEmbree::Materials materials;
    
    ofPlanePrimitive * ground;
    map<Device::RTShape, ofxAssimpModelLoader *> meshMap;
};
