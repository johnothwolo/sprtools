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

#include "SprObject.h"
#include "spr.h"

SprObject::SprObject(SprDocument const &doc): SprObject(doc, Type::None)
{
}

SprObject::SprObject(SprDocument const &doc, Type type) : document(doc), type(type)
{
}

uint32_t SprObject::loadAttrData(const char *attrdata)
{
    SprAttribute::List *list = (SprAttribute::List*) attrdata;
    SprAttribute::Raw *iter = &list->items[0];
    bool hasSize = HAS_SIZE_FIELD(document.GetVersionMajor(), document.GetVersionMinor());
    
    for (int i = 0, size = 0; i < bswap32(list->count); i++) {
        const char *data = hasSize ? &iter->withSize.data[0] : &iter->noSize.data[0], *cursor = data;
        
        switch (iter->type) {
            case (int) SprAttribute::Type::Clip:
                attributes.push_back(std::make_unique<SprAttributeClip>(&cursor));
                break;
            case (int) SprAttribute::Type::ClipPath:
                attributes.push_back(std::make_unique<SprAttributeClipPath>(&cursor));
                break;
            case (int) SprAttribute::Type::Fill:
                attributes.push_back(std::make_unique<SprAttributeFill>(&cursor));
                break;
            case (int) SprAttribute::Type::Stroke:
                attributes.push_back(std::make_unique<SprAttributeStroke>(&cursor));
                break;
            case (int) SprAttribute::Type::Matrix:
                attributes.push_back(std::make_unique<SprAttributeMatrix>(&cursor));
                break;
            case (int) SprAttribute::Type::AnimatorSet:
                attributes.push_back(std::make_unique<SprAttributeAnimatorSet>(&cursor));
                document.add_animation(shared_from_this());
                break;
            case (int) SprAttribute::Type::Shadow:
                attributes.push_back(std::make_unique<SprAttributeShadow>(&cursor));
                break;
            case (int) SprAttribute::Type::StrokeLinecap:
                attributes.push_back(std::make_unique<SprAttributeStrokeLinecap>(&cursor));
                break;
            case (int) SprAttribute::Type::StrokeLinejoin:
                attributes.push_back(std::make_unique<SprAttributeStrokeLinejoin>(&cursor));
                break;
            case (int) SprAttribute::Type::StrokeWidth:
                attributes.push_back(std::make_unique<SprAttributeStrokeWidth>(&cursor));
                break;
            case (int) SprAttribute::Type::StrokeMiterlimit:
                attributes.push_back(std::make_unique<SprAttributeStrokeMiterlimit>(&cursor));
                break;
            case (int) SprAttribute::Type::None:
                break;
            default:
                fprintf(stderr, "Unknown attribute", (int) iter->type); throw "";
        }
        
        if (hasSize){
            size = bswap32(iter->withSize.size);
            assert(size == (uint32_t)(cursor - data));
        } else {
            size = (uint32_t)(cursor - data);
        }
        
        size += (uint32_t)(data - (char*)iter);
        iter = (SprAttribute::Raw *)((char*)iter + size);
    }
    
    return (uint32_t)((char*)iter - (char*)list);
}

SprObjectCircle::SprObjectCircle(SprDocument const &doc, const char **objdata): SprObject(doc, Type::Circle)
{
    bswap_inline(&record.cx, *objdata + offsetof(Record, cx));
    bswap_inline(&record.cy, *objdata + offsetof(Record, cy));
    bswap_inline(&record.cr, *objdata + offsetof(Record, cr));
    (*objdata) += sizeof(record) + loadAttrData((*objdata) + sizeof(record));
}

SprObjectEllipse::SprObjectEllipse(SprDocument const &doc, const char **objdata): SprObject(doc, Type::Ellipse)
{
    bswap_inline(&record.left, *objdata + offsetof(Record, left));
    bswap_inline(&record.top, *objdata + offsetof(Record, top));
    bswap_inline(&record.right, *objdata + offsetof(Record, right));
    bswap_inline(&record.bottom, *objdata + offsetof(Record, bottom));
    (*objdata) += sizeof(record) + loadAttrData((*objdata) + sizeof(record));
}


