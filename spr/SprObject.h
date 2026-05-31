//
//  SprObject.h
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

#ifndef SprObject_h
#define SprObject_h

#include <vector>
#include "compiler.h"
#include "SprAttribute.h"

struct SprDocument;

struct SprObject : public std::enable_shared_from_this<SprObject> {
public:
    struct Raw {
        uint8_t type;
        union {
            struct { uint32_t size; char data[]; } withSize;
            struct { char data[]; } noSize;
        };
    } __packed;
    struct List {
        uint32_t count;
        Raw objs[];
    } __packed;
    
    enum class Type {
        None      = 0,
        Circle    = 1,
        Ellipse   = 2,
        Line      = 3,
        Path      = 4,
        Rectangle = 5,
        Group     = 16,
        Use       = 17,
    };
    
    std::vector<std::unique_ptr<SprAttribute>> attributes;
    SprDocument const &document;
    Type type;
    
    SprObject(SprDocument const &doc);
protected:
    SprObject(SprDocument const &doc, Type type);
    uint32_t loadAttrData(char const *attrdata);
};

struct SprObjectCircle : public SprObject {
    SprObjectCircle(SprDocument const &doc, const char **objdata);
    
    struct __packed Record {
        float cr;
        float cx;
        float cy;
    } record;
};

struct SprObjectEllipse : public SprObject {
    SprObjectEllipse(SprDocument const &doc, const char **objdata);
    
    struct __packed Record {
        float bottom;
        float left;
        float right;
        float top;
    } record;
};

struct SprObjectGroup : public SprObject {
protected:
    SprObjectGroup(SprDocument const &doc, const char **objdata, bool isRoot);
public:
    SprObjectGroup(SprDocument const &doc, const char **objdata);
    
    std::vector<std::shared_ptr<SprObject>> objects;
    const bool isroot;
};

struct SprRootObjectGroup : public SprObjectGroup {
    SprRootObjectGroup(SprDocument const &doc, const char **objdata);
};


struct SprObjectLine : public SprObject {
    SprObjectLine(SprDocument const &doc, const char **objdata);
    
    struct __packed Record {
        float x1;
        float x2;
        float y1;
        float y2;
    } record;
};

struct SprObjectPath : public SprObject {
    struct Command {
        static Command swapped(Command *, uint32_t);
        enum class Type : unsigned char {
            NONE = 0,
            MOVETO = 1,
            LINETO = 2,
            QUADRATIC_CURVETO = 3,
            BEZIER_CURVETO = 4,
            ELLIPTICAL_ARC = 5,
            CLOSE = 6,
        };
        
        Type type;
        union {
            struct __packed {
                float x;
                float y;
            } moveto;
            struct __packed {
                float x;
                float y;
            } lineto;
            struct __packed {
                float x;
                float y;
                float lastX;
                float lastY;
            } quadratic_curveto;
            struct __packed {
                float x1;
                float y1;
                float x2;
                float y2;
                float lastX;
                float lastY;
            } bezier_curveto;
            struct __packed {
                float x1;
                float y1;
                float x2;
                float y2;
                float startAngle;
                float endAngle;
            } elliptical_arc;
            float values[];
        } u;
    } __packed;
    struct List {
        uint32_t count;
        Command commands[];
    };
    SprObjectPath(SprDocument const &doc, const char **objdata);
    
    std::vector<Command> commands;
};

struct SprObjectRectangle : public SprObject {
    SprObjectRectangle(SprDocument const &doc, const char **objdata);
    
    struct __packed Record {
        float bottom;
        float left;
        float right;
        float rx;
        float ry;
        float top;
    } record;
};

struct SprObjectUse : public SprObject {
    SprObjectUse(SprDocument const &doc, const char **objdata);
    
    struct __packed Record {
        float link;
    } record;
};


#endif /* SprObject_h */
