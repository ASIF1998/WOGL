//
//  Context.inl
//  WOGL
//
//  Created by Асиф Мамедов on 04/02/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include <SDL2/SDL.h>
#include <GL/glew.h>

#include <string>

#include <memory>

#include <stdexcept>

#include "ModelRenderer.hpp"

#include <initializer_list>

namespace WOGL
{
    class ColorAttachments
    {
    public:
        ColorAttachments(const initializer_list<uint32_t>& l) :
            _ca(l.size())
        {
            auto* ptr_l = begin(l);
            
            for (size_t i{0}; i < l.size(); i++) {
                _ca[i] = GL_COLOR_ATTACHMENT0 + ptr_l[i];
            }
        }

        vector<GLenum>& colorAttachments() noexcept
        {
            return _ca;
        }

        const vector<GLenum>& colorAttachments() const noexcept
        {
            return _ca;
        }

        size_t size() const noexcept
        {
            return _ca.size();
        }
        
    private:
        vector<GLenum> _ca;
    };

    auto contextDeleter = [](SDL_GLContext* context)
    {
        SDL_GL_DeleteContext(*context);
    };

    class Context
    {
        using PtrContext = unique_ptr<SDL_GLContext, decltype(contextDeleter)>;

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
        static void inline clearColor(float r, float g, float b, float a) noexcept
        {
            glClearColor(r, g, b, a);
        }

        /**
         * Метод с помощью которой можно задать значения для очистки буфера глубины.
         *
         * @param d значение котрым очищается буффер
        */
        static inline void clearDepth(double d) noexcept
        {
            glClearDepth(d);
        }

        /**
         * Метод с помощью которой можно задать значения для очистки буфера трафарета.
         *
         * @param s значение котрым очищается буффер
        */
        static inline void clearStencil(int32_t s) noexcept
        {
            glClearStencil(s);
        }

        /*
         * Метод, необходимый для очистки буфера цвета.
        */
        static inline void clearColorBuffer() noexcept
        {
            glClear(GL_COLOR_BUFFER_BIT);
        }

        /**
         * Метод, необходимый для очистки буфера глубины.
        */
        static inline void clearDepthBuffer() noexcept
        {
            glClear(GL_DEPTH_BUFFER_BIT);
        }

        /**
         * Метод, необходимый для очистки буффера траффарета.
        */
        static inline void clearStencilBuffer() noexcept
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
        static inline void draw(DrawPrimitive primitive, int32_t size, int32_t numberRepetitions = 1) noexcept
        {
            glDrawElementsInstanced(static_cast<GLenum>(primitive), size, GL_UNSIGNED_INT, nullptr, numberRepetitions);
        }

        /**
         * Статический метод необходимый для отрисовки элемента столько раз, сколько будет находится
         * в numberRepetitions.
         *
         * @param primitive тип рисуемого примитива
         * @param size количество визуализируемых вершин
         * @param ca объект с текстурными прикреплениями (например вот таких: ColorAttachments ca {0, 1, 4})
         * @param numberRepetitions хранит информацию о количестве проходов рендера
        */
        static inline void draw(DrawPrimitive primitive, int32_t size, const ColorAttachments& ca, int32_t numberRepetitions = 1) noexcept
        {
            glDrawBuffers(static_cast<int32_t>(ca.size()), &ca.colorAttachments()[0]);
            glDrawElementsInstanced(static_cast<GLenum>(primitive), size, GL_UNSIGNED_INT, nullptr, numberRepetitions);
            
        }

        /**
         * Статический метод необходимый для отрисовки элемента столько раз, сколько будет находится
         * в numberRepetitions.
         *
         * @param primitive тип рисуемого примитива
         * @param indexBuffer буффер индексов
         * @param numberRepetitions хранит информацию о количестве проходов рендера
        */
        template<typename IndexBuffer>
        static inline void draw(DrawPrimitive primitive, const IndexBuffer& indexBuffer, int32_t numberRepetitions = 1) noexcept
        {
            glDrawElementsInstanced(static_cast<GLenum>(primitive), indexBuffer._size, GL_UNSIGNED_INT, nullptr, numberRepetitions);
        }

