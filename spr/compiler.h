//
//  compiler.h
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

#ifndef compiler_h
#define compiler_h

#include <stdint.h>
#include <assert.h>
#include <typeinfo>

#ifndef __packed
#define __packed __attribute__((packed))
#endif

template<typename NumberType> [[used]]
NumberType bswap_inline(NumberType *outVal, const char *inVal)
{
    unsigned char *in = (unsigned char*) inVal;
    unsigned char *out = (unsigned char*) outVal;
    
    for (int i = 0; i < sizeof(NumberType); i++)
        out[i] = in[sizeof(NumberType) - 1 - i];
    
    return *outVal;
}

template<typename NumberType>
NumberType bswap(NumberType inVal)
{
    NumberType outVal = 0;
    return bswap_inline(&outVal, (const char*)&inVal);
}

#define bswap16 bswap<uint16_t>
#define bswap32 bswap<uint32_t>
#define bswap64 bswap<uint64_t>

#endif /* compiler_h */
