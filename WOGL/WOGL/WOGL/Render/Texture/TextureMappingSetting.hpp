//
//  TextureMappingSetting.hpp
//  WOGL
//
//  Created by Асиф Мамедов on 11/03/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#ifndef TextureMappingSetting_hpp
#define TextureMappingSetting_hpp

#include <GL/glew.h>

namespace WOGL
{
    /**
     * Определяет тип отображения текстуры на область превышающую или меньшую, чем один элемент текстуры.
     *
     * @field LINEAR возвращает средневзвешенное значение четырех
     * элементов текстуры, которые находятся ближе всего к центру
     * текстурируемого пикселя.
     * @field NEAREST возвращает значение элемента текстуры,
     * ближайшего к центру текстурируемого пикселя.
     * @field NEAREST_MIPMAP_NEAREST Выбирает мип-карту, которая наиболее точно
     * соответствует размеру текстурируемого пикселя, и использует критерий GL_NEAREST
     * для получения значения текстуры.
     * @field LINEAR_MIPMAP_NEAREST выбирает мип-карту, которая наиболее точно
     * соответствует размеру текстурируемого пикселя, и использует критерий GL_LINEAR
     * для получения значения текстуры.
     * @field NEAREST_MIPMAP_LINEAR выбирает два мипмапа, которые наиболее точно
     * соответствуют размеру текстурируемого пикселя, и использует критерий GL_NEAREST
     * для получения значения текстуры из каждого мипмапа. Конечное значение текстуры
     * представляет собой средневзвешенное значение этих двух значений.
     * @field LINEAR_MIPMAP_LINEAR Выбирает два мипмапа, которые наиболее точно соответствуют
     * размеру текстурируемого пикселя, и использует критерий GL_LINEAR
     * для получения значения текстуры из каждого мипмапа. Конечное значение текстуры
     * представляет собой средневзвешенное значение этих двух значений.
    */
    enum class TextureFilter
    {
        LINEAR = GL_LINEAR,
        NEAREST = GL_NEAREST,
        NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
        LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
        NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
        LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
    };

    /**
     *  Опеределеняет способо отображения текстуры в случае выхода из деопозпона (0, 0) и (1, 1).
     *
     * @field REPEAT повторять текстуру
     * @field MIRRORED_REPEAT работает так же как REPEAT только изображение отражается
     * @field CLAMP_TO_EDGE  Привязывает координаты между 0 и 1. В результате выходящие
     * за пределы координаты будут привязаны к границе текстуры
     * @field CLAMP_TO_BORDER Координаты, выходящие за пределы диапазона будут давать установленный пользователем цвет границы.
    */
    enum class TextureWrapping
    {
        REPEAT = GL_REPEAT,
        MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
        CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
        CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
    };
}

#endif /* TextureMappingSetting_hpp */
