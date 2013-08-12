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
    
	class Materials {
        
    public:
        
#define STATIC_ static const char *
        STATIC_ MATTE;
        STATIC_ PLASTIC;
        STATIC_ DIELECTRIC;
        STATIC_ GLASS;
        STATIC_ THIN_DIELECTRIC;
        STATIC_ THIN_GLASS;
        STATIC_ MIRROR;
        STATIC_ METAL;
        STATIC_ METALLIC_PAINT;
        STATIC_ MATTE_TEXTURED;
        STATIC_ OBJ;
        STATIC_ VELVET;
#undef STATIC_
        
        Materials(){}
        virtual ~Materials(){}
        
        void add(string name, string type);
        void add(string name, Device::RTMaterial mat);
        
        void setProp(string name, string pname, int p);
        void setProp(string name, string pname, Vec2i p);
        void setProp(string name, string pname, float p);
        void setProp(string name, string pname, ofVec2f p);
        void setProp(string name, string pname, ofVec3f p);
        void setProp(string name, string pname, ofVec4f p);
        void setProp(string name, string pname, ofFloatColor p);
        
        void update(string name);
        
        Device::RTMaterial get(string name);
        
        /* some default / standard materials */
        
        Device::RTMaterial white();
        Device::RTMaterial gold();
        Device::RTMaterial glass();
        Device::RTMaterial mirror();
        // the next ones get cached based on color so it can fill up the memory quite quickly if continuously generated
        // however if you disable the cache you have to retain the instance yourself with a Handle<>
        Device::RTMaterial matte(ofFloatColor color, bool bCache=true);
        Device::RTMaterial mettalicPaint(ofFloatColor color, bool bCache=true);
        Device::RTMaterial velvet(ofFloatColor color, bool bCache=true);
        Device::RTMaterial plastic(ofFloatColor color, bool bCache=true);
        
        const map<string, Handle <Device::RTMaterial> > & getMap() { return materialMap; }
        
    protected:
        
        map<string, Handle <Device::RTMaterial> > materialMap;
	};
}

#endif /* defined(__ofxEmbree__Materials__) */
