//
//  TextureRenderer.inl
//  WOGL
//
//  Created by Асиф Мамедов on 09/02/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "../Data/Texture.hpp"

namespace WOGL
{
    class InitializeTextureRenderer
    {
    public:
        /**
         * Конструктор.
         *
         * Выделеннуя в GPU память нельзя будет изменить !!!
         *
         * @param texture текстура
         * @param texelFormat тип текселя
        */
        template<typename TextureType>
        explicit InitializeTextureRenderer(const TextureType& texture, TexelFormat texelFormat) :
            _width{static_cast<int32_t>(texture._width)},
            _height{static_cast<int32_t>(texture._height)}
        {
            _createHandle();

            glBindTexture(GL_TEXTURE_2D, _textureRendererHandle);
            glTexStorage2D(GL_TEXTURE_2D, 1, static_cast<GLenum>(texelFormat), _width, _height);
            update(texture);
            glGenerateMipmap(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        /**
         * Конструктор.
         *
         * Выделеннуя в GPU память нельзя будет изменить !!!
         *
         * @param texture указатель на текстуру
         * @param texelFormat тип текселя
        */
        template<typename TextureType>
        explicit InitializeTextureRenderer(const unique_ptr<TextureType>& texture, TexelFormat texelFormat) :
            InitializeTextureRenderer(*texture, texelFormat)
        {
        }

        /**
         * Конструктор.
         *
         * Выделеннуя в GPU память нельзя будет изменить !!!
         *
         * @param texture указатель на текстуру
         * @param texelFormat тип текселя
        */
        template<typename TextureType, template<typename> typename PtrTexture>
        explicit InitializeTextureRenderer(const PtrTexture<TextureType>& texture, TexelFormat texelFormat) :
                InitializeTextureRenderer(*texture, texelFormat)
        {
        }

        InitializeTextureRenderer(InitializeTextureRenderer&& initTexRenderer) :
            _width{initTexRenderer._width},
            _height{initTexRenderer._height},
            _textureRendererHandle{0}
        {
            swap(_textureRendererHandle, initTexRenderer._textureRendererHandle);
        }

        InitializeTextureRenderer(const InitializeTextureRenderer&) = delete;
        InitializeTextureRenderer& operator=(const InitializeTextureRenderer&) = delete;
        InitializeTextureRenderer& operator=(InitializeTextureRenderer&&) = delete;

        ~InitializeTextureRenderer()
        {
            if (_textureRendererHandle) {
                glDeleteTextures(1, &_textureRendererHandle);
            }
        }

        /**
         * Метод предназначенный для обновления данных объекта.
         *
         * @param texture текстура котороя будет помещена в памя GPU
        */
        template<typename DataType, TexelType Tx>
        void update(const Texture<DataType, Tx>& texture)
        {
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, static_cast<GLenum>(Tx), _type<DataType>(), &texture._data[0]);
        }

        /**
         * Метод предназначенный для обновления данных объекта.
         *
         * @param texture указатель на текстуру котороя будет помещена в памя GPU
        */
        template<typename DataType, TexelType Tx>
        void update(const unique_ptr<Texture<DataType, Tx>>& texture)
        {
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, static_cast<GLenum>(Tx), _type<DataType>(), &texture->_data[0]);
        }

        /**
         * Метод предназначенный для обновления данных объекта.
         *
         * @param texture указатель на текстуру котороя будет помещена в памя GPU
        */
        template<typename DataType, TexelType Tx>
        void update(const weak_ptr<Texture<DataType, Tx>>& texture)
        {
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _width, _height, static_cast<GLenum>(Tx), _type<DataType>(), &texture->_data[0]);
        }

    protected:
        inline void _createHandle()
        {
            glGenTextures(1, &_textureRendererHandle);

            if (!_textureRendererHandle) {
                throw runtime_error("Error create texture renderer handle");
            }
        }

        template<typename T>
        GLenum _type() const noexcept 
        {
            GLenum type = GL_FLOAT;

            if constexpr (is_same_v<T, int8_t>) {
                type = GL_BYTE;
            } else if constexpr (is_same_v<T, int16_t>) {
                type = GL_SHORT;
            } else if constexpr (is_same_v<T, int32_t>) {
                type = GL_INT;
            } else if constexpr (is_same_v<T, uint8_t>) {
                type = GL_UNSIGNED_BYTE;
            } else if constexpr (is_same_v<T, uint16_t>) {
                type = GL_UNSIGNED_SHORT;
            } else if constexpr (is_same_v<T, uint32_t>) {
                type = GL_UNSIGNED_INT;
            } 

            return type;
        }

        uint32_t _textureRendererHandle;
        int32_t _width;
        int32_t _height;
    };

    template<TexelFormat Tf>
    class TextureRenderer : 
        public InitializeTextureRenderer
    {
    public:
        /**
         * Конструктор.
         *
         * Выделеннуя в GPU память нельзя будет изменить !!!
         *
         * @param texture текстура
        */
        template<typename TextureType>
        explicit TextureRenderer(const TextureType& texture) : 
            InitializeTextureRenderer(texture, Tf)
        {
        }

        /**
         * Метод делающий текстуру текущей.
        */
        inline void bind(int32_t slot = 0) const noexcept 
        {
            if (slot >= 0) {
                glActiveTexture(GL_TEXTURE0 + slot);
                glBindTexture(GL_TEXTURE_2D, _textureRendererHandle);
            }
        }

        /**
         * Метод делающий текстуру не текущей. 
         * По сути он деалет текущую цель привязки нулевой.
        */
        inline static void unbind() noexcept
        {
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        inline constexpr int32_t width() const noexcept
        {
            return _width;
        }

        inline constexpr int32_t height() const noexcept
        {
            return _height;
        }

        inline constexpr TexelFormat texelFormat() const noexcept
        {
            return Tf;
        }

        /**
         * Метод необходимый для определения способа минимизации текстуры.
         *
         * @param f способа минимизации текстуры
        */
        void minFilter(TextureFilter f) const noexcept
        {
            glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(f));
        }

        /**
         * Метод необходимый для определения способа увелечения текстуры.
         *
         * @param f способа увелечения текстуры
        */
        void magFilter(TextureFilter f) const noexcept
        {
            glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(f));
        }

        /**
         * Метод отвечающий за определение способа оптекания текстуры по оси S(т. е. X).
         *
         * @param s определяет способ оптекания по оси S(определение TextureWrapping находится выше)
        */
        void textureWrappingS(TextureWrapping s) const noexcept
        {
            glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLenum>(s));
        }

        /**
         * Метод отвечающий за определение способа оптекания текстуры по оси T(т. е. Y).
         *
         * @param t определяет способ оптекания по оси T(определение TextureWrapping находится выше)
        */
        void textureWrappingT(TextureWrapping t) const noexcept
        {
            glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLenum>(t));
        }
    };
}

