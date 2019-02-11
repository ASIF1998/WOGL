//
//  Context.inl
//  WOGL
//
//  Created by Асиф Мамедов on 04/02/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include <SDL2/SDL.h>

#include <string>

#include <memory>

#include <stdexcept>

using namespace std;

namespace WOGL
{
    auto contextDeleter = [](SDL_GLContext* context)
    {
        SDL_GL_DeleteContext(*context);
    };

    class Context
    {
    public:
        /**
         * Конструктор.
         *
         * @param window окно
        */
        template<typename WindowType>
        explicit constexpr Context(const WindowType& window) noexcept :
            _context{new SDL_GLContext{SDL_GL_CreateContext(window._window.get())}, contextDeleter}
        {
            glewExperimental = GL_TRUE;
            glewInit();
        }

        Context(const Context&) = delete;
        Context(Context&&) = delete;
        Context operator=(const Context&) = delete;
        Context operator=(Context&&) = delete;

        /**
         * Метод необходимый для установки значения которым будет очищаться
         * буффер цветов.
         *
         * @param r канал red
         * @param g канал green
         * @param b канал blue
         * @param a каал alpha
        */
        void clearColor(float r, float g, float b, float a) noexcept
        {
            glClearColor(r, g, b, a);
        }

        /**
         * Метод с помощью которой можно задать значения для очистки буфера глубины.
         *
         * @param d значение котроым очищается буффер
        */
        static void clearDepth(double d) noexcept
        {
            glClearDepth(d);
        }

        /**
         * Метод с помощью которой можно задать значения для очистки буфера трафарета.
         *
         * @param s значение котроым очищается буффер
        */
        static void clearStencil(int32_t s) noexcept
        {
            glClearStencil(s);
        }

        /*
            * Метод, необходимый для очистки буффера цвета.
        */
        static void clearColorBuffer() noexcept
        {
            glClear(GL_COLOR_BUFFER_BIT);
        }

        /**
         *  Метод, необходимый для очистки буффера глубины.
        */
        static void clearDepthBuffer() noexcept
        {
            glClear(GL_DEPTH_BUFFER_BIT);
        }

        /**
         * Метод, необходимый для очистки буффера траффарета.
        */
        static void clearStencilBuffer() noexcept
        {
            glClear(GL_STENCIL_BUFFER_BIT);
        }

        /**
         * Статический метод необходимый для отрисовки элемента столько раз, сколько будет находится
         * в numberRepetitions.
         *
         * @param primitive тип рисуемого примитива
         * @param size количество визуализируемых вершин
         * @param numberRepetitions хранит информацию о количестве проходов рендера
        */
        static void draw(DrawPrimitive primitive, int32_t size, int32_t numberRepetitions = 1) noexcept
        {
            glDrawElementsInstanced(static_cast<GLenum>(primitive), size, GL_UNSIGNED_INT, nullptr, numberRepetitions);
        }

        /**
         * Функция предназначенная для выявления ошибок OpenGL.
         *
         * @throw runtime_error в случае если произошла ошибка
        */
        void chechError()
        {
            string err = "";
            
            switch (glGetError()) {
                case GL_INVALID_ENUM: {
                    err = "An invalid value is specified for the enumerated argument";
                    break;
                }

                case GL_INVALID_VALUE: {
                    err = "Numeric argument is out of range.";
                    break;
                }

                case GL_INVALID_OPERATION: {
                    err = "The specified operation is not allowed in its current state.";
                    break;
                }

                case GL_INVALID_FRAMEBUFFER_OPERATION: {
                    err = "The framebuffer object is not complete.";
                    break;
                }

                case GL_OUT_OF_MEMORY: {
                    err = "Not enough memory to execute the command.";
                    break;
                }

                case GL_STACK_OVERFLOW: {
                    err = "An attempt was made to perform an operation that could lead to an overflow of the internal stack.";
                    break;
                }

                case GL_STACK_UNDERFLOW: {
                    err = "An attempt was made to perform an operation that could lead to an overflow of the internal stack.";
                    break;
                }
            }
            
            if (!err.empty()) {
                throw runtime_error(err);
            }
        }

        void enable(Enable e) noexcept
        {
            glEnable(static_cast<GLenum>(e));
        }

        void stensil(StensilFunc s1, StensilFunc s2, StensilFunc s3) noexcept
        {
            glStencilFunc(static_cast<GLenum>(s1), static_cast<GLenum>(s2), static_cast<GLenum>(s3));
        }

        void depth(DethFunc d) noexcept
        {
            glDepthFunc(static_cast<GLenum>(d));
        }

        void blend(BlendFunc b1, BlendFunc b2) noexcept
        {
            glBlendFunc(static_cast<GLenum>(b1), static_cast<GLenum>(b2));
        }

        void cullFace(Face f) noexcept
        {
            glCullFace(static_cast<GLenum>(f));
        }

        void faceTraversal(FaceTraversal fv) noexcept
        {
            glFrontFace(static_cast<GLenum>(fv));
        }

    private:
        unique_ptr<SDL_GLContext, decltype(contextDeleter)> _context;
    };
}
