//
//  SprInterpolator.cpp
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

#include "SprInterpolator.h"

SprInterpolator::SprInterpolator(void) : type(Type::NONE)
{
}

SprInterpolator::SprInterpolator(Type type, const char *data) : type(type)
{
}

float
SprInterpolator::calculate(float input)
{
    return input;
}

/*
 
 
 class BackEaseIn : public Base {
     float overshot;
 public:
     BackEaseIn(float overshot) : overshot(overshot) {}
     float calculate(float t) override
     {
         auto o = overshot;
         if (o == 0.0) o = 1.70158;
         return t * t * (((1.0 + o) * t) - o);
     }
 };

 class BackEaseInOut : public Base {
     float overshot;
 public:
     BackEaseInOut(float overshot) : overshot(overshot) {}
     float calculate(float t) override
     {
         auto o = overshot;
         if (o == 0.0) o = 1.70158;
         float t2 = t * 2.0;
         if (t2 < 1.0) {
             float o2 = (float) (o * 1.525);
             return t2 * t2 * (((o2 + 1.0) * t2) - o2) * 0.5;
         }
         float t3 = t2 - 2.0;
         float o3 = (float) (o * 1.525);
         return ((t3 * t3 * (((o3 + 1.0) * t3) + o3)) + 2.0) * 0.5;
     }
 };

 class BackEaseOut : public Base {
     float overshot;
 public:
     BackEaseOut(float overshot) : overshot(overshot) {}
     float calculate(float t) override
     {
         auto o = overshot;
         if (o == 0.0) o = 1.70158;
         float t2 = t - 1.0;
         return (t2 * t2 * (((o + 1.0) * t2) + o)) + 1.0;
     }
 };

 class BounceEaseIn : public Base {
     float out(float t) {
         if (t < 0.36363636363636365) {
             return 7.5625 * t * t;
         }
         if (t < 0.7272727272727273) {
             float t2 = (float) (t - 0.5454545454545454);
             return (t2 * 7.5625 * t2) + 0.75;
         } else if (t < 0.9090909090909091) {
             float t3 = (float) (t - 0.8181818181818182);
             return (t3 * 7.5625 * t3) + 0.9375;
         } else {
             float t4 = (float) (t - 0.9545454545454546);
             return (t4 * 7.5625 * t4) + 0.984375;
         }
     }
 public:
     float calculate(float t) override
     {
         return 1.0 - out(1.0 - t);
     }
 };

 class BounceEaseInOut : public Base {
     float out(float t) {
         if (t < 0.36363636363636365) {
             return 7.5625 * t * t;
         }
         if (t < 0.7272727272727273) {
             float t2 = (float) (t - 0.5454545454545454);
             return (t2 * 7.5625 * t2) + 0.75;
         } else if (t < 0.9090909090909091) {
             float t3 = (float) (t - 0.8181818181818182);
             return (t3 * 7.5625 * t3) + 0.9375;
         } else {
             float t4 = (float) (t - 0.9545454545454546);
             return (t4 * 7.5625 * t4) + 0.984375;
         }
     }
 public:
     float calculate(float t) override
     {
         return t < 0.5 ?  (1.0 - out(1.0 - (2.0 * t))) * 0.5 : (out((2.0 * t) - 1.0) * 0.5) + 0.5;
     }
 };

 class BounceEaseOut : public Base {
 public:
     float calculate(float t) override
     {
         if (t < 0.36363636363636365) {
             return 7.5625 * t * t;
         }
         if (t < 0.7272727272727273) {
             float t2 = (float) (t - 0.5454545454545454);
             return (t2 * 7.5625 * t2) + 0.75;
         } else if (t < 0.9090909090909091) {
             float t3 = (float) (t - 0.8181818181818182);
             return (t3 * 7.5625 * t3) + 0.9375;
         } else {
             float t4 = (float) (t - 0.9545454545454546);
             return (t4 * 7.5625 * t4) + 0.984375;
         }
     }
 };

 class CircEaseIn : public Base {
 public:
     float calculate(float t) override
     {
         return (float) (-(sqrt(1.0 - (t * t)) - 1.0));
     }
 };

 class CircEaseInOut : public Base {
 public:
     float calculate(float t) override
     {
         float t2 = t * 2.0;
         if (t2 >= 1.0) {
             float t3 = t2 - 2.0;
             return (float) ((sqrt(1.0 - (t3 * t3)) + 1.0) * 0.5);
         }
         return (float) ((sqrt(1.0 - (t2 * t2)) - 1.0) * (-0.5));
     }
 };

 class CircEaseOut : public Base {
 public:
     float calculate(float t) override
     {
         float t2 = t - 1.0;
         return (float) sqrt(1.0 - (t2 * t2));
     }
 };

 class CubicEaseIn : public Base {
 public:
     float calculate(float t) override
     {
         return t * t * t;
     }
 };

 class CubicEaseInOut : public Base {
 public:
     float calculate(float t) override
     {
         float t2 = t * 2.0;
         if (t2 < 1.0) {
             return 0.5 * t2 * t2 * t2;
         }
         float t3 = t2 - 2.0;
         return ((t3 * t3 * t3) + 2.0) * 0.5;
     }
 };

 class CubicEaseOut : public Base {
 public:
     float calculate(float t) override
     {
         float t2 = t - 1.0;
         return (t2 * t2 * t2) + 1.0;
     }
 };

 class ElasticEaseIn : public Base {
 public:
     float calculate(float t) override
     {
         float a = amplitude;
         float p = period;
         float s;
         
         if (t == 0.0) {
             return 0.0;
         }
         if (t >= 1.0) {
             return 1.0;
         }
         if (p == 0.0) {
             p = 0.3;
         }
         if (a != 0.0 && a >= 1.0) {
             s = (float) ((p / 6.283185307179586) * asin(1.0 / a));
         } else {
             a = 1.0;
             s = p / 4.0;
         }
         float t2 = t - 1.0;
         return (float) (-(a * pow(2.0, t2 * 10.0) * sin(((t2 - s) * 6.283185307179586) / p)));
     }

     ElasticEaseIn(float amplitude, float period) : amplitude (amplitude), period(period)
     {
     }

     ElasticEaseIn() ElasticEaseIn(0.0, 0.0)
 private:
     float amplitude, period;
 };

 class ElasticEaseInOut : public Base {
 public:
     float calculate(float t) override
     {
         float a = amplitude;
         float p = period;
         float a2, p2, s;

         if (t == 0.0) {
             return 0.0;
         }
         if (t >= 1.0) {
             return 1.0;
         }
         if (p != 0.0) {
             p2 = p;
         } else {
             p2 = 0.45000002;
         }
         if (a != 0.0 && a >= 1.0) {
             s = (float) ((p2 / 6.283185307179586) * asin(1.0 / a));
             a2 = a;
         } else {
             a2 = 1.0;
             s = p2 / 4.0;
         }
         float t2 = 2.0 * t;
         if (t2 < 1.0) {
             float t3 = t2 - 1.0;
             return (float) (a2 * pow(2.0, t3 * 10.0) * sin(((t3 - s) * 6.283185307179586) / p2) * (-0.5));
         }
         float t4 = t2 - 1.0;
         return (float) ((a2 * pow(2.0, t4 * (-10.0)) * sin(((t4 - s) * 6.283185307179586) / p2) * 0.5) + 1.0);

     }

     ElasticEaseInOut(float amplitude, float period) : amplitude (amplitude), period(period)
     {
     }

     ElasticEaseInOut() ElasticEaseInOut(0.0, 0.0)
 private:
     float amplitude, period;
 };

 class ElasticEaseOut : public Base {
 public:
     float calculate(float t) override
     {
         float a = amplitude;
         float p = period;
         float s;

         if (t == 0.0) {
             return 0.0;
         }
         if (t >= 1.0) {
             return 1.0;
         }
         if (p == 0.0) {
             p = 0.3;
         }
         if (a != 0.0 && a >= 1.0) {
             s = (float) ((p / 6.283185307179586) * asin(1.0 / a));
         } else {
             a = 1.0;
             s = p / 4.0;
         }
         return (float) ((a * pow(2.0, (-10.0) * t) * sin(((t - s) * 6.283185307179586) / p)) + 1.0);

     }

     ElasticEaseOut(float amplitude, float period) : amplitude (amplitude), period(period)
     {
     }

     ElasticEaseOut() ElasticEaseOut(0.0, 0.0)
 private:
     float amplitude, period;
 };

 class ExpoEaseIn : public Base {
 public:
     float calculate(float t) override
     {
         return (float) (t == 0.0 ? ENVIRONMENT_VALUE_UNKNOWN : pow(2.0, (t - 1.0) * 10.0));
     }
 };

 class ExpoEaseInOut : public Base {
 public:
     float calculate(float t) override
     {
         if (t == 0.0) {
             return 0.0;
         }
         if (t >= 1.0) {
             return 1.0;
         }
         float t2 = t * 2.0;
         if (t2 < 1.0) {
             return (float) (pow(2.0, (t2 - 1.0) * 10.0) * 0.5);
         }
         return (float) (((-pow(2.0, (t2 - 1.0) * (-10.0))) + 2.0) * 0.5);
     }
 };

 class ExpoEaseOut : public Base {
 public:
     float calculate(float t) override
     {
         return (float) (t < 1.0 ? 1.0 + (-pow(2.0, (-10.0) * t)) : 1.0);
     }
 };

 class QuadEaseIn : public Base {
 public:
     float calculate(float t) override
     {
         return t * t;
     }
 };

 class QuadEaseInOut : public Base {
 public:
     float calculate(float t) override
     {
         float t2 = t * 2.0;
         if (t2 >= 1.0) {
             float t3 = t2 - 1.0;
             return ((t3 * (t3 - 2.0)) - 1.0) * (-0.5);
         }
         return 0.5 * t2 * t2;
     }
 };

 class QuadEaseOut : public Base {
 public:
     float calculate(float t) override
     {
         return (-t) * (t - 2.0);
     }
 };

 class QuartEaseIn : public Base {
 public:
     float calculate(float t) override
     {
         return t * t * t * t;
     }
 };

 class QuartEaseInOut : public Base {
 public:
     float calculate(float t) override
     {
         float t2 = t * 2.0;
         if (t2 >= 1.0) {
             float t3 = t2 - 2.0;
             return ((((t3 * t3) * t3) * t3) - 2.0) * (-0.5);
         }
         return 0.5 * t2 * t2 * t2 * t2;
     }
 };

 class QuartEaseOut : public Base {
 public:
     float calculate(float t) override
     {
         float t2 = t - 1.0;
         return -((((t2 * t2) * t2) * t2) - 1.0);
     }
 };

 class QuintEaseIn : public Base {
 public:
     float calculate(float t) override
     {
         return t * t * t * t * t;
     }
 };

 class QuintEaseInOut : public Base {
 public:
     float calculate(float t) override
     {
         float t2 = t * 2.0;
         if (t2 < 1.0) {
             return 0.5 * t2 * t2 * t2 * t2 * t2;
         }
         float t3 = t2 - 2.0;
         return ((t3 * t3 * t3 * t3 * t3) + 2.0) * 0.5;
     }
 };

 class QuintEaseOut : public Base {
 public:
     float calculate(float t) override
     {
         float t2 = t - 1.0;
         return (t2 * t2 * t2 * t2 * t2) + 1.0;
     }
 };

 class QuintOut50 : public Base {
 public:
     float calculate(float t) override
     {
         float _loc_5 = input / 1.0;
         int _loc_6 = SIZEOF_ARRAY(segments);
         int _loc_9 = (int) floor(_loc_6 * _loc_5);
         float fArr[][] = segments;
         if (_loc_9 >= fArr.length) {
             _loc_9 = fArr.length - 1;
         }
         float _loc_7 = (_loc_5 - (_loc_9 * (1.0 / _loc_6))) * _loc_6;
         float _loc_8[] = segments[_loc_9];
         float ret = ((_loc_8[0] + ((((1.0 - _loc_7) * 2.0 * (_loc_8[1] - _loc_8[0])) + ((_loc_8[2] - _loc_8[0]) * _loc_7)) * _loc_7)) * 1.0) + 0.0;
         return ret;
     }

 private:
     static float segments[][] = { {0.0, 0.502, 0.742}, {0.742, 1.082, 1.0} };

 };

 class QuintOut80 : public Base {
 public:
     float calculate(float t) override
     {
         float _loc_5 = input / 1.0;
         int _loc_6 = SIZEOF_ARRAY(segments);
         int _loc_9 = (int) floor(_loc_6 * _loc_5);
         float fArr[][] = segments;
         if (_loc_9 >= fArr.length) {
             _loc_9 = fArr.length - 1;
         }
         float _loc_7 = (_loc_5 - (_loc_9 * (1.0 / _loc_6))) * _loc_6;
         float _loc_8[] = segments[_loc_9];
         float ret = ((_loc_8[0] + ((((1.0 - _loc_7) * 2.0 * (_loc_8[1] - _loc_8[0])) + ((_loc_8[2] - _loc_8[0]) * _loc_7)) * _loc_7)) * 1.0) + 0.0;
         return ret;
     }

 private:
     static float segments[][] = { {0.0, 0.718, 0.845}, {0.845, 0.998, 1.0} };
 };

 class SineEaseIn : public Base {
 public:
     float calculate(float t) override
     {
         return (float) ((-cos(t * 1.5707963267948966)) + 1.0);
     }
 };

 class SineEaseInOut : public Base {
 public:
     float calculate(float t) override
     {
         return (float) ((cos(t * 3.141592653589793) - 1.0) * (-0.5));
     }
 };

 class SineEaseOut : public Base {
 public:
     float calculate(float t) override
     {
         return (float) sin(t * 1.5707963267948966);
     }
 };

 class SineIn33 : public Base {
 public:
     float calculate(float t) override
     {
         float _loc_5 = input / 1.0;
         int _loc_6 = SIZEOF_ARRAY(segments);
         int _loc_9 = (int) floor(_loc_6 * _loc_5);
         float fArr[][] = segments;
         if (_loc_9 >= fArr.length) {
             _loc_9 = fArr.length - 1;
         }
         float _loc_7 = (_loc_5 - (_loc_9 * (1.0 / _loc_6))) * _loc_6;
         float _loc_8[] = segments[_loc_9];
         float ret = ((_loc_8[0] + ((((1.0 - _loc_7) * 2.0 * (_loc_8[1] - _loc_8[0])) + ((_loc_8[2] - _loc_8[0]) * _loc_7)) * _loc_7)) * 1.0) + 0.0;
         return ret;
     }

 private:
     static float segments[][] = { {0.0, 0.001, 0.32}, {0.32, 0.59, 1.0} };
 };

 class SineInOut33 : public Base {
 public:
     float calculate(float t) override
     {
         float _loc_5 = input / 1.0;
         int _loc_6 = SIZEOF_ARRAY(segments);
         int _loc_9 = (int) floor(_loc_6 * _loc_5);
         float fArr[][] = segments;
         if (_loc_9 >= fArr.length) {
             _loc_9 = fArr.length - 1;
         }
         float _loc_7 = (_loc_5 - (_loc_9 * (1.0 / _loc_6))) * _loc_6;
         float _loc_8[] = segments[_loc_9];
         float ret = ((_loc_8[0] + ((((1.0 - _loc_7) * 2.0 * (_loc_8[1] - _loc_8[0])) + ((_loc_8[2] - _loc_8[0]) * _loc_7)) * _loc_7)) * 1.0) + 0.0;
         return ret;
     }

 private:
     static float segments[][] = { {0.0, 0.05, 0.495}, {0.495, 0.94, 1.0} };
 };

 class SineInOut50 : public Base {
 public:
     float calculate(float t) override
     {
         float _loc_5 = input / 1.0;
         int _loc_6 = SIZEOF_ARRAY(segments);
         int _loc_9 = (int) floor(_loc_6 * _loc_5);
         float fArr[][] = segments;
         if (_loc_9 >= fArr.length) {
             _loc_9 = fArr.length - 1;
         }
         float _loc_7 = (_loc_5 - (_loc_9 * (1.0 / _loc_6))) * _loc_6;
         float _loc_8[] = segments[_loc_9];
         float ret = ((_loc_8[0] + ((((1.0 - _loc_7) * 2.0 * (_loc_8[1] - _loc_8[0])) + ((_loc_8[2] - _loc_8[0]) * _loc_7)) * _loc_7)) * 1.0) + 0.0;
         return ret;
     }

 private:
     static float segments[][] = {{0.0, 0.05, 0.61}, {0.61, 0.999, 1.0}};
 };

 class SineInOut60 : public Base {
 public:
     float calculate(float t) override
     {
         float _loc_5 = input / 1.0;
         int _loc_6 = SIZEOF_ARRAY(segments);
         int _loc_9 = (int) floor(_loc_6 * _loc_5);
         float fArr[][] = segments;
         if (_loc_9 >= fArr.length) {
             _loc_9 = fArr.length - 1;
         }
         float _loc_7 = (_loc_5 - (_loc_9 * (1.0 / _loc_6))) * _loc_6;
         float _loc_8[] = segments[_loc_9];
         float ret = ((_loc_8[0] + ((((1.0 - _loc_7) * 2.0 * (_loc_8[1] - _loc_8[0])) + ((_loc_8[2] - _loc_8[0]) * _loc_7)) * _loc_7)) * 1.0) + 0.0;
         return ret;
     }

 private:
     static float segments[][] = {{0.0, 0.01, 0.37}, {0.37, 0.72, 0.888}, {0.888, 0.9999, 1.0}};
 };

 class SineInOut70 : public Base {
 public:
     float calculate(float t) override
     {
         float _loc_5 = input / 1.0;
         int _loc_6 = SIZEOF_ARRAY(segments);
         int _loc_9 = (int) floor(_loc_6 * _loc_5);
         float fArr[][] = segments;
         if (_loc_9 >= fArr.length) {
             _loc_9 = fArr.length - 1;
         }
         float _loc_7 = (_loc_5 - (_loc_9 * (1.0 / _loc_6))) * _loc_6;
         float _loc_8[] = segments[_loc_9];
         float ret = ((_loc_8[0] + ((((1.0 - _loc_7) * 2.0 * (_loc_8[1] - _loc_8[0])) + ((_loc_8[2] - _loc_8[0]) * _loc_7)) * _loc_7)) * 1.0) + 0.0;
         return ret;
     }

 private static float segments[][] = {{0.0, 0.01, 0.45}, {0.45, 0.8, 0.908}, {0.908, 0.9999, 1.0}};
 };

 class SineInOut80 : public Base {
 public:
     float calculate(float t) override
     {
         float _loc_5 = input / 1.0;
         int _loc_6 = SIZEOF_ARRAY(segments);
         int _loc_9 = (int) floor(_loc_6 * _loc_5);
         float fArr[][] = segments;
         if (_loc_9 >= fArr.length) {
             _loc_9 = fArr.length - 1;
         }
         float _loc_7 = (_loc_5 - (_loc_9 * (1.0 / _loc_6))) * _loc_6;
         float _loc_8[] = segments[_loc_9];
         float ret = ((_loc_8[0] + ((((1.0 - _loc_7) * 2.0 * (_loc_8[1] - _loc_8[0])) + ((_loc_8[2] - _loc_8[0]) * _loc_7)) * _loc_7)) * 1.0) + 0.0;
         return ret;
     }

 private:
     static float segments[][] = {{0.0, 0.0, 0.195}, {0.195, 0.48, 0.645}, {0.645, 0.835, 0.885}, {0.885, 0.955, 0.978}, {0.978, 0.9999, 1.0}};
 };

 class SineInOut90 : public Base {
 public:
     float calculate(float t) override
     {
         float _loc_5 = input / 1.0;
         int _loc_6 = SIZEOF_ARRAY(segments);
         int _loc_9 = (int) floor(_loc_6 * _loc_5);
         float fArr[][] = segments;
         if (_loc_9 >= fArr.length) {
             _loc_9 = fArr.length - 1;
         }
         float _loc_7 = (_loc_5 - (_loc_9 * (1.0 / _loc_6))) * _loc_6;
         float _loc_8[] = segments[_loc_9];
         float ret = ((_loc_8[0] + ((((1.0 - _loc_7) * 2.0 * (_loc_8[1] - _loc_8[0])) + ((_loc_8[2] - _loc_8[0]) * _loc_7)) * _loc_7)) * 1.0) + 0.0;
         return ret;
     }

 private:
     static float segments[][] = {{0.0, 0.0, 0.247}, {0.247, 0.48, 0.7}, {0.7, 0.835, 0.905}, {0.905, 0.955, 0.978}, {0.978, 0.9999, 1.0}};
 };

 class SineOut33 : public Base {
 public:
     float calculate(float t) override
     {
         float _loc_5 = input / 1.0;
         int _loc_6 = SIZEOF_ARRAY(segments);
         int _loc_9 = (int) floor(_loc_6 * _loc_5);
         float fArr[][] = segments;
         if (_loc_9 >= fArr.length) {
             _loc_9 = fArr.length - 1;
         }
         float _loc_7 = (_loc_5 - (_loc_9 * (1.0 / _loc_6))) * _loc_6;
         float _loc_8[] = segments[_loc_9];
         float ret = ((_loc_8[0] + ((((1.0 - _loc_7) * 2.0 * (_loc_8[1] - _loc_8[0])) + ((_loc_8[2] - _loc_8[0]) * _loc_7)) * _loc_7)) * 1.0) + 0.0;
         return ret;
     }

 private:
     static float segments[][] = {{0.0, 0.386, 0.645}, {0.645, 0.962, 1.0}};
 };

 class SprTimeInterpolator : public Base {

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

     float calculate(float input) override
     {
         long now;
         long currentTime = System.currentTimeMillis();
         long now2 = TimeZone.getDefault().getOffset(currentTime) + currentTime;
         if (mPeriodType == 1) {
             now = now2 % 86400000;
         } else {
             now = (now2 - DevicePolicyManager.DEFAULT_STRONG_AUTH_TIMEOUT_MS) % 604800000;
         }
         long tick = now % mDuration;
         int i = mQuotient;
         if (i > 1) {
             tick = (tick / i) * i;
         }
         return ((float) tick) / mDuration;
     }
 };
 
 */
