//
//  Renderer.h
//  ofxEmbreeExample
//
//  Created by kikko_fr on 6/26/13.
//
//

#ifndef __ofxEmbree__Renderer__
#define __ofxEmbree__Renderer__

#include "ofMain.h"

#include "ofxEmbree.h"
#include "VHandle.h"
#include "Materials.h"
#include "Camera.h"

using namespace embree;

namespace ofxEmbree {
    
    class Renderer {
        
    public:
        Renderer();
        virtual ~Renderer();
        
        void setup(Camera& cam, int numThreads=0);
        void exit();
        void update();
        void draw(int x, int y);
        void renderGL();
        
        void buildScene();
        void reset();
        
        void loadScene(string filepath);
        void loadBackPlate(string filepath);
        
        void addHDRILight(string filepath, ofColor luminosity);
        void addQuadLight(ofPoint position, ofPoint u, ofPoint v, ofColor luminosity);
        //TODO : addAmbientLight
        //TODO : addPointLight
        //TODO : addDirectionalLight
        //TODO : addDistantLight
        //TODO : addTriangleLight
        
        Handle<Device::RTShape> addSphere(Device::RTMaterial material, ofSpherePrimitive& sphere);
        Handle<Device::RTShape> addSphere(Device::RTMaterial material, ofPoint pos, float radius, int numTheta=50, int numPhi=50);
        Handle<Device::RTShape> addMesh(Device::RTMaterial material, ofMesh & mesh);
        Handle<Device::RTShape> addMesh(Device::RTMaterial material, ofMesh & mesh, const ofMatrix4x4& transform);
        Handle<Device::RTShape> addShape(Device::RTMaterial material, Device::RTShape shape);
        Handle<Device::RTShape> addShape(Device::RTMaterial material, Device::RTShape shape, const ofMatrix4x4& transform);
        Handle<Device::RTShape> addShape(Device::RTMaterial material, Device::RTShape shape, const AffineSpace3f& transform);
        
        void setSize(int width, int height);
        void setSamplePerPixel(int spp);
        void setRecursionDepth(int depth);
        void setGamma(float gamma);
        void setVignetting(bool bVigneting);
        
        int getAccumulation();
        float getLastRenderTime();
        float getAccumulatedTime();
        
        Device * getDevice() { return device; }
        ofFbo& getFbo() { return fbo; }
        vector<Handle<Device::RTPrimitive> >& getPrimitives() { return prims; }
        
    protected:
        
        Handle<Device::RTScene> createScene();
        void updateCamera();
        
        Camera * cam;
        ofFbo fbo;
        map<string, Handle<Device::RTShape> > meshMap;
        
        size_t width , height;
        
        int depth;                       //!< recursion depth
        int spp;                         //!< samples per pixel for ordinary rendering
        
        // output settings
        int numBuffers;                   //!< number of buffers of the framebuffer
        float gamma;
        bool bVignetting;
        
        // rendering settings
        string accel;
        string tri;
        
        // display settings
        bool resetAccumulation;
        int accumulation;
        
        //stats
        float lastRenderTime;
        float accumulatedTime;
        
        // internal object
        Device * device;
        Handle<Device::RTCamera> camera;
        Handle<Device::RTRenderer> renderer;
        Handle<Device::RTToneMapper> tonemapper;
        Handle<Device::RTFrameBuffer> frameBuffer;
        Handle<Device::RTImage> backplate;
        Handle<Device::RTScene> render_scene;
        vector<Handle<Device::RTPrimitive> > prims;
        
        ofMatrix4x4 identity;
    };
    
}


#endif /* defined(__ofxEmbree__Renderer__) */
