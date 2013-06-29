#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    
	cam.setup(ofPoint(0, 0, 0));
	cam.setDistance(600);
	cam.setFov(75);
    
	renderer.setup(cam);
	renderer.setRecursionDepth(20);
    
    renderer.addHDRILight("lines.ppm", ofColor(1.0,1.0,1.0));
    
    addSphere(ofPoint(50, 0, 0), 50);
    addSphereNatively(ofPoint(-50, 0, 0), 50);
    
    addGround();
    
    for (int i=0; i<5; i++){
        ofPoint p = ofPoint(ofRandom(-400, 400), ofRandom(-400, 400), ofRandom(-400, 400));
        ofPoint r = ofPoint(ofRandom(360), ofRandom(360), ofRandom(360));
        addBox(p, r);
    }
}

//------------------------------

void testApp::addSphere(ofPoint pos, float radius){
    
    string matName = "random_"+ ofToString( materials.getMap().size() );
    materials.add(matName, "MetallicPaint");
    materials.setProp(matName, "eta", 1.45f);
    materials.setProp(matName, "glitterColor", ofColor(0.5, 0.44, 0.42));
    materials.setProp(matName, "glitterSpread", 0.01f);
    materials.setProp(matName, "shadeColor", ofPoint(ofRandom(1.f), ofRandom(1.f), ofRandom(1.f)));
    
    ofSpherePrimitive * sphere = new ofSpherePrimitive();
    sphere->setRadius(radius);
    sphere->setPosition(pos);
    Device::RTShape shape = renderer.addSphere(materials.get(matName), *sphere);
    spheresMap[shape] = sphere;
}

void testApp::addGround(){
    
    ofPlanePrimitive * plane = new ofPlanePrimitive();
    plane->set(1500, 1500, 20, 20);
    plane->setPosition(0, -400, 0);
    plane->rotate(90, 1, 0, 0);
    Device::RTShape shape = renderer.addMesh(materials.white(), plane->getMesh(), plane->getGlobalTransformMatrix());
    meshMap[shape] = plane;
}

void testApp::addBox(ofPoint pos, ofPoint rot){
    
    ofBoxPrimitive * box = new ofBoxPrimitive();
    box->set(150, 150, 150);
    box->setPosition(pos);
    box->setOrientation(rot);
    Device::RTShape shape = renderer.addMesh(materials.gold(), box->getMesh(), box->getGlobalTransformMatrix());
    meshMap[shape] = box;
}

/*
 Demonstrate how to use the Embree API directly
 */
void testApp::addSphereNatively(ofPoint pos, float radius){
    
    // retrieve the shared device
    Device * device = renderer.getDevice();
    
    // create a red velvet material
    Handle<Device::RTMaterial> material = g_device->rtNewMaterial("Velvet");
    g_device->rtSetFloat3(material, "reflectance", 0.4, 0, 0);
    g_device->rtSetFloat1(material, "backScattering", 0.5f);
    g_device->rtSetFloat3(material, "horizonScatteringColor", 0.75, 0.1, 0.1);
    g_device->rtSetFloat1(material, "horizonScatteringFallOff", 10.0f);
    g_device->rtCommit(material);
    
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