//
//  Renderer.h
//  ofxEmbreeExample
//
//  Created by kikko_fr on 6/26/13.
//
//

#ifndef __ofxEmbreeExample__Renderer__
#define __ofxEmbreeExample__Renderer__

#include "ofMain.h"

#include "ofxEmbree.h"
#include "VHandle.h"
#include "Material.h"
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
        
        void addMaterial(string name, string type);
        void updateMaterial(string name);
        void setMaterialProp(string name, string pname, int p);
        void setMaterialProp(string name, string pname, Vec2i p);
        void setMaterialProp(string name, string pname, float p);
        void setMaterialProp(string name, string pname, ofVec2f p);
        void setMaterialProp(string name, string pname, ofVec3f p);
        void setMaterialProp(string name, string pname, ofVec4f p);
        
        Handle<Device::RTShape> addSphere(string matName, ofSpherePrimitive& sphere);
        Handle<Device::RTShape> addSphere(string matName, ofPoint pos, float radius, int numTheta=50, int numPhi=50);
        Handle<Device::RTShape> addMesh(string matName, ofMesh & mesh);
        Handle<Device::RTShape> addMesh(string matName, ofMesh & mesh, const ofMatrix4x4& transform);
        Handle<Device::RTShape> addShape(string matName, Device::RTShape shape);
        Handle<Device::RTShape> addShape(string matName, Device::RTShape shape, const ofMatrix4x4& transform);
        Handle<Device::RTShape> addShape(string matName, Device::RTShape shape, const AffineSpace3f& transform);
        
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
        Handle<Device::RTCamera> updateCamera();
        
        Camera * cam;
        ofFbo fbo;
        map<string, Handle<Device::RTMaterial> > materialMap;
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


#endif /* defined(__ofxEmbreeExample__Renderer__) */
