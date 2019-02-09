//
//  Texture.hpp
//  WOGL
//
//  Created by Асиф Мамедов on 07/02/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#ifndef Texture_hpp
#define Texture_hpp

namespace WOGL
{
    inline namespace Data
    {
        enum class TexelType
        {
            RED,
            RG,
            RGB,
            RGBA
        };

        enum class Canal
        {
            RED,
            GREEN,
            BLUE,
            ALPHA
        };
    }
}

#include "Texture.inl"

#endif /* Texture_hpp */
