//
//  Material.h
//  ofxEmbree-Crown
//
//  Created by kikko on 6/27/13.
//
//

#ifndef __ofxEmbree_Crown__Material__
#define __ofxEmbree_Crown__Material__

#include "ofMain.h"

namespace ofxEmbree {
    
    class Material {
        
    public:
        
        Material();
        virtual ~Material();
        
        //Device::RTMaterial embreeMat() { return material; }
        
        string type;
        
        //glass && Dielectric
        ofColor transmission;
        float etaOutside;
        float etaInside;
        
        ofColor transmissionOutside;
        
        // MetallicPaint
        float eta;
        ofColor shadeColor;
        
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
        
    protected:
        
        //Device::RTMaterial material;
    };
}

#endif /* defined(__ofxEmbree_Crown__Material__) */
