#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
	cam.setup(ofPoint(278, 273, 200));
	cam.setDistance(600);
	cam.setFov(55);
    
	renderer.setup(cam);
	renderer.loadScene("cornell_box.obj");
	renderer.addQuadLight(ofPoint(213, 548.77, 227), ofPoint(130, 0, 0), ofPoint(0, 0, 105), ofColor(50, 50, 50));
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
        case ' ': {
            //"Matte", "Plastic", "Dielectric", "ThinDielectric", "Mirror", "Metal", "MetallicPaint", "MatteTextured", "Obj"
            //ofxEmbree::Material mat("glass");
            //renderer.addSphere(mat.embreeMat(), ofPoint(278, 273, 200), 50);
        }
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