//
//  Materials.cpp
//  ofxEmbree
//
//  Created by kikko on 6/29/13.
//
//

#include "Materials.h"
#include "ofxEmbree.h"

namespace ofxEmbree {
    
    Device::RTMaterial Materials::get(string name){
        
        return materialMap[name];
    }
    
    void Materials::add(string name, string type){
        
        if(materialMap.find(name) != materialMap.end()){
            ofLog() << "ofxEmbree ERROR - material " << name << " already exists";
            return;
        }
        materialMap[name] = g_device->rtNewMaterial(type.c_str());
    }
    
    void Materials::add(string name, Device::RTMaterial mat){
        
        if(materialMap.find(name) != materialMap.end()){
            ofLog() << "ofxEmbree ERROR - material " << name << " already exists";
            return;
        }
        materialMap[name] = mat;
    }
    
    void Materials::update(string name){
        if(!materialMap.count(name)){
            ofLog() << "ofxEmbree ERROR - material " << name << " not found";
            return;
        }
        g_device->rtCommit(materialMap[name]);
    }
    
    void Materials::setProp(string name, string pname, int p){
        if(!materialMap.count(name)){
            ofLog() << "ofxEmbree ERROR - material " << name << " not found";
            return;
        }
        g_device->rtSetInt1(materialMap[name], pname.c_str(), p);
        update(name);
    }
    void Materials::setProp(string name, string pname, Vec2i p){
        if(!materialMap.count(name)){
            ofLog() << "ofxEmbree ERROR - material " << name << " not found";
            return;
        }
        g_device->rtSetInt2(materialMap[name], pname.c_str(), p.x, p.y);
        update(name);
    }
    void Materials::setProp(string name, string pname, float p){
        if(!materialMap.count(name)){
            ofLog() << "ofxEmbree ERROR - material " << name << " not found";
            return;
        }
        g_device->rtSetFloat1(materialMap[name], pname.c_str(), p);
        update(name);
    }
    void Materials::setProp(string name, string pname, ofVec2f p){
        if(!materialMap.count(name)){
            ofLog() << "ofxEmbree ERROR - material " << name << " not found";
            return;
        }
        g_device->rtSetFloat2(materialMap[name], pname.c_str(), p.x, p.y);
    }
    void Materials::setProp(string name, string pname, ofVec3f p){
        if(!materialMap.count(name)){
            ofLog() << "ofxEmbree ERROR - material " << name << " not found";
            return;
        }
        g_device->rtSetFloat3(materialMap[name], pname.c_str(), p.x, p.y, p.z);
        update(name);
    }
    void Materials::setProp(string name, string pname, ofVec4f p){
        if(!materialMap.count(name)){
            ofLog() << "ofxEmbree ERROR - material " << name << " not found";
            return;
        }
        g_device->rtSetFloat4(materialMap[name], pname.c_str(), p.x, p.y, p.z, p.w);
        update(name);
    }
    void Materials::setProp(string name, string pname, ofColor p){
        if(!materialMap.count(name)){
            ofLog() << "ofxEmbree ERROR - material " << name << " not found";
            return;
        }
        g_device->rtSetFloat3(materialMap[name], pname.c_str(), p.r, p.g, p.b);
        update(name);
    }
    
    
#pragma mark - Some standard materials
    
    Device::RTMaterial Materials::gold(){
        
        if(materialMap.find("default_gold") != materialMap.end()){
            return materialMap["default_gold"];
        }
        Handle<Device::RTMaterial> material = g_device->rtNewMaterial("Metal");
        g_device->rtSetFloat3(material, "eta", 0.19, 0.45, 1.50);
        g_device->rtSetFloat3(material, "k", 3.06, 2.40, 1.88);
        g_device->rtSetFloat1(material, "roughness", 0.005f);
        g_device->rtCommit(material);
        
        materialMap["default_gold"] = material;
        
        return material;
    }
    
    Device::RTMaterial Materials::glass(){
        
        if(materialMap.find("default_glass") != materialMap.end()){
            return materialMap["default_glass"];
        }
        Handle<Device::RTMaterial> material = g_device->rtNewMaterial("Glass");
        g_device->rtSetFloat3(material, "transmission", 1, 1, 1);
        g_device->rtSetFloat1(material, "etaOutside", 1.0f);
        g_device->rtSetFloat1(material, "etaInside", 1.45f);
        g_device->rtCommit(material);
        
        materialMap["default_glass"] = material;
        
        return material;
    }
    
    Device::RTMaterial Materials::white(){
        
        if(materialMap.find("default_white") != materialMap.end()){
            return materialMap["default_white"];
        }
        Handle<Device::RTMaterial> material = g_device->rtNewMaterial("Matte");
        g_device->rtSetFloat3(material, "reflectance", 1.0, 1.0, 1.0);
        g_device->rtCommit(material);
        
        materialMap["default_white"] = material;
        
        return material;
    }
}