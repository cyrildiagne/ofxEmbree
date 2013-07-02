//
//  ofxEmbree.h
//  ofxEmbree
//
//  Created by kikko_fr on 6/27/13.
//
//

#ifndef ofxEmbreeExample_ofxEmbree_h
#define ofxEmbreeExample_ofxEmbree_h

#include "sys/platform.h"
#include "sys/filename.h"
#include "sys/ref.h"
#include "lexers/streamfilters.h"
#include "lexers/parsestream.h"
#include "loaders/loaders.h"
#include "math/math.h"
#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include "math/col3.h"
#include "math/affinespace.h"
#include "image/image.h"
#include "renderer/api/device.h"
#include "renderer/api/handle.h"

#include "Renderer.h"
#include "Camera.h"
#include "Material.h"
#include "Vhandle.h"

namespace ofxEmbree {
    
    inline embree::Vec3f toEmbree(ofPoint pt) {
        return embree::Vec3f(pt.x, pt.y, pt.z);
    }
    
    inline embree::Col3f toEmbree(ofColor col) {
        return embree::Col3f(col.r, col.g, col.b);
    }
}

#endif
