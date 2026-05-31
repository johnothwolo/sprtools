//
//  SprFileAttribute.h
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

#ifndef SprFileAttribute_h
#define SprFileAttribute_h

#include <vector>
#include "compiler.h"

struct SprFileAttribute {
    struct Raw {
        uint8_t type;
        uint32_t size;
        char data[];
    } __packed;

    struct List {
        uint32_t count;
        Raw attrs[];
    };
    enum class Type {
        None = 0,
        NinePatch = 1,
    };
    
    Type type;
    
    SprFileAttribute(void) : type(Type::None) {}
protected:
    SprFileAttribute(Type type) : type(type) {}
    
};

class SprFileAttributeNinePatch : public SprFileAttribute {
    // layout:
    //    uint32_t xSize;
    //    uint32_t xStart[...];
    //    uint32_t xEnd[...];
    //    uint32_t ySize;
    //    uint32_t yStart[...];
    //    uint32_t yEnd[...];
    
    struct NinePatch {
        std::vector<uint32_t> start;
        std::vector<uint32_t> end;
    };
    
    NinePatch parseNp(const char* attrData)
    {
        uint32_t *xdata = (uint32_t*) attrData, xvsize = *xdata;
        uint32_t *xstartv = &xdata[1], *xendv = &xstartv[xvsize];
        uint32_t *ydata = &xendv[xvsize], yvsize = *ydata;
        uint32_t *ystartv = &ydata[1], *yendv = &ystartv[yvsize];
        return {std::vector<uint32_t>(ystartv, yendv), std::vector<uint32_t>(yendv, &yendv[yvsize])};
    }
    
    uint32_t getNpSize(const char* attrData)
    {
        uint32_t *xdata = (uint32_t*) attrData, xvsize = *xdata;
        uint32_t *xstartv = &xdata[1], *xendv = &xstartv[xvsize];
        uint32_t *ydata = &xendv[xvsize], yvsize = *ydata;
        uint32_t *ystartv = &ydata[1], *yendv = &ystartv[yvsize];
        return (uint32_t)((char*)&yendv[yvsize] - attrData);
    }
    
public:
    SprFileAttributeNinePatch(const char* attrData)
    : SprFileAttribute(Type::NinePatch), x(parseNp(attrData)), y{parseNp(attrData + 4 + x.start.size() * 2)}
    {
    }
    
    const NinePatch x;
    const NinePatch y;
};


#endif /* SprFileAttribute_h */