        /**
         * Статический метод необходимый для отрисовки элемента столько раз, сколько будет находится
         * в numberRepetitions.
         *
         * @param primitive тип рисуемого примитива
         * @param indexBuffer буффер индексов
         * @param ca объект с текстурными прикреплениями (например вот таких: ColorAttachments ca {0, 1, 4})
         * @param numberRepetitions хранит информацию о количестве проходов рендера
        */
        template<typename IndexBuffer>
        static inline void draw(DrawPrimitive primitive, const IndexBuffer& indexBuffer, const ColorAttachments& ca, int32_t numberRepetitions = 1) noexcept
        {
            glDrawBuffers(static_cast<int32_t>(ca.size()), &ca.colorAttachments()[0]);
            glDrawElementsInstanced(static_cast<GLenum>(primitive), indexBuffer._size, GL_UNSIGNED_INT, nullptr, numberRepetitions);
        }

        /**
         * Статический метод необходимый для отрисовки элемента столько раз, сколько будет находится
         * в numberRepetitions.
         * 
         * @param modelRenderer модель 
         * @param numberRepetitions хранит информацию о количестве проходов рендера
        */
        template<TexelFormat Tf, template<TexelFormat> typename T>
        static void draw(const T<Tf>& modelRenderer, int32_t numberRepetitions = 1)
        {
            for (size_t i{0}, size{modelRenderer._texturersRenderer.size()}; i < size; i++) {
                modelRenderer._texturersRenderer[i].first.bind(modelRenderer._texturersRenderer[i].second);
            }

            for (size_t i{0}; i < modelRenderer._meshRenderers.size(); i++) {
                modelRenderer._meshRenderers[i].draw(numberRepetitions);
            }
        }

        /**
         * Статический метод необходимый для отрисовки элемента столько раз, сколько будет находится
         * в numberRepetitions.
         *
         * @param modelRenderer модель
         * @param ca объект с текстурными прикреплениями (например вот таких: ColorAttachments ca {0, 1, 4})
         * @param numberRepetitions хранит информацию о количестве проходов рендера
        */
        template<TexelFormat Tf, template<TexelFormat> typename T>
        static void draw(const T<Tf>& modelRenderer, const ColorAttachments& ca, int32_t numberRepetitions = 1)
        {
            for (size_t i{0}, size{modelRenderer._texturersRenderer.size()}; i < size; i++) {
                modelRenderer._texturersRenderer[i].first.bind(modelRenderer._texturersRenderer[i].second);
            }

            glDrawBuffers(static_cast<int32_t>(ca.size()), &ca.colorAttachments()[0]);

            for (size_t i{0}; i < modelRenderer._meshRenderers.size(); i++) {
                modelRenderer._meshRenderers[i].draw(numberRepetitions);
            }

        }

        /**
         * Статический метод необходимый для отрисовки сразу нескольких моделей столько раз, сколько будет находится,
         * в numberRepetitions.
         * 
         * @param modelsRenderer модели
         * @param numberRepetitions хранит информацию о количестве проходов рендера
        */
        template<typename ContainerWithModelsRenderer>
        static void draw(const ContainerWithModelsRenderer& modelsRenderer, int32_t numberRepetitions = 1)
        {
            for (size_t i{0}; i < modelsRenderer.size(); i++) {
                draw(modelsRenderer[i], numberRepetitions);
            }
        }

        /**
         * Статический метод необходимый для отрисовки сразу нескольких моделей столько раз, сколько будет находится,
         * в numberRepetitions.
         *
         * @param modelsRenderer модели
         * @param ca объект с текстурными прикреплениями (например вот таких: ColorAttachments ca {0, 1, 4})
         * @param numberRepetitions хранит информацию о количестве проходов рендера
         */
        template<typename ContainerWithModelsRenderer>
        static void draw(const ContainerWithModelsRenderer& modelsRenderer, const ColorAttachments& ca, int32_t numberRepetitions = 1)
        {
            glDrawBuffers(static_cast<int32_t>(ca.size()), &ca.colorAttachments()[0]);

            for (size_t i{0}; i < modelsRenderer.size(); i++) {
                draw(modelsRenderer[i], numberRepetitions);
            }
        }

