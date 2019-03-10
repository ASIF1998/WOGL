//
//  Window.inl
//  WOGL
//
//  Created by Асиф Мамедов on 04/02/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include <SDL2/SDL.h>

#include <memory>

#include <string_view>
#include <string>

#include <stdexcept>

using namespace std;

namespace WOGL
{
    auto windowDeleter = [](SDL_Window* window)
    {
        SDL_DestroyWindow(window);
    };

    /**
     * @template DepthSize количесвто бит в буфере глубины
     * @template StensilSize  количесвто бит в буфере трафарета
     * @template Multisamples количество семплеров(необходимо при включённом MSAA)
    */
    template<bool Resizable = false, int32_t DepthSize = 24, int32_t StensilSize = 8, int32_t Multisamples = 4>
    class Window
    {
        using PtrWindow = unique_ptr<SDL_Window, decltype(windowDeleter)>;
        
        friend class Context;

    public:
        /**
         * Конструктор.
         *
         * @param title загаловок окна
         * @throw runtime_error в случае если не удалось создать окна
        */
        explicit Window(const string_view title, int32_t width, int32_t height) :
            _window{nullptr, windowDeleter}
        {
            if constexpr (Resizable) {
                _window.reset(SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN));
            } else {
                _window.reset(SDL_CreateWindow(title.data(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN));
            }

            if (!_window) {
                string msg(SDL_GetError());
                SDL_ClearError();
                throw runtime_error(msg);
            }
            
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
            SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, DepthSize);
            SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, StensilSize);
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, Multisamples);
        }

        Window(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(const Window&) = delete;
        Window& operator=(Window&&) = delete;

        tuple<int32_t, int32_t> size() const
        {
            int w = 0, h = 0;
            SDL_GetWindowSize(_window.get(), &w, &h);
            return make_tuple(w, h);
        }
        
        tuple<int32_t, int32_t> drawableSize() const
        {
            int w = 0, h = 0;
            SDL_GL_GetDrawableSize(_window.get(), &w, &h);
            return make_tuple(w, h);
        }
        
        void size(int32_t w, int32_t h) noexcept
        {
            SDL_SetWindowSize(_window.get(), w, h);
        }

        string title() const noexcept
        {
            return SDL_GetWindowTitle(_window.get());
        }

        void title(const string_view &text) noexcept
        {
            SDL_SetWindowTitle(_window.get(), text.data());
        }

        void present() const noexcept
        {
            SDL_GL_SwapWindow(_window.get());
        }

    private:
        PtrWindow _window;
    };

}
