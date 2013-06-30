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
    
#define STATIC_ const char * Materials
    STATIC_::MATTE = "Matte";
    STATIC_::PLASTIC = "Plastic";
    STATIC_::DIELECTRIC = "Dielectric";
    STATIC_::GLASS = "Glass";
    STATIC_::THIN_DIELECTRIC = "ThinDielectric";
    STATIC_::THIN_GLASS = "ThinGlass";
    STATIC_::MIRROR = "Mirror";
    STATIC_::METAL = "Metal";
    STATIC_::METALLIC_PAINT = "MetallicPaint";
    STATIC_::MATTE_TEXTURED = "MatteTextured";
    STATIC_::OBJ = "Obj";
    STATIC_::VELVET = "Velvet";
#undef STATIC_
    
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
    void Materials::setProp(string name, string pname, ofFloatColor p){
        if(!materialMap.count(name)){
            ofLog() << "ofxEmbree ERROR - material " << name << " not found";
            return;
        }
        g_device->rtSetFloat3(materialMap[name], pname.c_str(), p.r, p.g, p.b);
        update(name);
    }
    
    
#pragma mark - Some standard materials
    
    Device::RTMaterial Materials::white(){
        return matte(ofFloatColor(1.0, 1.0, 1.0));
    }
    
    Device::RTMaterial Materials::gold(){
        
        string name = "default_gold";
        if(materialMap.find(name) != materialMap.end()){
            return materialMap[name];
        }
        Handle<Device::RTMaterial> material = g_device->rtNewMaterial( METAL );
        g_device->rtSetFloat3(material, "eta", 0.19, 0.45, 1.50);
        g_device->rtSetFloat3(material, "k", 3.06, 2.40, 1.88);
        g_device->rtSetFloat1(material, "roughness", 0.005f);
        g_device->rtCommit(material);
        materialMap["default_gold"] = material;
        return material;
    }
    
    Device::RTMaterial Materials::glass(){
        
        string name = "default_glass";
        if(materialMap.find(name) != materialMap.end()){
            return materialMap[name];
        }
        Handle<Device::RTMaterial> material = g_device->rtNewMaterial( GLASS );
        g_device->rtSetFloat3(material, "transmission", 1, 1, 1);
        g_device->rtSetFloat1(material, "etaOutside", 1.0f);
        g_device->rtSetFloat1(material, "etaInside", 1.45f);
        g_device->rtCommit(material);
        materialMap[name] = material;
        return material;
    }
    
    Device::RTMaterial Materials::mirror(){
        
        string name = "default_mirror";
        if(materialMap.find(name) != materialMap.end()){
            return materialMap[name];
        }
        Handle<Device::RTMaterial> material = g_device->rtNewMaterial( MIRROR );
        g_device->rtSetFloat3(material, "reflectance", 1.0, 1.0, 1.0);
        g_device->rtCommit(material);
        materialMap["default_mirror"] = material;
        return material;
    }
    
    Device::RTMaterial Materials::mettalicPaint(ofFloatColor color, bool bCache){
        
        string ext = ofToString(int(color.r*255)) + "_" + ofToString(int(color.g*255)) + "_" + ofToString(int(color.b*255));
        string name = "default_metalpaint_" + ext;
        if(materialMap.find(name) != materialMap.end()){
            return materialMap[name];
        }
        Handle<Device::RTMaterial> material = g_device->rtNewMaterial( METALLIC_PAINT );
        g_device->rtSetFloat1(material, "eta", 1.45f);
        g_device->rtSetFloat3(material, "glitterColor", 0.5, 0.44, 0.42);
        g_device->rtSetFloat1(material, "glitterSpread", 0.01f);
        g_device->rtSetFloat3(material, "shadeColor", color.r, color.g, color.b);
        g_device->rtCommit(material);
        if(bCache){
            materialMap[name] = material;
        }
        return material;
    }
    
    Device::RTMaterial Materials::velvet(ofFloatColor color, bool bCache){
        
        string ext = ofToString(int(color.r*255)) + "_" + ofToString(int(color.g*255)) + "_" + ofToString(int(color.b*255));
        string name = "default_velvet_" + ext;
        if(materialMap.find(name) != materialMap.end()){
            return materialMap[name];
        }
        Handle<Device::RTMaterial> material = g_device->rtNewMaterial( VELVET );
        g_device->rtSetFloat3(material, "reflectance", color.r, color.g, color.b);
        g_device->rtSetFloat1(material, "backScattering", 0.5f);
        g_device->rtSetFloat3(material, "horizonScatteringColor", color.r*2, color.g * 2, color.b*2);
        g_device->rtSetFloat1(material, "horizonScatteringFallOff", 10.0f);
        g_device->rtCommit(material);
        if(bCache){
            materialMap[name] = material;
        }
        return material;
    }
    
    Device::RTMaterial Materials::matte(ofFloatColor color, bool bCache){
        
        string ext = ofToString(int(color.r*255)) + "_" + ofToString(int(color.g*255)) + "_" + ofToString(int(color.b*255));
        string name = "default_matte_" + ext;
        if(materialMap.find(name) != materialMap.end()){
            return materialMap[name];
        }
        Handle<Device::RTMaterial> material = g_device->rtNewMaterial( MATTE );
        g_device->rtSetFloat3(material, "reflectance", color.r, color.g, color.b);
        g_device->rtCommit(material);
        materialMap[name] = material;
        return material;
    }
    
    Device::RTMaterial Materials::plastic(ofFloatColor color, bool bCache){
        
        string ext = ofToString(int(color.r*255)) + "_" + ofToString(int(color.g*255)) + "_" + ofToString(int(color.b*255));
        string name = "default_plastic_" + ext;
        if(materialMap.find(name) != materialMap.end()){
            return materialMap[name];
        }
        Handle<Device::RTMaterial> material = g_device->rtNewMaterial( PLASTIC );
        g_device->rtSetFloat3(material, "pigmentColor", color.r, color.g, color.b);
        g_device->rtSetFloat1(material, "eta", 1.4f);
        g_device->rtSetFloat1(material, "roughness", 0.01f);
        g_device->rtSetFloat1(material, "rcpRoughness", rcp(0.01f));
        g_device->rtCommit(material);
        materialMap[name] = material;
        return material;
    }
}