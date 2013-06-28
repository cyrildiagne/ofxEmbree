#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
	cam.setup(ofPoint(0, 0, 0));
	cam.setDistance(600);
	cam.setFov(55);
    
	renderer.setup(cam);
	renderer.setRecursionDepth(20);
    
    renderer.addHDRILight("lines.ppm", ofColor(1.0,1.0,1.0));
    
    renderer.addMaterial("defaultMat", "matte");
    renderer.setMaterialProp("defaultMat", "reflectance", ofPoint(0.5, 0.5, 0.5));
    renderer.updateMaterial("defaultMat");
    
    renderer.addMaterial("glassMat", "glass");
    renderer.setMaterialProp("glassMat", "transmission", ofPoint(1, 1, 1));
    renderer.setMaterialProp("glassMat", "etaOutside", 1.0f);
    renderer.setMaterialProp("glassMat", "etaInside", 1.45f);
    renderer.updateMaterial("glassMat");
    
    renderer.addMaterial("goldMat", "Metal");
    renderer.setMaterialProp("goldMat", "eta", ofPoint(0.19, 0.45, 1.50));
    renderer.setMaterialProp("goldMat", "k", ofPoint(3.06,2.40,1.88));
    renderer.setMaterialProp("goldMat", "roughness", 0.005f);
    renderer.updateMaterial("goldMat");
    
    addSphere(ofPoint(50, 0, 0), 50);
    addSphereNatively(ofPoint(-50, 0, 0), 50);
    
    addGround();
    
    for (int i=0; i<5; i++) {
        ofPoint p = ofPoint(ofRandom(-400, 400), ofRandom(-400, 400), ofRandom(-400, 400));
        ofPoint r = ofPoint(ofRandom(360), ofRandom(360), ofRandom(360));
        addBox(p, r);
    }
}

//------------------------------

void testApp::addSphere(ofPoint pos, float radius){
    
    ofSpherePrimitive * sphere = new ofSpherePrimitive();
    sphere->setRadius(radius);
    sphere->setPosition(pos);
    Device::RTShape shape = renderer.addSphere("glassMat", *sphere);
    spheresMap[shape] = sphere;
}

void testApp::addGround(){
    
    ofPlanePrimitive * plane = new ofPlanePrimitive();
    plane->set(500, 500, 20, 20);
    Device::RTShape shape = renderer.addMesh("defaultMat", plane->getMesh());
    meshMap[shape] = plane;
}

void testApp::addBox(ofPoint pos, ofPoint rot){
    
    ofBoxPrimitive * box = new ofBoxPrimitive();
    box->set(150, 150, 150);
    box->setPosition(pos);
    box->setOrientation(rot);
    Device::RTShape shape = renderer.addMesh("goldMat", box->getMesh(), box->getGlobalTransformMatrix());
    meshMap[shape] = box;
}

/*
 Demonstrate how to use the Embree API directly
 */
void testApp::addSphereNatively(ofPoint pos, float radius){
    
    // retrieve the shared device
    Device * device = renderer.getDevice();
    
    // create a material
    Handle<Device::RTMaterial> material = device->rtNewMaterial("Metal");
    device->rtSetFloat3(material, "eta", 0.19, 0.45, 1.50);
    device->rtSetFloat3(material, "k", 3.06, 2.40, 1.88);
    device->rtSetFloat1(material, "roughness", 0.005f);
    device->rtCommit(material);
    
    // create a sphere shape
    Handle<Device::RTShape> sphere = device->rtNewShape("sphere");
    device->rtSetFloat3(sphere, "P", pos.x, pos.y, pos.z);
    device->rtSetFloat1(sphere, "r", radius);
    device->rtSetInt1(sphere, "numTheta", 50);
    device->rtSetInt1(sphere, "numPhi"  , 50);
    device->rtCommit(sphere);
    
    // create a shape primitive
    AffineSpace3f transform = AffineSpace3f(one);
    Handle<Device::RTPrimitive> prim = g_device->rtNewShapePrimitive(sphere, material, copyToArray(transform));
    
    // add the primitive to the renderer "display list"
    renderer.getPrimitives().push_back(prim);
    
    // rebuild the scene
    renderer.buildScene();
    
    // keep the OF scene in sync
    ofSpherePrimitive * sphereOF = new ofSpherePrimitive();
    sphereOF->setRadius(radius);
    sphereOF->setPosition(pos);
    spheresMap[sphere] = sphereOF;
}

//------------------------------

void testApp::exit(){
    for (SphereMap::iterator it = spheresMap.begin(); it!=spheresMap.end(); ++it) {
        delete it->second;
    }
    for (MeshMap::iterator it = meshMap.begin(); it!=meshMap.end(); ++it) {
        delete it->second;
    }
}

void testApp::update(){
    
	cam.update();
    
    renderer.update();
    renderer.renderGL();
}

void testApp::draw(){
	
    string text;
    
    // Draw GL render
    
    cam.begin(ofRectangle(0, 0, ofGetWidth()*0.5, ofGetHeight()));
    for (SphereMap::iterator it = spheresMap.begin(); it!=spheresMap.end(); ++it) {
        ofSpherePrimitive * s = it->second;
        s->draw(OF_MESH_WIREFRAME);
    }
    for (MeshMap::iterator it = meshMap.begin(); it!=meshMap.end(); ++it) {
        of3dPrimitive * s = it->second;
        s->draw(OF_MESH_WIREFRAME);
    }
    cam.end();
    
    text = "Press 's' to add a random sphere\n";
    text += "Press 'f' to toggle fullscreen";
    ofDrawBitmapStringHighlight(text, 10, 20);
    
    
    // Draw Embree render
    
    renderer.draw(ofGetWidth()*0.5, 0);
    
    text = "FPS : " + ofToString(ofGetFrameRate()) + "\n";
    text += "Last render time : " + ofToString(renderer.getLastRenderTime()*1000, 0) + "ms" + "\n";
    text += "Accumulated passes : " + ofToString(renderer.getAccumulation()) + "\n";
    text += "Accumulated time : " + ofToString(renderer.getAccumulatedTime(), 0) + "s";

    ofDrawBitmapStringHighlight(text, ofGetWidth()*0.5+10, 20);
}

void testApp::keyPressed(int key){
	switch(key){
        case 'f':
            ofToggleFullscreen();
            break;
        case 's': {
            ofPoint pos = ofPoint(ofRandom(-200, 200), ofRandom(-200, 200), ofRandom(-200, 200));
            addSphere(pos, ofRandom(25, 75));
        }
            break;
        default:
            break;
	}
}

void testApp::windowResized(int w, int h){
	renderer.setSize(w/2, h);
}

void testApp::mouseDragged(int x, int y, int button){
	renderer.reset();
}

void testApp::mouseReleased(int x, int y, int button){
	renderer.reset();
}