SprObjectGroup::SprObjectGroup(SprDocument const &doc, const char **objdata, bool isRoot) : SprObject(doc, Type::Group), isroot(isRoot)
{
    SprObject::List *list = (SprObject::List *) *objdata;
    SprObject::Raw *iter = &list->objs[0];
    
    for (int i = 0, size = 0; i < bswap32(list->count); i++) {
        bool hasSize = HAS_SIZE_FIELD(document.GetVersionMajor(), document.GetVersionMinor());
        const char *data = hasSize ? &iter->withSize.data[0] : &iter->noSize.data[0], *cursor = data;
        
        switch (iter->type) {
            case (int) Type::Circle:
                objects.push_back(std::make_shared<SprObjectCircle>(doc, &cursor)); break;
            case (int) Type::Ellipse:
                objects.push_back(std::make_shared<SprObjectEllipse>(doc, &cursor)); break;
            case (int) Type::Line:
                objects.push_back(std::make_shared<SprObjectLine>(doc, &cursor)); break;
            case (int) Type::Path:
                objects.push_back(std::make_shared<SprObjectPath>(doc, &cursor)); break;
            case (int) Type::Rectangle:
                objects.push_back(std::make_shared<SprObjectRectangle>(doc, &cursor)); break;
            case (int) Type::Group:
                objects.push_back(std::make_shared<SprObjectGroup>(doc, &cursor)); break;
            case (int) Type::Use:
                objects.push_back(std::make_shared<SprObjectUse>(doc, &cursor)); break;
            default:
                fprintf(stderr, "unknown element type: %d", (int) type);
                throw "";
        }
        
        if (hasSize){
            size = bswap32(iter->withSize.size);
            assert(size == (uint32_t)(cursor - data));
        } else {
            size = (uint32_t)(cursor - data);
        }
        
        size += (uint32_t)(data - (char*)iter);
        iter = (SprObject::Raw *)((char*)iter + size);
    }
    
    // at this point iter points to the end of objects.
    (*objdata) = (char*)iter;
    
    if (!isRoot) {
        (*objdata) += loadAttrData(*objdata);
    } 
//    else {
//        fprintf(stdout, "Done parsing doc\n");
//    }
}

SprObjectGroup::SprObjectGroup(SprDocument const &doc, const char **objdata): SprObjectGroup(doc, objdata, false)
{
    
}

SprRootObjectGroup::SprRootObjectGroup(SprDocument const &doc, const char **objdata) : SprObjectGroup(doc, objdata, true)
{
}

SprObjectLine::SprObjectLine(SprDocument const &doc, const char **objdata): SprObject(doc, Type::Line)
{
    bswap_inline(&record.x1, *objdata + offsetof(Record, x1));
    bswap_inline(&record.x2, *objdata + offsetof(Record, x2));
    bswap_inline(&record.y1, *objdata + offsetof(Record, y1));
    bswap_inline(&record.y2, *objdata + offsetof(Record, y2));
    (*objdata) += sizeof(record) + loadAttrData((*objdata) + sizeof(record));
}

SprObjectPath::Command
SprObjectPath::Command::swapped(SprObjectPath::Command *cmd, uint32_t recsize)
{
    SprObjectPath::Command retval = {};
    retval.type = cmd->type;

    for (int i = 0; i < recsize/sizeof(float); i++)
        bswap_inline(&retval.u.values[i], (const char*) &cmd->u.values[i]);
    
    return retval;
}

SprObjectPath::SprObjectPath(SprDocument const &doc, const char **objdata): SprObject(doc, Type::Path)
{
    SprObjectPath::List *list = (SprObjectPath::List *) *objdata;
    SprObjectPath::Command *cmd = &list->commands[0];
    uint32_t attroffset;
    
    for (int i = 0; i < bswap32(list->count); i++) {
        uint32_t recordsize = 0;
        switch (cmd->type) {
            case Command::Type::NONE: // none and close commands are both empty commands.
            case Command::Type::CLOSE: break;
            case Command::Type::MOVETO: recordsize = sizeof(cmd->u.moveto); break;
            case Command::Type::LINETO: recordsize = sizeof(cmd->u.lineto); break;
            case Command::Type::QUADRATIC_CURVETO: recordsize = sizeof(cmd->u.quadratic_curveto); break;
            case Command::Type::BEZIER_CURVETO: recordsize = sizeof(cmd->u.bezier_curveto); break;
            case Command::Type::ELLIPTICAL_ARC: recordsize = sizeof(cmd->u.elliptical_arc); break;
                break;
            default:
                throw "unrecognized path type";
        };
        
        // byteswap all the float values
        commands.push_back(Command::swapped(cmd, recordsize));
        
        cmd = (SprObjectPath::Command *)((char*)cmd + sizeof(cmd->type) + recordsize);
    }
    
    attroffset = (uint32_t)((char*)cmd - *objdata);
    
    (*objdata) += attroffset + loadAttrData((*objdata) + attroffset);
}

SprObjectRectangle::SprObjectRectangle(SprDocument const &doc, const char **objdata): SprObject(doc, Type::Rectangle)
{
    bswap_inline(&record.left, *objdata + offsetof(Record, left));
    bswap_inline(&record.top, *objdata + offsetof(Record, top));
    bswap_inline(&record.right, *objdata + offsetof(Record, right));
    bswap_inline(&record.bottom, *objdata + offsetof(Record, bottom));
    bswap_inline(&record.rx, *objdata + offsetof(Record, rx));
    bswap_inline(&record.ry, *objdata + offsetof(Record, ry));
    (*objdata) += sizeof(record) + loadAttrData((*objdata) + sizeof(record));
}

SprObjectUse::SprObjectUse(SprDocument const &doc, const char **objdata): SprObject(doc, Type::Use)
{
    bswap_inline(&record.link, *objdata);
    (*objdata) += sizeof(record) + loadAttrData((*objdata) + sizeof(record));
}
