//
//  Material.h
//  ofxEmbree-Crown
//
//  Created by kikko on 6/27/13.
//
//

#ifndef __ofxEmbree__Materials__
#define __ofxEmbree__Materials__

#include "ofMain.h"
#include "renderer/api/device.h"
#include "math/vec2.h"
#include "VHandle.h"

using namespace embree;

namespace ofxEmbree {
    
	//extern Device* g_device;
    
	class Materials {
        
    public:
        
        Materials(){
        }
        virtual ~Materials(){
        }
        
        void add(string name, string type);
        void add(string name, Device::RTMaterial mat);
        
        void setProp(string name, string pname, int p);
        void setProp(string name, string pname, Vec2i p);
        void setProp(string name, string pname, float p);
        void setProp(string name, string pname, ofVec2f p);
        void setProp(string name, string pname, ofVec3f p);
        void setProp(string name, string pname, ofVec4f p);
        void setProp(string name, string pname, ofColor p);
        
        void update(string name);
        
        Device::RTMaterial get(string name);
        
        // some default / standard materials
        Device::RTMaterial white();
        Device::RTMaterial gold();
        Device::RTMaterial glass();
        
        const map<string, Handle <Device::RTMaterial> > & getMap() { return materialMap; }
        
        /*
         //glass && Dielectric
         ofColor transmission;
         float etaOutside;
         float etaInside;
         
         ofColor transmissionOutside;
         
         // MetallicPaint
         float eta;
         ofColor shadeColor;
         ofColor glitterColor;
         float glitterSpread;
         
         // Metal
         ofPoint k;
         float roughness;
         
         // MatteTextured
         string Kd;
         float s0[2];
         float ds[2];
         
         // Matte
         ofPoint reflectance;
         
         // Velvet
         float backScattering;
         ofPoint horizonScatteringColor;
         float horizonScatteringFallOff;
         */
        
    protected:
        
        map<string, Handle <Device::RTMaterial> > materialMap;
	};
}

#endif /* defined(__ofxEmbree__Materials__) */
