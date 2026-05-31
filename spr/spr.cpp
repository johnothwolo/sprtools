//
//  spr.cpp
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

#include "spr.h"

SprHeader::SprHeader(const char *filedata)
:
h34 {
    .base = {
        .magic = bswap32(((SprHeaderBase*)filedata)->magic),
        .majorvers = bswap16(((SprHeaderBase*)filedata)->majorvers),
        .minorvers = bswap16(((SprHeaderBase*)filedata)->minorvers),
        .referenceSectionOffset = bswap32(((SprHeaderBase*)filedata)->referenceSectionOffset),
        .documentOffset = bswap32(((SprHeaderBase*)filedata)->documentOffset),
        .fileAttributeOffset = bswap32(((SprHeaderBase*)filedata)->fileAttributeOffset),
        .unk1 = {
            bswap32(((SprHeaderBase*)filedata)->unk1[0]),
            bswap32(((SprHeaderBase*)filedata)->unk1[1])
        },
        .dimensions = {
            .x = bswap<float>(((SprHeaderBase*)filedata)->dimensions.x),
            .y = bswap<float>(((SprHeaderBase*)filedata)->dimensions.y),
            .width = bswap<float>(((SprHeaderBase*)filedata)->dimensions.width),
            .height = bswap<float>(((SprHeaderBase*)filedata)->dimensions.height),
        },
        .ninepatch = {
            .bottom = bswap<float>(((SprHeaderBase*)filedata)->ninepatch.bottom),
            .right = bswap<float>(((SprHeaderBase*)filedata)->ninepatch.right),
            .left = bswap<float>(((SprHeaderBase*)filedata)->ninepatch.left),
            .top = bswap<float>(((SprHeaderBase*)filedata)->ninepatch.top),
        },
        .padding = {
            .bottom = bswap<float>(((SprHeaderBase*)filedata)->padding.bottom),
            .right = bswap<float>(((SprHeaderBase*)filedata)->padding.right),
            .left = bswap<float>(((SprHeaderBase*)filedata)->padding.left),
            .top = bswap<float>(((SprHeaderBase*)filedata)->padding.top),
        },
        .density = bswap32(((SprHeaderBase*)filedata)->density),
    },
    
    .documentCount = (h34.base.majorvers >= 12336 && base.minorvers >= 12339)  ? bswap32(((SprHeader33*)filedata)->documentCount) : 1,
    .repeatCount = (h34.base.majorvers >= 12336 && base.minorvers >= 12339) ? bswap32(((SprHeader33*)filedata)->repeatCount) : 0,
    .repeatMode = (h34.base.majorvers >= 12336 && base.minorvers >= 12339) ? (((SprHeader33*)filedata)->repeatMode) : (uint8_t) 2,
    
    .mAnimationMode = (h34.base.majorvers >= 12336 && base.minorvers >= 12340) ? bswap32(((SprHeader34*)filedata)->mAnimationMode) : 0,
    .mAnimationInterval = (h34.base.majorvers >= 12336 && base.minorvers >= 12340) ? bswap32(((SprHeader34*)filedata)->mAnimationInterval) : 0,
}
{
    
}

SprDocument::SprDocument(const char* data) : filedata(data), header(data)
{
    if (header.base.magic != SPR_MAGIC && header.base.magic != SVF_MAGIC){
        fprintf(stderr, "Invalid SPR file");
        exit(EINVAL);
    }
    
    // setup file attributes
    if (header.base.fileAttributeOffset != 0) {
        
        SprFileAttribute::List *list = (SprFileAttribute::List *)(data + header.base.fileAttributeOffset);
        SprFileAttribute::Raw *attr = &list->attrs[0];
        
        for (int i = 0; i < bswap32(list->count); i++) {
            switch (attr->type) {
                case (int) SprFileAttribute::Type::NinePatch:
                    fileattrs.push_back(std::make_unique<SprFileAttributeNinePatch>(attr->data));
                    break;
                default: break;
            }
            attr = (SprFileAttribute::Raw *)((char*)attr + attr->size);
        }
    }
    
    // setup reference objects
    SprRefrenceObjectList *refobjlist = (SprRefrenceObjectList *)(data + header.base.referenceSectionOffset);
    SprRefrenceObjectElement *iter = &refobjlist->objs[0];
    for (int i = 0, size = 0; i < refobjlist->count; i++) {
        bool hasSize = HAS_SIZE_FIELD(header.base.majorvers, header.base.minorvers);
        const char *data = hasSize ? &iter->withSize.data[0] : &iter->noSize.data[0], *cursor = data;
        switch (iter->type) {
            case (int) SprObject::Type::Circle:
                refobjects.push_back(std::make_shared<SprObjectCircle>(*this, &cursor));
                break;
            case (int) SprObject::Type::Ellipse:
                refobjects.push_back(std::make_shared<SprObjectEllipse>(*this, &cursor));
                break;
            case (int) SprObject::Type::Line:
                refobjects.push_back(std::make_shared<SprObjectLine>(*this, &cursor));
                break;
            case (int) SprObject::Type::Path:
                refobjects.push_back(std::make_shared<SprObjectPath>(*this, &cursor));
                break;
            case (int) SprObject::Type::Rectangle:
                refobjects.push_back(std::make_shared<SprObjectRectangle>(*this, &cursor));
                break;
            case (int) SprObject::Type::Group:
                refobjects.push_back(std::make_shared<SprObjectGroup>(*this, &cursor));
                break;
            case (int) SprObject::Type::Use:
                refobjects.push_back(std::make_shared<SprObjectUse>(*this, &cursor));
                break;
            default: break;
        }
        
        // make sure the size is set according to version
        if (hasSize){
            size = bswap32(iter->withSize.size);
            assert(size == (uint32_t)(cursor - data));
        } else {
            size = (uint32_t)(cursor - data);
        }
        
        size += (uint32_t)(data - (char*)iter);
        iter = (SprRefrenceObjectElement *)((char*)iter + size);
    }
    
    // setup documents
    uint32_t documentCount = HAS_SIZE_FIELD(header.base.majorvers, header.base.minorvers) ? header.h33.documentCount : 1;
    const char *dociter = data + header.base.documentOffset;
    for (int i = 0; i < documentCount; i++) {
        documents.push_back(std::make_unique<SprRootObjectGroup>(*this, &dociter)); // cursor ptr automatically moved by callees
    }
}

SprDocument::~SprDocument()
{
    delete [] filedata;
}

int
SprDocument::GetVersionMajor() const
{
    return header.base.majorvers;
}

int
SprDocument::GetVersionMinor() const
{
    return header.base.minorvers;
}

void 
SprDocument::add_animation(std::shared_ptr<SprObject> const obj) const
{
    ((SprDocument*)this)->animations.push_back(obj);
}
