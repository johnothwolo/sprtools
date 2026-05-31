//
//  SprAttribute.cpp
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

#include "SprAttribute.h"
#include "spr.h"



SprColoredAttribute::SprColoredAttribute(Type type, const char **attrdata) : SprAttribute(type)
{
    colorType = *(ColorType*)*attrdata;
    (*attrdata) += sizeof(ColorType);
    
    switch (colorType) {
        case ColorType::Link:
            throw "links are not yet implemented";
        case ColorType::ARGB:
            bswap_inline(&color, *attrdata);
            (*attrdata) += sizeof(color);
            break;
        case ColorType::LinearGradient:
            gradient = SprGradient(SprGradient::Type::Linear, attrdata);
            break;
        case ColorType::RadialGradient:
            gradient = SprGradient(SprGradient::Type::Radial, attrdata);
            break;
        case ColorType::None:
            break;
        default:
            throw "unknown color-fill type";
    }
}

SprAttributeAnimatorSet::SprAttributeAnimatorSet(const char **attrdata) : SprAttribute(Type::AnimatorSet) 
{
    throw "unimplemented";
}

SprAttributeClip::SprAttributeClip(const char **attrdata) : SprAttribute(Type::Clip)
{
    bswap_inline(&record.left, *attrdata + offsetof(Record, left));
    bswap_inline(&record.top, *attrdata + offsetof(Record, top));
    bswap_inline(&record.right, *attrdata + offsetof(Record, right));
    bswap_inline(&record.bottom, *attrdata + offsetof(Record, bottom));
    (*attrdata) += sizeof(Record);
}

SprAttributeClipPath::SprAttributeClipPath(const char **attrdata) : SprAttribute(Type::ClipPath)
{
    bswap_inline(&record.link, *attrdata);
    (*attrdata) += sizeof(Record);
}

SprAttributeMatrix::SprAttributeMatrix(const char **attrdata) : SprAttribute(Type::Matrix)
{
    bswap_inline(&record.matrix.a, *attrdata + offsetof(SprMatrix, a));
    bswap_inline(&record.matrix.b, *attrdata + offsetof(SprMatrix, b));
    bswap_inline(&record.matrix.c, *attrdata + offsetof(SprMatrix, c));
    bswap_inline(&record.matrix.d, *attrdata + offsetof(SprMatrix, d));
    bswap_inline(&record.matrix.e, *attrdata + offsetof(SprMatrix, e));
    bswap_inline(&record.matrix.f, *attrdata + offsetof(SprMatrix, f));
    (*attrdata) += sizeof(Record);
}

SprAttributeShadow::SprAttributeShadow(const char **attrdata) : SprAttribute(Type::Shadow)
{
    bswap_inline(&record.radius, *attrdata + offsetof(Record, radius));
    bswap_inline(&record.dx, *attrdata + offsetof(Record, dx));
    bswap_inline(&record.dy, *attrdata + offsetof(Record, dy));
    bswap_inline(&record.shadowColor, *attrdata + offsetof(Record, shadowColor));
    (*attrdata) += sizeof(Record);
}

SprAttributeStrokeLinecap::SprAttributeStrokeLinecap(const char **attrdata) : SprAttribute(Type::StrokeLinecap)
{
    record.linecap = *(uint8_t*) attrdata;
    (*attrdata) += sizeof(Record);
}

SprAttributeStrokeLinejoin::SprAttributeStrokeLinejoin(const char **attrdata) : SprAttribute(Type::StrokeLinejoin)
{
    record.linejoin = *(uint8_t*) attrdata;
    (*attrdata) += sizeof(Record);
}

SprAttributeStrokeMiterlimit::SprAttributeStrokeMiterlimit(const char **attrdata) : SprAttribute(Type::StrokeMiterlimit)
{
    bswap_inline(&record.miterlimit, *attrdata + offsetof(Record, miterlimit));
    (*attrdata) += sizeof(Record);
}

SprAttributeStrokeWidth::SprAttributeStrokeWidth(const char **attrdata) : SprAttribute(Type::StrokeWidth)
{
    bswap_inline(&record.strokewidth, *attrdata + offsetof(Record, strokewidth));
    (*attrdata) += sizeof(Record);
}

SprAttributeFill::SprAttributeFill(const char **attrdata) : SprColoredAttribute(Type::Fill, attrdata)
{
    
}

SprAttributeStroke::SprAttributeStroke(const char **attrdata) : SprColoredAttribute(Type::Stroke, attrdata)
{
    
}

