#pragma once

#include "ofMain.h"
#include "ofxEmbree.h"

class testApp : public ofBaseApp{

public:
    void setup();
    void update();
    void draw();
    void keyPressed  (int key);
    void windowResized(int w, int h);
    
    void mouseDragged(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    
private:
    
    ofxEmbree::Renderer renderer;
    ofxEmbree::Camera cam;
};
