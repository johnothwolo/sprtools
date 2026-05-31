//
//  SprInterpolator.h
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

#ifndef SprInterpolator_h
#define SprInterpolator_h

#include "compiler.h"

struct SprInterpolator {
    enum class Type : unsigned char {
        NONE = 0,
        ACCELERATE_DECELERATE = 1,
        ACCELERATE = 2,
        ANTICIPATE = 3,
        ANTICIPATE_OVERSHOOT = 4,
        BOUNCE = 5,
        CYCLE = 6,
        DECELERATE = 7,
        LINEAR = 8,
        OVERSHOOT = 9,
        BACKEASEIN = 10,
        BACKEASEOUT = 11,
        BACKEASEINOUT = 12,
        BOUNCEEASEIN = 13,
        BOUNCEEASEOUT = 14,
        BOUNCEEASEINOUT = 15,
        CIRCEASEIN = 16,
        CIRCEASEOUT = 17,
        CIRCEASEINOUT = 18,
        CUBICEASEIN = 19,
        CUBICEASEOUT = 20,
        CUBICEASEINOUT = 21,
        ELASTICEASEIN = 22,
        ELASTICEASEOUT = 23,
        ELASTICEASEINOUT = 24,
        EXPOEASEIN = 25,
        EXPOEASEOUT = 26,
        EXPOEASEINOUT = 27,
        QUADEASEIN = 28,
        QUADEASEOUT = 29,
        QUADEASEINOUT = 30,
        QUARTEASEIN = 31,
        QUARTEASEOUT = 32,
        QUARTEASEINOUT = 33,
        QUINTEASEIN = 34,
        QUINTEASEOUT = 35,
        QUINTEASEINOUT = 36,
        SINEEASEIN = 37,
        SINEEASEOUT = 38,
        SINEEASEINOUT = 39,
        QUINTOUT50 = 40,
        QUINTOUT80 = 41,
        SINEIN33 = 42,
        SINEINOUT33 = 43,
        SINEINOUT50 = 44,
        SINEINOUT60 = 45,
        SINEINOUT70 = 46,
        SINEINOUT80 = 47,
        SINEINOUT90 = 48,
        SINEOUT33 = 49,
    };
    
    SprInterpolator(void);
    SprInterpolator(Type type, const char *data);
    virtual float calculate(float input);
    
    const Type type;
    float cycle = 0.0f;
    float overshot = 0.0f;
    float amplitude = 0.0f;
    float period = 0.0f;
};


class BackEaseIn : public SprInterpolator {
    float overshot;
public:
    BackEaseIn(const char *data, float overshot);
    float calculate(float t) override;
};

class BackEaseInOut : public SprInterpolator {
    float overshot;
public:
    BackEaseInOut(const char *data, float overshot);
    float calculate(float t) override;
};

class BackEaseOut : public SprInterpolator {
    float overshot;
public:
    BackEaseOut(const char *data, float overshot);
    float calculate(float t) override;
};

class BounceEaseIn : public SprInterpolator {
    float out(float t);
public:
    BounceEaseIn(const char *data);
    float calculate(float t) override;
};

class BounceEaseInOut : public SprInterpolator {
    float out(float t);
public:
    BounceEaseInOut(const char *data);
    float calculate(float t) override;
};

class BounceEaseOut : public SprInterpolator {
public:
    BounceEaseOut(const char *data);
    float calculate(float t) override;
};

class CircEaseIn : public SprInterpolator {
public:
    CircEaseIn(const char *data);
    float calculate(float t) override;
};

class CircEaseInOut : public SprInterpolator {
public:
    CircEaseInOut(const char *data);
    float calculate(float t) override;
};

class CircEaseOut : public SprInterpolator {
public:
    CircEaseOut(const char *data);
    float calculate(float t) override;
};

class CubicEaseIn : public SprInterpolator {
public:
    CubicEaseIn(const char *data);
    float calculate(float t) override;
};

class CubicEaseInOut : public SprInterpolator {
public:
    CubicEaseInOut(const char *data);
    float calculate(float t) override;
};

class CubicEaseOut : public SprInterpolator {
public:
    CubicEaseOut(const char *data);
    float calculate(float t) override;
};

class ElasticEaseIn : public SprInterpolator {
public:
    float calculate(float t) override;

    ElasticEaseIn(float amplitude, float period) : amplitude (amplitude), period(period)
    {
    }

    ElasticEaseIn(const char *data) : ElasticEaseIn(0.0, 0.0) {}
private:
    float amplitude, period;
};

class ElasticEaseInOut : public SprInterpolator {
public:
    float calculate(float t) override;

    ElasticEaseInOut(float amplitude, float period) : amplitude (amplitude), period(period)
    {
    }

    ElasticEaseInOut(const char *data) : ElasticEaseInOut(0.0, 0.0) {}
private:
    float amplitude, period;
};

class ElasticEaseOut : public SprInterpolator {
public:
    float calculate(float t) override;

    ElasticEaseOut(float amplitude, float period) : amplitude (amplitude), period(period)
    {
    }

    ElasticEaseOut(const char *data) : ElasticEaseOut(0.0, 0.0) {}
private:
    float amplitude, period;
};

class ExpoEaseIn : public SprInterpolator {
public:
    ExpoEaseIn(const char *data);
    float calculate(float t) override;
};

class ExpoEaseInOut : public SprInterpolator {
public:
    ExpoEaseInOut(const char *data);
    float calculate(float t) override;
};

