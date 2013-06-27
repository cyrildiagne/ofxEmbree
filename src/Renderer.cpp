//
//  Renderer.cpp
//  RendererExample
//
//  Created by kikko_fr on 6/26/13.
//
//

#include "Renderer.h"

#include "renderers/integratorrenderer.h"
#include "sys/sync/mutex.h"

namespace embree {
    
    Device * g_device = NULL;
}

namespace ofxEmbree {
    
    Renderer::Renderer(){
        
        width = 512;
        height = 512;
        
        depth = 2;
        spp = 1;
        gamma = 1.0f;
        numBuffers = 1;
        
        accel = "default";
        tri = "default";
        
        resetAccumulation = false;
        accumulation = 0;
        
        lastRenderTime = accumulatedTime = 0.0f;
    }
    
    Renderer::~Renderer(){
        
    }
    
    void Renderer::setup(Camera & cam_, int numThreads){
        
        cam = &cam_;
        
        device = g_device = Device::rtCreateDevice("default", numThreads);
        
        renderer = device->rtNewRenderer("pathtracer");
        if(depth >= 0){
            device->rtSetInt1(renderer, "maxDepth", depth);
        }
        device->rtSetInt1(renderer, "sampler.spp", spp);
        device->rtSetInt1(renderer, "printstats", 0);
        device->rtCommit(renderer);
        
        tonemapper = device->rtNewToneMapper("default");
        device->rtSetFloat1(tonemapper, "gamma", gamma);
        device->rtCommit(tonemapper);
        
        frameBuffer = device->rtNewFrameBuffer("RGB_FLOAT32", width, height, numBuffers);
        
        camera = device->rtNewCamera("depthoffield");
        
        fbo.allocate(width, height, GL_RGB);
        
        backplate = NULL;
    }
    
    void Renderer::exit(){
        
        delete device;
    }
    
    void Renderer::updateScene(){
        
        render_scene = createScene();
    }
    
    void Renderer::reset(){
        
        accumulatedTime = 0;
        resetAccumulation = true;
    }
    
    void Renderer::update(){
        
        accumulation = resetAccumulation ? 0 : accumulation+1;
        resetAccumulation = false;
        
        updateCamera();
        
        double t = getSeconds();
        device->rtRenderFrame(renderer, camera, render_scene, tonemapper, frameBuffer, accumulation);
        lastRenderTime = getSeconds() - t;
        accumulatedTime += lastRenderTime;
        device->rtSwapBuffers(frameBuffer);
    }
    
    void Renderer::renderGL(){
        
        void * ptr = device->rtMapFrameBuffer(frameBuffer);
        
        fbo.begin();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawPixels((GLsizei)width, (GLsizei)height, GL_RGBA, GL_FLOAT, ptr);
        fbo.end();
        
        device->rtUnmapFrameBuffer(frameBuffer);
    }
    
    void Renderer::draw(int x, int y){
        
        fbo.draw(x, y);
    }
    
#pragma mark - stats
    
    int Renderer::getAccumulation(){
        return accumulation;
    }
    
    float Renderer::getLastRenderTime(){
        return lastRenderTime;
    }
    
    float Renderer::getAccumulatedTime(){
        return accumulatedTime;
    }
    
#pragma mark - scene
    
    void Renderer::loadScene(string filepath){
        
        std::vector <Device::RTPrimitive> p = embree::loadScene(ofToDataPath(filepath, true), device);
        prims.insert(prims.end(), p.begin(), p.end());
        
        reset();
    }
    
    void Renderer::loadBackPlate(string filepath){
        
        backplate = loadImage(ofToDataPath(filepath, true), device);
        device->rtSetImage(renderer, "backplate", backplate);
        device->rtCommit(renderer);
        
        reset();
    }
    
    void Renderer::addSphere(Material & material, ofPoint pos, float radius, int numTheta, int numPhi){
        
        Device::RTMaterial mat = device->rtNewMaterial(material.type.c_str());
        
        Device::RTShape sphere = device->rtNewShape("sphere");
        device->rtSetFloat3(sphere, "P", pos.x, pos.y, pos.z);
        //g_device->rtSetFloat3(sphere, "dPdt", dPdt.x, dPdt.y, dPdt.z);
        device->rtSetFloat1(sphere, "r", radius);
        device->rtSetInt1(sphere, "numTheta", numTheta);
        device->rtSetInt1(sphere, "numPhi"  , numPhi);
        device->rtCommit(sphere);
        
        AffineSpace3f transform = AffineSpace3f(one);
        Device::RTPrimitive prim = g_device->rtNewShapePrimitive(sphere, mat, copyToArray(transform));
        prims.push_back(prim);
        
        updateScene();
    }
    
    
#pragma mark - lights
    
