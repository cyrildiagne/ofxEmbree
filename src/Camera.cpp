//
//  Camera.cpp
//  ofxEmbreeExample
//
//  Created by kikko_fr on 6/27/13.
//
//

#include "Camera.h"

namespace ofxEmbree {

    Camera::Camera() {
        
        longitude = 0;
        latitude = 0;
        
        sensitivity = 0.2;
        longitude = dlong = 180;
        latitude = dlat = 0;
        
        radius = 1.0;
    }

    Camera::~Camera() {
        ofRemoveListener(ofEvents().mousePressed, this, &Camera::mousePressed);
        ofRemoveListener(ofEvents().mouseReleased, this, &Camera::mouseReleased);
    }
    
    void Camera::setup(ofPoint targetPt) {
        target = new ofNode();
        target->setPosition(targetPt);
        setup(*target);
    }

    void Camera::setup(ofNode& target) {
        
        setFov(68.f);
        setDistance(300.f);
        setFarClip(20000.f);
        
        setTarget(target);
        
        ofAddListener(ofEvents().mousePressed, this, &Camera::mousePressed);
        ofAddListener(ofEvents().mouseReleased, this, &Camera::mouseReleased);
    }

    void Camera::mousePressed(ofMouseEventArgs& ev) {
        
        initMouse = ofPoint(ofGetMouseX(), ofGetMouseY());
        initLongLat = ofPoint(dlong, dlat);
        ofAddListener(ofEvents().mouseDragged, this, &Camera::mouseDragged);
    }

    void Camera::mouseDragged(ofMouseEventArgs& ev) {
        
        dlong = initLongLat.x - (ofGetMouseX() - initMouse.x) * sensitivity;
        dlat = initLongLat.y - (ofGetMouseY() - initMouse.y) * sensitivity;
    }

    void Camera::mouseReleased(ofMouseEventArgs& ev) {
        ofRemoveListener(ofEvents().mouseDragged, this, &Camera::mouseDragged);
    }

    void Camera::update() {
        
        longitude += (dlong - longitude) * 1;
        latitude += (dlat - latitude) * 1;
        
        ofPoint p = target ? target->getPosition() : ofPoint::zero();
        orbit(longitude, latitude, distance, p);
    }

    void Camera::setTarget(ofNode& target_) {
        
        target = &target_;
    }
    
    ofPoint Camera::getTargetPos() {
        return target->getPosition();
    }

    void Camera::setDistance(float distance_) {
        
        distance = distance_;
    }
    
    float Camera::getRadius() {
        return radius;
    }
    void Camera::setRadius(float radius_) {
        radius = radius_;
    }

}