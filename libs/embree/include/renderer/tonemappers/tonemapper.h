// ======================================================================== //
// Copyright 2009-2012 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#ifndef __EMBREE_TONEMAPPER_H__
#define __EMBREE_TONEMAPPER_H__

#include "api/parms.h"
#include "api/framebuffer.h"

namespace embree
{
  /*! Interface to different tonemappers. */
  class ToneMapper : public RefCount {
    ALIGNED_CLASS
  public:

    /*! Tonemappers need a virtual destructor. */
    virtual ~ToneMapper() {}

    /*! Evaluates the tonemapper, */
    virtual void eval (      Ref<FrameBuffer>& dst, /*!< source buffer. */
                       const Ref<FrameBuffer>& src, /*!< target buffer. */
                       Vec2i start,                 /*!< first pixel to process */
                       Vec2i end                    /*!< last pixel to process */) const
    {
    }
  };
}

#endif