    void Renderer::addHDRILight(string filepath, ofColor luminosity){
        
        Handle <Device::RTLight> light = device->rtNewLight("hdrilight");
        const Col3f L = toEmbree(luminosity);
        device->rtSetFloat3(light, "L", L.r, L.g, L.b);
        device->rtSetImage(light, "image", loadImage(ofToDataPath(filepath, true), device));
        device->rtCommit(light);
        prims.push_back(device->rtNewLightPrimitive(light, NULL));
    }
    
    void Renderer::addQuadLight(ofPoint P, ofPoint U, ofPoint V, ofColor L){
        
        Handle <Device::RTLight> light0 = device->rtNewLight("trianglelight");
        device->rtSetFloat3(light0, "v0", P.x + U.x + V.x, P.y + U.y + V.y, P.z + U.z + V.z);
        device->rtSetFloat3(light0, "v1", P.x + U.x, P.y + U.y, P.z + U.z);
        device->rtSetFloat3(light0, "v2", P.x, P.y, P.z);
        device->rtSetFloat3(light0, "L",  L.r, L.g, L.b);
        device->rtCommit(light0);
        prims.push_back(device->rtNewLightPrimitive(light0, NULL));
        
        Handle <Device::RTLight> light1 = device->rtNewLight("trianglelight");
        device->rtSetFloat3(light1, "v0", P.x + U.x + V.x, P.y + U.y + V.y, P.z + U.z + V.z);
        device->rtSetFloat3(light1, "v1", P.x, P.y, P.z);
        device->rtSetFloat3(light1, "v2", P.x + V.x, P.y + V.y, P.z + V.z);
        device->rtSetFloat3(light1, "L",  L.r, L.g, L.b);
        device->rtCommit(light1);
        prims.push_back(device->rtNewLightPrimitive(light1, NULL));
    }
    
    
#pragma mark - properties
    
    void Renderer::setGamma(float gamma_){
        gamma = gamma_;
        device->rtSetFloat1(tonemapper, "gamma", gamma);
        device->rtCommit(tonemapper);
    }
    
    void Renderer::setSize(int width_, int height_){
        width = width_;
        height = height_;
        frameBuffer = device->rtNewFrameBuffer("RGB_FLOAT32", width, height, 1);
        fbo.allocate(width, height, GL_RGB);
    }
    
    void Renderer::setSamplePerPixel(int spp_){
        spp = spp_;
        device->rtSetInt1(renderer, "sampler.spp", spp);
        device->rtCommit(renderer);
    }
    
    void Renderer::setRecursionDepth(int depth_){
        depth = depth_;
        device->rtSetInt1(renderer, "maxDepth", depth);
        device->rtCommit(renderer);
    }
    
    void Renderer::setVignetting(bool bVignetting_){
        bVignetting = bVignetting_;
        device->rtSetBool1(tonemapper, "vignetting", bVignetting);
        device->rtCommit(tonemapper);
    }
    
    
#pragma mark - internal
    
    Handle <Device::RTCamera> Renderer::updateCamera(){
        
        Vec3f p = toEmbree(cam->getPosition());
        Vec3f t = toEmbree(cam->getTargetPos());
        Vec3f u = toEmbree(cam->getUpDir());
        AffineSpace3f camSpace = AffineSpace3f::lookAtPoint(p, t, u);
        AffineSpace3f transform = AffineSpace3f(camSpace.l, camSpace.p);
        
        device->rtSetTransform(camera, "local2world", copyToArray(transform));
        device->rtSetFloat1(camera, "angle", cam->getFov());
        device->rtSetFloat1(camera, "aspectRatio", float(width) / float(height));
        device->rtSetFloat1(camera, "lensRadius", cam->getRadius());
        device->rtSetFloat1(camera, "focalDistance", (cam->getTargetPos() - cam->getPosition()).length());
        device->rtCommit(camera);
        return camera;
    }
    
    Handle <Device::RTScene> Renderer::createScene(){
        Device::RTPrimitive * p = (Device::RTPrimitive *)(prims.size() == 0 ? NULL : &prims[0]);
        return device->rtNewScene((accel + " " + tri).c_str(), p, prims.size());
    }
    
}