#pragma once

#include "ofMain.h"
#include "ofxEmbree.h"

typedef map<Device::RTShape, ofSpherePrimitive*> SphereMap;
typedef map<Device::RTShape, of3dPrimitive*> MeshMap;

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
    
    void addSphere(ofPoint pos, float radius);
    void addSphereNatively(ofPoint pos, float radius);
    void addBox(ofPoint pos, ofPoint rot);
    void addGround();
    
private:
    
    ofxEmbree::Renderer renderer;
    ofxEmbree::Camera cam;
    
    SphereMap spheresMap;
    MeshMap meshMap;
};
