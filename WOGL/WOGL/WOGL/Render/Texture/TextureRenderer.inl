//
//  TextureRenderer.inl
//  WOGL
//
//  Created by Асиф Мамедов on 09/02/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "../../Data/Texture.hpp"

namespace WOGL
{
    template<TexelFormat>
    class BaseFramebuffer;
    
    class InitializeTextureRenderer
    {
        friend class InitializeCubeMapTextureRenderer;
        
    protected:
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
            _height{static_cast<int32_t>(texture._height)},
            _width{static_cast<int32_t>(texture._height)}
        {
            _createHandle();

            glBindTexture(GL_TEXTURE_2D, _textureRendererHandle);
            glTexStorage2D(GL_TEXTURE_2D, 1, static_cast<GLenum>(texelFormat), _height, _width);
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

        explicit InitializeTextureRenderer(int32_t width, int32_t height, TexelFormat texelFormat) :
            _height{height},
            _width{width}
        {
            if (_width == 0 || _height == 0) {
                throw invalid_argument("Transferred size is zero");
            }

            _createHandle();

            glBindTexture(GL_TEXTURE_2D, _textureRendererHandle);
            glTexStorage2D(GL_TEXTURE_2D, 1, static_cast<GLenum>(texelFormat), _height, _width);
            glBindTexture(GL_TEXTURE_2D, 0);
        }

        InitializeTextureRenderer(InitializeTextureRenderer&& initTexRenderer) :
            _height{initTexRenderer._height},
            _width{initTexRenderer._width},
            _textureRendererHandle{0}
        {
            swap(_textureRendererHandle, initTexRenderer._textureRendererHandle);
        }

        InitializeTextureRenderer(const InitializeTextureRenderer&) = delete;
        InitializeTextureRenderer& operator=(const InitializeTextureRenderer&) = delete;
        InitializeTextureRenderer& operator=(InitializeTextureRenderer&&) = delete;

        virtual ~InitializeTextureRenderer()
        {
            if (_textureRendererHandle) {
                glDeleteTextures(1, &_textureRendererHandle);
            }
        }

    public:
        /**
         * Метод предназначенный для обновления данных объекта.
         *
         * @param texture текстура котороя будет помещена в памя GPU
        */
        template<typename DataType, TexelType Tx>
        void update(const Texture<DataType, Tx>& texture)
        {
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _height, _width, static_cast<GLenum>(Tx), _type<DataType>(), &texture._data[0]);
        }

        /**
         * Метод предназначенный для обновления данных объекта.
         *
         * @param texture указатель на текстуру котороя будет помещена в памя GPU
        */
        template<typename DataType, TexelType Tx>
        void update(const unique_ptr<Texture<DataType, Tx>>& texture)
        {
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _height, _width, static_cast<GLenum>(Tx), _type<DataType>(), &texture->_data[0]);
        }

        /**
         * Метод предназначенный для обновления данных объекта.
         *
         * @param texture указатель на текстуру котороя будет помещена в памя GPU
        */
        template<typename DataType, TexelType Tx>
        void update(const weak_ptr<Texture<DataType, Tx>>& texture)
        {
            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, _height, _width, static_cast<GLenum>(Tx), _type<DataType>(), &texture->_data[0]);
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
        static GLenum _type() noexcept
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
        int32_t _height;
        int32_t _width;
    };

    template<TexelFormat Tf>
    class TextureRenderer : 
        public InitializeTextureRenderer
    {
        friend class BaseFramebuffer<Tf>;

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

        explicit TextureRenderer(int32_t width, int32_t height) :
            InitializeTextureRenderer(width, height, Tf)
        {
        }

        TextureRenderer(TextureRenderer&& tr) :
            InitializeTextureRenderer(forward<InitializeTextureRenderer>(tr))
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
         * Метод необходимый для определения способа увелечения текстуры.
         *
         * @param mf способа минимизации текстуры
        */
        static inline void magFilter(const TextureFilter mf) noexcept
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(mf));
        }
        
        /**
         * Метод необходимый для определения способа минимизации текстуры.
         *
         * @param mf способа увелечения текстуры
        */
        static inline void minFilter(const TextureFilter mf) noexcept
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(mf));
        }
        
        /**
         * Метод отвечающий за определение способа оптекания текстуры по оси S(т. е. X).
         *
         * @param ws определяет способ оптекания по оси S(определение TextureWrapping находится выше)
        */
        static inline void textureWrappingS(const TextureWrapping ws) noexcept
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLenum>(ws));
        }
        
        /**
         * Метод отвечающий за определение способа оптекания текстуры по оси T(т. е. Y).
         *
         * @param wt определяет способ оптекания по оси T(определение TextureWrapping находится выше)
        */
        static inline void textureWrappingT(const TextureWrapping wt) noexcept
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLenum>(wt));
        }
        
        /**
         * Устанавливает режим сравнения текстур для текущих привязанных текстур глубины.
        */
        static inline void textureCompareMode(const TextureCompareMode cm) noexcept
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, static_cast<GLenum>(cm));
        }
        
        /**
         * Устанавливает оператор сравнения, используемый, когда для TextureCompareMode установлено значение COMPARE_REF_TO_TEXTURE.
        */
        static inline void textureCompareFunc(const TextureCompareFunc cf) noexcept
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, static_cast<GLenum>(cf));
        }
    };
}

