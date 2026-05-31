//
//  spr.h
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

#ifndef spr_h
#define spr_h

#include "compiler.h"
#include "SprFileAttribute.h"
#include "SprAttribute.h"
#include "SprObject.h"

#include <string>

#define SPR_MAGIC ((unsigned int)'SPR\0')
#define SVF_MAGIC ((unsigned int)'SVF\0')
 
#define HAS_SIZE_FIELD(major, minor) (major >= 12336 && minor >= 12338)

enum class SprRepeatMode {
    RESTART = 2,
    REVERSE = 1,
};

struct SprRect {
    float x;
    float y;
    float width;
    float height;
} __packed;

struct SprBoundModifier {
    float bottom;
    float right;
    float left;
    float top;
} __packed;

struct SprHeaderBase {
    uint32_t magic;
    uint16_t majorvers;
    uint16_t minorvers;
    uint32_t referenceSectionOffset;
    uint32_t documentOffset;
    uint32_t fileAttributeOffset;
    uint32_t unk1[2];
    SprRect dimensions;
    SprBoundModifier ninepatch;
    SprBoundModifier padding;
    uint32_t density;
} __packed;

struct SprHeader33 {
    SprHeaderBase base;
    uint32_t documentCount;
    uint32_t repeatCount;
    uint8_t repeatMode;
} __packed;

struct SprHeader34 {
    SprHeaderBase base;
    uint32_t documentCount;
    uint32_t repeatCount;
    uint8_t  repeatMode;
    uint32_t mAnimationMode;
    uint32_t mAnimationInterval;
} __packed;

union SprHeader {
    SprHeader(const char *filedata);
    
    SprHeaderBase base;
    SprHeader33 h33;
    SprHeader34 h34;
};

struct SprRefrenceObjectElement {
    uint32_t pad;
    uint8_t type;
    union {
        struct {
            uint32_t size;
            char data[];
        } withSize;
        struct {
            char data[];
        } noSize;
    };
} __packed;

struct SprRefrenceObjectList {
    uint32_t count;
    SprRefrenceObjectElement objs[];
};

struct SprData {
    uint32_t size;
    char bytes[];
};

struct SprDocument {
    SprDocument(const char *filedata);
    ~SprDocument();
    int GetVersionMajor() const;
    int GetVersionMinor() const;
    void add_animation(std::shared_ptr<SprObject> const) const;
    
    const std::string& drawSvg(void);
    
private:
    std::vector<std::shared_ptr<SprObject>> animations;
    std::vector<std::shared_ptr<SprObject>> refobjects;
    std::vector<std::unique_ptr<SprRootObjectGroup>> documents;
    std::vector<std::unique_ptr<SprFileAttribute>> fileattrs;
    std::string cached_svg;
public:
    const char *filedata;
    const SprHeader header;
};


#endif /* spr_h */
