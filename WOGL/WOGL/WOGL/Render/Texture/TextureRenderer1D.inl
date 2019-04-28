//
//  TextureRenderer1D.inl
//  WOGL
//
//  Created by Асиф Мамедов on 06/04/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "TextureRenderer.hpp"
#include "../../Data/Texture1D.hpp"

#include <memory>

namespace WOGL
{
    class BaseTextureRenderer1D :
        public BaseTextureRenderer
    {
    protected:
        /**
         * Конструктор.
         *
         * Выделенную в GPU память нельзя будет изменить !!!
         *
         * @param texture текстура
         * @param tf тип текселя
        */
        template<typename TextureType>
        explicit BaseTextureRenderer1D(const TextureType& texture, TexelFormat tf) :
            BaseTextureRenderer(),
            _size{static_cast<int32_t>(texture._data.size())}
        {
            glBindTexture(GL_TEXTURE_1D, _textureRendererHandle);
            glTexStorage1D(GL_TEXTURE_1D, 1, static_cast<GLenum>(tf), _size);
            update(texture);
            glBindTexture(GL_TEXTURE_1D, 0);
        }

        /**
         * Конструктор.
         *
         * Выделенную в GPU память нельзя будет изменить !!!
         *
         * @param texture указатель на текстуру
         * @param tf тип текселя
        */
        template<typename TextureType, typename DelType, template<typename, typename> typename Ptr>
        explicit BaseTextureRenderer1D(const Ptr<TextureType, DelType>& texture, TexelFormat tf) :
            BaseTextureRenderer(),
            _size{static_cast<int32_t>(texture->_data.size())}
        {
            glBindTexture(GL_TEXTURE_1D, _textureRendererHandle);
            glTexStorage1D(GL_TEXTURE_1D, 1, static_cast<GLenum>(tf), _size);
            update(texture);
            glBindTexture(GL_TEXTURE_1D, 0);
        }

        /**
         * Конструктор.
         *
         * Выделенную в GPU память нельзя будет изменить !!!
         *
         * @param texture указатель на текстуру
         * @param tf тип текселя
        */
        template<typename TextureType, template<typename> typename Ptr>
        explicit BaseTextureRenderer1D(const Ptr<TextureType>& texture, TexelFormat tf) :
            BaseTextureRenderer(),
            _size{static_cast<int32_t>(texture->_data.size())}
        {
            glBindTexture(GL_TEXTURE_1D, _textureRendererHandle);
            glTexStorage1D(GL_TEXTURE_1D, 1, static_cast<GLenum>(tf), _size);
            update(texture);
            glBindTexture(GL_TEXTURE_1D, 0);
        }

        /**
         * Конструктор.
         * 
         * Выделенную в GPU память нельзя будет изменить !!!
         * 
         * @param size размер одномерной текстуры
         * @param tf тип текселя
         * @throw в случае если size равен нулю
        */
        explicit BaseTextureRenderer1D(int32_t size, TexelFormat tf) :
            BaseTextureRenderer(),
            _size{size}
        {
            assert(!(size == 0));

            glBindTexture(GL_TEXTURE_1D, _textureRendererHandle);
            glTexStorage1D(GL_TEXTURE_1D, 1, static_cast<GLenum>(tf), _size);
            glBindTexture(GL_TEXTURE_1D, 0);
        }

        BaseTextureRenderer1D(BaseTextureRenderer1D&& texture) :
            BaseTextureRenderer{move(texture)},
            _size{texture._size}
        {
        }

        BaseTextureRenderer1D(const BaseTextureRenderer1D&) = delete;
        BaseTextureRenderer1D& operator=(const BaseTextureRenderer1D&) = delete;
        BaseTextureRenderer1D& operator=(BaseTextureRenderer1D&&) = delete;

    public:
        /**
         * Метод предназначенный для обновления данных объекта.
         *
         * @param texture текстура котороя будет помещена в памя GPU
        */
        template<typename DataType, TexelType Tx>
        inline void update(const Texture1D<DataType, Tx>& texture)
        {
            glTexSubImage1D(GL_TEXTURE_1D, 0, 0, _size, static_cast<GLenum>(Tx), _type<DataType>(), &texture._data[0]);
        }

        /**
         * Метод предназначенный для обновления данных объекта.
         *
         * @param texture указатель на текстуру котороя будет помещена в памя GPU
        */
        template<typename DataType, TexelType Tx, typename DelType, template<typename, typename> typename Ptr>
        inline void update(const Ptr<Texture1D<DataType, Tx>, DelType>& texture)
        {
            glTexSubImage1D(GL_TEXTURE_1D, 0, 0, _size, static_cast<GLenum>(Tx), _type<DataType>(), &texture->_data[0]);
        }
        
        /**
         * Метод предназначенный для обновления данных объекта.
         *
         * @param texture указатель на текстуру котороя будет помещена в памя GPU
        */
        template<typename DataType, TexelType Tx, template<typename> typename Ptr>
        inline void update(const Ptr<Texture1D<DataType, Tx>>& texture)
        {
            glTexSubImage1D(GL_TEXTURE_1D, 0, 0, _size, static_cast<GLenum>(Tx), _type<DataType>(), &texture->_data[0]);
        }

        /**
         * Метод необходимый для определения способа увеличения текстуры.
         *
         * @param mf способа минимизации текстуры
        */
        static inline void magFilter(const TextureFilter mf) noexcept
        {
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(mf));
        }
        
