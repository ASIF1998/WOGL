//
//  WOGL.hpp
//  WOGL
//
//  Created by Асиф Мамедов on 03/02/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#ifndef WOGL_hpp
#define WOGL_hpp

#include "Render/Window.hpp"
#include "Render/Context.hpp"

namespace WOGL
{
    /**
     * Функция предназначенная для инициализации подсистем
     * (инициализации библиотеки SDL2). Эту функцию необходимо
     * вызывать перед началом использования классов и других
     * функций WOGL.
    */
    void init();
    
    /**
     * Функция предназначенная для очистки всех инициализирванных подсистем.
     * Её необходимо вызывать перед завершением программы.
    */
    void quit();
    
    /**
     * Функция с помощью которой можно узнать о инициализации SDL2.
     * true - SDL2 инициализирован;
     * false - SDL2 не инициализирован.
    */
    bool initOpenGL();
}

#endif /* WOGL_hpp */
