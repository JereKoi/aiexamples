//// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= ////
//// Pixelwridworld
////
//// MIT Licanse
////
//// Copyright (c) 2022 Mikko Romppainen.
////
//// Permission is hereby granted, free of charge, to any person obtaining
//// a copy of this software and associated documentation files (the
//// "Software"), to deal in the Software without restriction, including
//// without limitation the rights to use, copy, modify, merge, publish,
//// distribute, sublicense, and/or sell copies of the Software, and to
//// permit persons to whom the Software is furnished to do so, subject to
//// the following conditions:
////
//// The above copyright notice and this permission notice shall be included
//// in all copies or substantial portions of the Software.
////
//// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
//// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= ////
#pragma once
#include <vector>
#include <functional>

namespace core {
    typedef std::vector<int> Vector;
    typedef std::vector<Vector> Matrix;

    inline Vector vecN(size_t N, int value) {
        Vector res;
        for(size_t i=0; i<N; ++i) {
            res.push_back(value);
        }
        return res;
    }

    inline Matrix matN(size_t N, int value) {
        Matrix res;
        for(size_t i=0; i<N; ++i) {
            res.push_back(Vector());
            for(size_t j=0; j<N; ++j) {
                res[i].push_back(value);
            }
        }
        return res;
    }

    // Red - Green - Blue - Alpha -color.
    struct RGBA {
        RGBA(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a = 0xff)
            : r(_r), g(_g), b(_b), a(_a) {
        }

        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };

}
