//
//  WOGL.cpp
//  WOGL
//
//  Created by Асиф Мамедов on 03/02/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "WOGL.hpp"

#include <SDL2/SDL.h>

#include <stdexcept>

using namespace std;

namespace WOGL
{
    static bool _initOpenGL = false;

    void init()
    {
        if (SDL_Init(SDL_INIT_EVERYTHING)) {
            throw runtime_error("Erro init system");
        }

        _initOpenGL = true;
    }

    void quit()
    {
        SDL_Quit();
        _initOpenGL = false;
    }

    bool initOpenGL()
    {
        return _initOpenGL;
    }
}
