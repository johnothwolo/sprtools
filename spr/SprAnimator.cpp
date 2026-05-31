//
//  SprAnimator.cpp
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
//  Created by John Othwolo on 5/26/26.
//

#include "spr.h"
#include "SprAnimator.h"

SprAnimator::SprAnimator(void) : type(Type::None)
{
    
}

SprAnimator::SprAnimator(Type type, const char *animdata, uint32_t &size) : type(type)
{
    SprData *data = (SprData *) animdata;
    
    uint32_t sz = bswap32(data->size);
}

SprAnimatorAlpha::SprAnimatorAlpha(const char *animdata, uint32_t &size)
{
    
}

SprAnimatorFillColor::SprAnimatorFillColor(const char *animdata, uint32_t &size)
{
    
}

SprAnimatorRotate::SprAnimatorRotate(const char *animdata, uint32_t &size)
{
    
}

SprAnimatorScale::SprAnimatorScale(const char *animdata, uint32_t &size)
{
    
}

SprAnimatorStrokeColor::SprAnimatorStrokeColor(const char *animdata, uint32_t &size)
{
    
}

SprAnimatorTranslate::SprAnimatorTranslate(const char *animdata, uint32_t &size)
{
    
}
