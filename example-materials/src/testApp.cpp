#include "testApp.h"

typedef map<Device::RTShape, ofxAssimpModelLoader *> MeshMap;

//--------------------------------------------------------------

void testApp::setup(){
    
    ofBackground(ofColor::black);
    
	cam.setup(ofPoint(0, 150, 0));
	cam.setDistance(800);
	cam.setFov(65);
    cam.setLongitude(0);
    
	renderer.setup(cam);
	renderer.setRecursionDepth(10);
    renderer.addQuadLight(ofPoint(-500, 600, -500), ofPoint(1000, 0, 0), ofPoint(0, 0, 1000), ofColor(2.0, 2.0, 2.0));
    
    addGround();
    addSquirrels();
    
    bEmbreeRender = false;
}

//------------------------------

void testApp::addGround(){
    
    ground = new ofPlanePrimitive();
    ground->set(1500, 1500, 20, 20, OF_PRIMITIVE_TRIANGLES);
    ground->setPosition(0, -15, 0);
    ground->rotate(90, 1, 0, 0);
    renderer.addMesh(materials.white(), ground->getMesh(), ground->getGlobalTransformMatrix());
}

void testApp::addSquirrels(){
    
    vector<Device::RTMaterial> matlist(4);
    matlist[0] = materials.white();
    matlist[1] = materials.glass();
    matlist[2] = materials.gold();
    matlist[3] = materials.mettalicPaint(ofColor(1.0, 0, 0));
    int x = - ((int)matlist.size()-1) * 200.f * 0.5;
    for (int i=0; i<matlist.size(); i++) {
        addMesh(matlist[i], ofPoint(x+i*200, 0, 0));
    }
}

void testApp::addMesh(Device::RTMaterial material, ofPoint pos){
    
    ofxAssimpModelLoader * model = new ofxAssimpModelLoader();
    model->loadModel("NewSquirrel.3ds", true);
    model->setPosition(pos.x, pos.y, pos.z);
    ofMesh mesh = model->getMesh(0);
    Device::RTShape shape = renderer.addMesh(material, mesh, model->getModelMatrix() * model->getMeshHelper(0).matrix);
    model->setRotation(0, 180, 1, 0, 0);
    meshMap[shape] = model;
}


//------------------------------

void testApp::exit(){
    
    for (MeshMap::iterator it = meshMap.begin(); it!=meshMap.end(); ++it) {
        delete it->second;
    }
}

void testApp::update(){
    
	cam.update();
    
    if(bEmbreeRender) {
        renderer.update();
        renderer.renderGL();
    }
}

void testApp::draw(){
	
    string text;
    text = "Press ' ' to toggle render mode\n";
    text += "Press 's' to add a random sphere\n";
    text += "Press 'f' to toggle fullscreen";
    
    if(!bEmbreeRender) {
    
        cam.begin();
        ground->draw(OF_MESH_WIREFRAME);
        for (MeshMap::iterator it = meshMap.begin(); it!=meshMap.end(); ++it) {
            ofxAssimpModelLoader * s = it->second;
            s->draw(OF_MESH_WIREFRAME);
        }
        cam.end();   
    }
    else {
    
        renderer.draw(0, 0);
        
        text += "\n\nFPS : " + ofToString(ofGetFrameRate()) + "\n";
        text += "Last render time : " + ofToString(renderer.getLastRenderTime()*1000, 0) + "ms" + "\n";
        text += "Accumulated passes : " + ofToString(renderer.getAccumulation()) + "\n";
        text += "Accumulated time : " + ofToString(renderer.getAccumulatedTime(), 0) + "s";
    }
    
    ofDrawBitmapStringHighlight(text, 10, 20, ofColor(0,0,0,50), ofColor::white);
}

void testApp::keyPressed(int key){
	switch(key){
        case 'f':
            ofToggleFullscreen();
            break;
        case ' ':
            bEmbreeRender = !bEmbreeRender;
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