        /**
         * Метод необходимый для определения способа минимизации текстуры.
         *
         * @param mf способа увеличения текстуры
        */
        static inline void minFilter(const TextureFilter mf) noexcept
        {
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(mf));
        }
        
        /**
         * Метод отвечающий за определение способа оптекания текстуры по оси S(т. е. X).
         *
         * @param ws определяет способ оптекания по оси S(определение TextureWrapping находится выше)
        */
        static inline void textureWrappingS(const TextureWrapping ws) noexcept
        {
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, static_cast<GLenum>(ws));
        }
        
        /**
         * Устанавливает режим сравнения текстур для текущих привязанных текстур глубины.
        */
        static inline void textureCompareMode(const TextureCompareMode cm) noexcept
        {
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_COMPARE_MODE, static_cast<GLenum>(cm));
        }
        
        /**
         * Устанавливает оператор сравнения, используемый, когда для TextureCompareMode установлено значение COMPARE_REF_TO_TEXTURE.
        */
        static inline void textureCompareFunc(const TextureCompareFunc cf) noexcept
        {
            glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_COMPARE_FUNC, static_cast<GLenum>(cf));
        }

        /**
         * Метод делающий текстуру не текущей.
         * По сути он деалет текущую цель привязки нулевой.
        */
        inline static void unbind() noexcept
        {
            glBindTexture(GL_TEXTURE_1D, 0);
        }

        int32_t size() const noexcept
        {
            return _size;
        }

    protected:
        int32_t _size;
    };

    template<TexelFormat Tf>
    class TextureRenderer1D :
        public BaseTextureRenderer1D
    {
    public:
        /**
         * Конструктор.
         * 
         * @param texture одномерная текстура
        */
        template<typename TextureType>
        explicit TextureRenderer1D(const TextureType& texture) :
            BaseTextureRenderer1D{texture, Tf}
        {
        }

        /**
         * Конструктор.
         * 
         * @param размер одномерной текстуры
        */
        explicit TextureRenderer1D(int32_t size) :
            BaseTextureRenderer1D{size, Tf}
        {
        }

        TextureRenderer1D(TextureRenderer1D&& texture) :
            BaseTextureRenderer1D{forward<BaseTextureRenderer1D>(texture)}
        {
        }

        TextureRenderer1D(const TextureRenderer1D&) = delete;
        TextureRenderer1D& operator=(const TextureRenderer1D&) = delete;
        TextureRenderer1D& operator=(TextureRenderer1D&&) = delete;

        /**
         * Метод необходимый для генерации mipmap'а.
        */
         virtual inline void genMipmap() const noexcept override
         {
            glGenerateMipmap(GL_TEXTURE_1D);
         }

        /**
         * Метод делающий текстуру текущей.
         * 
         * @param slot текстурный слот
        */
         virtual inline void bind(int32_t slot) const noexcept override
         {
            if (slot >= 0) {
                glActiveTexture(GL_TEXTURE0 + slot);
                glBindTexture(GL_TEXTURE_1D, _textureRendererHandle);
            }
         }

        /**
         * Метод возвращающий формат текселя.
         * 
         * @return формат текселя
        */
         virtual inline TexelFormat texelFormat() const noexcept override
         {
            return Tf;
         }

        /**
         * Метод возвращающий формат текселя.
         * 
         * @return формат текселя
        */
         inline constexpr TexelFormat cTexelFormat() const noexcept
         {
            return Tf;
         }
    };
}

