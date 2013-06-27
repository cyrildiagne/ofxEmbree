#include "testApp.h"

void testApp::setup() {
    
    cam.setup(ofPoint(3.72811, 87.9981, -82.1874));
    cam.setDistance(1200);
    cam.setFov(10);
    cam.setRadius(5);
    
    renderer.setup(cam);
    renderer.setSize(ofGetWidth(), ofGetHeight());
    renderer.loadScene("crown/crown_by_martin_lubich.xml");
    renderer.loadBackPlate("crown/background.ppm");
    renderer.addHDRILight("crown/light_tent_by_lightmapltd.pfm", ofColor(1,1,1));
    renderer.setGamma(1.7);
    renderer.setSamplePerPixel(1);
    renderer.setRecursionDepth(20);
    renderer.updateScene();
}

void testApp::update(){
    
	cam.update();
    
	renderer.update();
    renderer.renderGL();
}

void testApp::draw(){
	
    renderer.draw(0, 0);
    
    string stats;
    stats += "FPS : " + ofToString(ofGetFrameRate()) + "\n";
    stats += "Last render time : " + ofToString(renderer.getLastRenderTime()*1000, 0) + "ms" + "\n";
    stats += "Accumulated passes : " + ofToString(renderer.getAccumulation()) + "\n";
    stats += "Accumulated time : " + ofToString(renderer.getAccumulatedTime(), 0) + "s";
    ofDrawBitmapStringHighlight(stats, 10, 20, ofColor(0,0,0,80), ofColor::white);
}

void testApp::keyPressed(int key){
	switch(key){
        case 'f':
            ofToggleFullscreen();
            break;
        default:
            break;
	}
}

void testApp::windowResized(int w, int h){
	renderer.setSize(w, h);
}

void testApp::mouseDragged(int x, int y, int button){
	renderer.reset();
}

void testApp::mouseReleased(int x, int y, int button){
	renderer.reset();
}