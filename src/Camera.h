//
//  Camera.h
//  ofxEmbree
//
//  Created by kikko_fr on 6/27/13.
//
//

#ifndef __ofxEmbree__Camera__
#define __ofxEmbree__Camera__

#include "ofMain.h"

namespace ofxEmbree {
    
    class Camera : public ofCamera {
        
    public:
        
        Camera();
        virtual ~Camera();
        
        void setup(ofNode &target);
        void setup(ofPoint targetPt);
        void update();
        
        void setTarget(ofNode &target);
        ofPoint getTargetPos();
        
        void setDistance(float distance);
        
        float distance;
        
        float getRadius();
        void setRadius(float radius);
        
    protected:
        
        void mousePressed(ofMouseEventArgs& ev);
        void mouseDragged(ofMouseEventArgs& ev);
        void mouseReleased(ofMouseEventArgs& ev);
        
        ofNode* target;
        
        ofPoint initMouse, initLongLat;
        float sensitivity;
        float longitude, latitude;
        float dlong, dlat;
        
        float radius;
    };
}

#endif /* defined(__ofxEmbreeExample__Camera__) */