        /**
         * Функция предназначенная для выявления ошибок OpenGL.
         *
         * @throw runtime_error в случае если произошла ошибка
        */
        static void checkError()
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

        static inline void enable(Enable e) noexcept
        {
            glEnable(static_cast<GLenum>(e));
        }

        static inline void disable(Enable d) 
        {
            glDisable(static_cast<GLenum>(d));
        }

        /**
         * Тест ножницами.
         * Статический метод, определяющий прямоугольник в экранных координатах.
         * 
         * @param x определяет точку левого нижнего прямоугольника по оси X
         * @param y определяет точку левого нижнего прямоугольника по оси Y
         * @param width опредеяет ширину прямоугольника
         * @param height опредеяет высоту прямоугольника
        */
        static inline void scitionTest(int32_t x, int32_t y, int32_t width, int32_t height)
        {
            glScissor(x, y, width, height);
        }

        static inline void alpthFunc(Func f, float res) noexcept
        {
            glAlphaFunc(static_cast<GLenum>(f), res);
        }

        static inline void stensil(Func s1, int32_t s2, uint32_t s3) noexcept
        {
            glStencilFunc(static_cast<GLenum>(s1), s2, s3);
        }

        /**
         * Статический метод устанавливающий действие к передней или задней грани.
         * 
         * @param face устанавливает какая грань
         * @param sfail действие применяемое когда тест трафарета провалился 
         * @param zfail действие применяемое когда тест трафарета пройден но тест глубины нет 
         * @param pass действие применяемое когда тест трафарета и глубины пройдены
        */
        static inline void stensilOperatorSeparate(Face face, StencilValueOperation sfail, StencilValueOperation zfail, StencilValueOperation pass) noexcept
        {
            glStencilOpSeparate(static_cast<GLenum>(face), static_cast<GLenum>(sfail), static_cast<GLenum>(zfail), static_cast<GLenum>(pass));
        }

        static inline void depth(Func d) noexcept
        {
            glDepthFunc(static_cast<GLenum>(d));
        }

        static inline void depthMask(bool d) noexcept
        {
            glDepthMask(static_cast<GLenum>(d));
        }

        static inline void blend(BlendFunc b1, BlendFunc b2) noexcept
        {
            glBlendFunc(static_cast<GLenum>(b1), static_cast<GLenum>(b2));
        }

        /**
         * Метод предназначенный для включения и выключения записи компонентов цвета кадрового буфера.
        */
        static inline void colorMask(bool r, bool g, bool b, bool a) noexcept
        {
            glColorMask(static_cast<GLenum>(r), static_cast<GLenum>(g), static_cast<GLenum>(b), static_cast<GLenum>(a));
        }

        static inline void cullFace(Face f) noexcept
        {
            glCullFace(static_cast<GLenum>(f));
        }

        static inline void faceTraversal(FaceTraversal fv) noexcept
        {
            glFrontFace(static_cast<GLenum>(fv));
        }

        static inline void viewport(int32_t x, int32_t y, int32_t width, int32_t height) noexcept
        {
            glViewport(x, y, width, height);
        }

        static inline void frameBufferSRGB(bool turnOn) noexcept
        {
            if (turnOn) {
                glEnable(GL_FRAMEBUFFER_SRGB);
            } else {
                glDisable(GL_FRAMEBUFFER_SRGB);
            }
        }

        /**
         * Метод, задерживающий программу на указанное количество миллисекунд.
         *
         * @param ms милисекунды
        */
        static inline void delay(uint32_t ms) noexcept
        {
            SDL_Delay(ms);
        }

    private:
        PtrContext _context;
    };
}
