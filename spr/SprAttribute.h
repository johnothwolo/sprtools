//
//  SprAttribute.h
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

#ifndef SprAttribute_h
#define SprAttribute_h

#include <vector>
#include "compiler.h"
#include "SprGradient.h"

struct SprAttribute {
    struct Raw {
        uint8_t type;
        union {
            struct { uint32_t size; char data[0]; } withSize;
            struct { char data[0]; } noSize;
        };
    } __packed;
    
    struct List {
        uint32_t count;
        Raw items[0];
    };
    
    enum class Type {
        None             = 0,
        Clip             = 1,
        ClipPath         = 3,
        Fill             = 32,
        Stroke           = 35,
        StrokeLinecap    = 37,
        StrokeLinejoin   = 38,
        StrokeWidth      = 40,
        StrokeMiterlimit = 41,
        Matrix           = 64,
        Duration         = 96,
        AnimatorSet      = 97,
        Shadow           = 112,
    };
    
    Type type;
    
    SprAttribute(void) : type(Type::None) {}
protected:
    SprAttribute(Type type) : type(type) {}
};

struct SprAttributeAnimatorSet : public SprAttribute {
    SprAttributeAnimatorSet(const char **attrdata);
    struct __packed Record  {
        uint32_t startOffset;
        uint32_t duration;
        uint32_t repeatCount;
    } record;
};

struct SprAttributeClip : public SprAttribute {
    SprAttributeClip(const char **attrdata);
    struct __packed Record {
        float left;
        float top;
        float right;
        float bottom;
    } record;
};

struct SprAttributeClipPath : public SprAttribute {
    SprAttributeClipPath(const char **attrdata);
    struct __packed Record {
        uint32_t link;
    } record;
};

struct SprAttributeMatrix : public SprAttribute {
    SprAttributeMatrix(const char **attrdata);
    struct __packed Record {
        SprMatrix matrix;
    } record;
};

struct SprAttributeShadow : public SprAttribute {
    SprAttributeShadow(const char **attrdata);
    
    struct __packed Record {
        float radius;
        float dx;
        float dy;
        float shadowColor;
    } record;
};

struct SprAttributeStrokeLinecap : public SprAttribute {
    SprAttributeStrokeLinecap(const char **attrdata);
    
    struct __packed Record {
        uint8_t linecap;
    } record;
    
};

struct SprAttributeStrokeLinejoin : public SprAttribute {
    SprAttributeStrokeLinejoin(const char **attrdata);
    
    struct __packed Record {
        uint8_t linejoin;
    } record;
};

struct SprAttributeStrokeMiterlimit : public SprAttribute {
    SprAttributeStrokeMiterlimit(const char **attrdata);
    
    struct __packed Record {
        float miterlimit;
    } record;
};

struct SprAttributeStrokeWidth : public SprAttribute {
    SprAttributeStrokeWidth(const char **attrdata);
    
    struct __packed Record {
        float strokewidth;
    } record;
};

struct SprColoredAttribute : SprAttribute {
    enum class ColorType : unsigned char {
        None = 0,
        ARGB = 1,
        Link = 2,
        LinearGradient = (unsigned char) SprGradient::Type::Linear,
        RadialGradient = (unsigned char) SprGradient::Type::Radial,
    };
    
    SprColor::ARGB color;
    ColorType colorType = ColorType::None;
    SprGradient gradient;
    
    
protected:
    SprColoredAttribute(Type type, const char **attrdata);
};

struct SprAttributeFill : public SprColoredAttribute {
    SprAttributeFill(const char **attrdata);
    
};

struct SprAttributeStroke : public SprColoredAttribute {
    SprAttributeStroke(const char **attrdata);
    
};

#endif /* SprAttribute_h */
