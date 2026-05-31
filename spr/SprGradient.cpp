//
//  SprGradient.cpp
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

#include "SprGradient.h"

SprGradient::SprGradient(Type type, const char **data)
{
    int length;
    
    spreadMode = *(SpreadMode*) *data; (*data) += sizeof(SpreadMode);
    bswap_inline(&length, *data); (*data) += sizeof(length);
    
    assert(length > 0);
    
    for (int i = 0; i < length; i++) {
        struct {
            char pad; // FIXME: not sure how RGB is store in a 4byte integer
            SprColor::RGB rgb;
        } colorval;
        SprColor::RGBA color;
        float offset, opacity;
        
        bswap_inline(&offset, (*data)++);
        bswap_inline(&colorval, (*data)++);
        bswap_inline(&opacity, (*data)++);
        
        color.red = colorval.rgb.red;
        color.green = colorval.rgb.green;
        color.blue = colorval.rgb.blue;
        color.alpha = opacity;
        
        colors.push_back(color);
        positions.push_back(offset);
    }
    
    switch (type) {
        case Type::Linear:
            bswap_inline(&record.linear.x1, *data + offsetof(Record::Linear, x1));
            bswap_inline(&record.linear.x2, *data + offsetof(Record::Linear, x2));
            bswap_inline(&record.linear.y1, *data + offsetof(Record::Linear, y1));
            bswap_inline(&record.linear.y2, *data + offsetof(Record::Linear, y2));
            (*data) += sizeof(record.linear);
            break;
        case Type::Radial:
            bswap_inline(&record.radial.cx, *data + offsetof(Record::Radial, cx));
            bswap_inline(&record.radial.cy, *data + offsetof(Record::Radial, cy));
            bswap_inline(&record.radial.r, *data + offsetof(Record::Radial, r));
            (*data) += sizeof(record.radial);
        default:
            throw "Invalid graident type";
    }
    
}

SprGradient::SprGradient(void)
{
}
