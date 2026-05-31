//
//  SprGradient.h
//  sprtools
//
//  BSD 3-Clause License
//
//  Copyright (c) 2026, John Othwolo
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are met:
//
//  1. Redistributions of source code must retain the above copyright notice, this
//     list of conditions and the following disclaimer.

//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//  3. Neither the name of the copyright holder nor the names of its
//     contributors may be used to endorse or promote products derived from
//     this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
//  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
//  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
//  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
//  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//  Created by John Othwolo on 5/24/26.
//

#ifndef SprGradient_h
#define SprGradient_h

#include "compiler.h"

#include <cstdio>
#include <vector>
#include <functional>


struct SprMatrix {
    float a,b,c,d,e,f;
};

namespace SprColor {
    struct ARGB {
        unsigned char alpha;
        unsigned char red;
        unsigned char green;
        unsigned char blue;
    };
    struct RGBA {
        unsigned char red;
        unsigned char green;
        unsigned char blue;
        unsigned char alpha;
    };
    
    struct RGB {
        unsigned char red;
        unsigned char green;
        unsigned char blue;
    };
}

struct SprGradient {
    enum class Type : unsigned char { Invalid = 0, Linear = 3, Radial = 4, };
    enum class SpreadMode : unsigned char { None = 0, Pad = 1, Reflect = 2, Repeat = 3 };
    
    SprGradient(Type type, const char **data);
    SprGradient(void);
    
    std::vector<SprColor::RGBA> colors;
    std::vector<float> positions;
    SpreadMode spreadMode;
    SprMatrix matrix;
    Type type;
    
    union Record {
        struct __packed Linear {
            float x1;
            float x2;
            float y1;
            float y2;
        } linear;
        struct __packed Radial {
            float cx;
            float cy;
            float r;
        } radial;
    } record;
};


#endif /* SprGradient_h */