class ExpoEaseOut : public SprInterpolator {
public:
    ExpoEaseOut(const char *data);
    float calculate(float t) override;
};

class QuadEaseIn : public SprInterpolator {
public:
    QuadEaseIn(const char *data);
    float calculate(float t) override;
};

class QuadEaseInOut : public SprInterpolator {
public:
    QuadEaseInOut(const char *data);
    float calculate(float t) override;
};

class QuadEaseOut : public SprInterpolator {
public:
    QuadEaseOut(const char *data);
    float calculate(float t) override;
};

class QuartEaseIn : public SprInterpolator {
public:
    QuartEaseIn(const char *data);
    float calculate(float t) override;
};

class QuartEaseInOut : public SprInterpolator {
public:
    QuartEaseInOut(const char *data);
    float calculate(float t) override;
};

class QuartEaseOut : public SprInterpolator {
public:
    QuartEaseOut(const char *data);
    float calculate(float t) override;
};

class QuintEaseIn : public SprInterpolator {
public:
    QuintEaseIn(const char *data);
    float calculate(float t) override;
};

class QuintEaseInOut : public SprInterpolator {
public:
    QuintEaseInOut(const char *data);
    float calculate(float t) override;
};

class QuintEaseOut : public SprInterpolator {
public:
    QuintEaseOut(const char *data);
    float calculate(float t) override;
};

class QuintOut50 : public SprInterpolator {
public:
    QuintOut50(const char *data);
    float calculate(float t) override;

private:
    static constexpr float segments[][3] = { {0.0, 0.502, 0.742}, {0.742, 1.082, 1.0} };

};

class QuintOut80 : public SprInterpolator {
public:
    QuintOut80(const char *data);
    float calculate(float t) override;

private:
    static constexpr float segments[][3] = { {0.0, 0.718, 0.845}, {0.845, 0.998, 1.0} };
};

class SineEaseIn : public SprInterpolator {
public:
    SineEaseIn(const char *data);
    float calculate(float t) override;
};

class SineEaseInOut : public SprInterpolator {
public:
    SineEaseInOut(const char *data);
    float calculate(float t) override;
};

class SineEaseOut : public SprInterpolator {
public:
    SineEaseOut(const char *data);
    float calculate(float t) override;
};

class SineIn33 : public SprInterpolator {
public:
    SineIn33(const char *data);
    float calculate(float t) override;

private:
    static constexpr float segments[][3] = { {0.0, 0.001, 0.32}, {0.32, 0.59, 1.0} };
};

class SineInOut33 : public SprInterpolator {
public:
    SineInOut33(const char *data);
    float calculate(float t) override;

private:
    static constexpr float segments[][3] = { {0.0, 0.05, 0.495}, {0.495, 0.94, 1.0} };
};

class SineInOut50 : public SprInterpolator {
public:
    SineInOut50(const char *data);
    float calculate(float t) override;

private:
    static constexpr float segments[][3] = {{0.0, 0.05, 0.61}, {0.61, 0.999, 1.0}};
};

class SineInOut60 : public SprInterpolator {
public:
    SineInOut60(const char *data);
    float calculate(float t) override;

private:
    static constexpr float segments[][3] = {{0.0, 0.01, 0.37}, {0.37, 0.72, 0.888}, {0.888, 0.9999, 1.0}};
};

class SineInOut70 : public SprInterpolator {
public:
    SineInOut70(const char *data);
    float calculate(float t) override;

private:
    static constexpr float segments[][3] = {{0.0, 0.01, 0.45}, {0.45, 0.8, 0.908}, {0.908, 0.9999, 1.0}};
};

class SineInOut80 : public SprInterpolator {
public:
    SineInOut80(const char *data);
    float calculate(float t) override;

private:
    static constexpr float segments[][3] = {{0.0, 0.0, 0.195}, {0.195, 0.48, 0.645}, {0.645, 0.835, 0.885}, {0.885, 0.955, 0.978}, {0.978, 0.9999, 1.0}};
};

class SineInOut90 : public SprInterpolator {
public:
    SineInOut90(const char *data);
    float calculate(float t) override;

private:
    static constexpr float segments[][3] = {{0.0, 0.0, 0.247}, {0.247, 0.48, 0.7}, {0.7, 0.835, 0.905}, {0.905, 0.955, 0.978}, {0.978, 0.9999, 1.0}};
};

class SineOut33 : public SprInterpolator {
public:
    SineOut33(const char *data);
    float calculate(float t) override;

private:
    static constexpr float segments[][3] = {{0.0, 0.386, 0.645}, {0.645, 0.962, 1.0}};
};

class SprTimeInterpolator : public SprInterpolator {

    static const int DAY_MILLISECONDS = 86400000;
    static const int WEEK_MILLISECONDS = 604800000;
    int mDuration, mPeriodType, mQuotient;

public:
    static const int DAY_TYPE = 1, WEEK_TYPE = 2;

    SprTimeInterpolator(int duration, int type, int quotient)
    : mDuration(duration), mPeriodType(type), mQuotient(quotient) {}

    SprTimeInterpolator() : SprTimeInterpolator(0,0,0) {}

    void setDuration(int duration) { mDuration = duration; }
    void setPeriodType(int type) { mPeriodType = type; }
    void setQuotient(int quotient) { mQuotient = quotient; }

    float calculate(float input) override;
};


#endif /* SprInterpolator_h */
