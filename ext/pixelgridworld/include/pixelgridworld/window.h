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
#include <pixelgridworld/core.h>
#include <functional>
#include <string>

struct GLFWwindow;

namespace window {
    class Window {
    public:
        typedef std::function<core::Matrix(float deltaTime)> RunFunc;
        Window(int argc, char **argv, int sizeX, int sizeY, const std::string& title, const std::vector<core::RGBA>& palette, RunFunc run);
        ~Window();
        int update();
        int run();
        void screenshot(const std::string filename);
        bool shouldClose();
    private:
        void renderMap(const core::Matrix& map);

        const RunFunc       runFunc;
        const int           m_width;
        const int           m_height;
        const std::vector<core::RGBA>   m_palette;
        GLFWwindow*         m_window;
        unsigned int        m_outputTexture;
        float               prevTime;

    };
}
