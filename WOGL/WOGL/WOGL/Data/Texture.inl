//
//  Texture.inl
//  WOGL
//
//  Created by Асиф Мамедов on 07/02/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include <vector>

#include <memory>

#include <stdexcept>

#include <string_view>

#include "../../../SOIL2/SOIL2.h"

using namespace std;
using namespace glm;

namespace WOGL
{
    /**
     * @template DataType тип каждого из каналов текстуры(например float)
     * @template Tx тип текселя
    */
    template<typename DataType, TexelType Tx>
    class Texture
    {
        friend class InitializeTextureRenderer;

    public:
        /**
         * Констуктор выделяющий память под текстуру размером width * height * bpp.
         * bpp зависит от того, кого чему равняется tx.
         *
         * @param width ширина текстуры
         * @param height высота текстуры
        */
        explicit Texture(size_t width, size_t height) :
            _width{width},
            _height{height}
        {
            if constexpr (Tx == TexelType::RED) {
                _bpp = 1;
            } else if constexpr (Tx == TexelType::RG) {
                _bpp = 2;
            } else if constexpr (Tx == TexelType::RGB) {
                _bpp = 3;
            } else if constexpr (Tx == TexelType::RGBA) {
                _bpp = 4;
            }

            _data.resize(_width * _height * _bpp);
        }

        /**
         * Конструктор выделяющий память под текстуру размером width * height * bpp и
         * заполняющий её данными из data.
         * 
         * @param data контейнер или указателя с данными (например типа std::vector<float>)
         * @param width ширина текстуры 
         * @param height высота текстуры
        */
        template<typename T>
        explicit Texture(const T& data, size_t width, size_t height)
        {
            if constexpr (Tx == TexelType::RED) {
                _bpp = 1;
            } else if constexpr (Tx == TexelType::RG) {
                _bpp = 2;
            } else if constexpr (Tx == TexelType::RGB) {
                _bpp = 3;
            } else if constexpr (Tx == TexelType::RGBA) {
                _bpp = 4;
            }

            set(data, width, height);
        }

        Texture(const Texture& texture) :
            _data{texture._data},
            _width{texture._width},
            _height{texture._height},
            _bpp{texture._bpp}
        {
        }

        Texture(Texture&& texture)  :
            _data{move(texture._data)},
            _width{texture._width},
            _height{texture._height},
            _bpp{texture._bpp}
        {
        }

        Texture& operator=(const Texture&) = delete;
        Texture& operator=(Texture&&) = delete;

        constexpr size_t width() const noexcept
        {
            return _width;
        }

        constexpr size_t height() const noexcept
        {
            return _height;
        }

        constexpr TexelType texelType() const noexcept
        {
            return Tx;
        }

        /**
         * Метод предназначенный для обновления данных текстуры.
         * 
         * @param data контейнер или указателя с данными (например типа std::vector<float>)
         * @param width ширина текстуры 
         * @param height высота текстуры
        */
        template<typename T>
        void set(const T& data, size_t width, size_t height)
        {
            using VecData = vector<DataType>;
            using UPtrData = unique_ptr<DataType[]>;
            using SPtrData = shared_ptr<DataType[]>;
            using WPtrData = weak_ptr<DataType[]>;

            if constexpr (is_same_v<T, VecData>) {
                if (data.size() != _data.size()) {
                    _data.resize(data.size());
                }

                copy(begin(data), end(data), begin(_data));
            } else if constexpr(is_same_v<T, UPtrData> || is_same_v<T, SPtrData> || is_same_v<T, WPtrData>) {
                if (width * height * _bpp != _data.size()) {
                    _data.reserve(width * height * _bpp);
                }

                copy(data.get(), data.get() + _data.size(), begin(_data));
            }

            _width = width;
            _height = height;
        }

        /**
         * Метод предназначенный для обновления отдельных частей текстуры.
         * 
         * param texture текстурв
         * @param x смещение по строке
         * @param y смещение по столбцу
         * @throw invalid_argument в случае если переданное смещение и размер текстуры не вписываеются в обновляемую текстуру
        */
        void subSet(const Texture& texture, size_t x, size_t y)
        {
            auto& subTex = texture._data;
            size_t subTexWidth = texture._width;
            size_t subTexHeight = texture._height;

            if ((x + subTexHeight) >= _height || (y + subTexWidth) >= _width) {
                throw invalid_argument("The arguments passed do not fit in the original texture");
            }

            for (DataType* ptr {&_data[(x * _height * _bpp) + (y * _bpp)]}; x < subTexHeight; x++, ptr = &_data[(x * _height * _bpp) + (y * _bpp)]) {
                copy(&subTex[x * subTexHeight * _bpp], &subTex[(x * subTexHeight * _bpp) + (y * _bpp)], ptr);
            }
        }

        template<Canal canal>
        DataType at(size_t i, size_t j) const
        {
            auto* ptr = &_data._data.at((i * _height * _bpp) + (j * _bpp));

            if (canal == Canal::GREEN && _bpp > 1) {
                ptr++;
            } else if (canal == Canal::BLUE && _bpp > 2) {
                ptr += 2;
            } else if (canal == Canal::ALPHA && _bpp > 3) {
                ptr += 3;
            }

            return *ptr;
        }

        template<Canal canal>
        DataType& at(size_t i, size_t j)
        {
            auto* ptr = &_data.at((i * _height * _bpp) + (j * _bpp));

            if (canal == Canal::GREEN && _bpp > 1) {
                ptr++;
            } else if (canal == Canal::BLUE && _bpp > 2) {
                ptr += 2;
            } else if (canal == Canal::ALPHA && _bpp > 3) {
                ptr += 3;
            }

            return *ptr;
        }

        template<Canal canal>
        DataType atNoexcept(size_t i, size_t j) const
        {
            auto* ptr = &_data[(i * _height * _bpp) + (j * _bpp)];

            if (canal == Canal::GREEN && _bpp > 1) {
                ptr++;
            } else if (canal == Canal::BLUE && _bpp > 2) {
                ptr += 2;
            } else if (canal == Canal::ALPHA && _bpp > 3) {
                ptr += 3;
            }

            return *ptr;
        }

        template<Canal canal>
        DataType& atNoexcept(size_t i, size_t j)
        {
            auto* ptr = &_data[(i * _height * _bpp) + (j * _bpp)];

            if (canal == Canal::GREEN && _bpp > 1) {
                ptr++;
            } else if (canal == Canal::BLUE && _bpp > 2) {
                ptr += 2;
            } else if (canal == Canal::ALPHA && _bpp > 3) {
                ptr += 3;
            }

            return *ptr;
        }

        /**
         * Функция предназначенная для загрузки изображения.
         *
         * @param path путь до изображения
         * @throw invalid_argument в случае если не удалось загрузить изображение
        */
        static auto loadTexture(const string_view path)
        {
            int32_t width, height;
            uint8_t* imageData;

            if constexpr(Tx == TexelType::RED) {
                imageData = SOIL_load_image(path.data(), &width, &height, nullptr, SOIL_LOAD_L);
            } else if constexpr(Tx == TexelType::RG) {
                imageData = SOIL_load_image(path.data(), &width, &height, nullptr, SOIL_LOAD_LA);
            } else if constexpr(Tx == TexelType::RGB) {
                imageData = SOIL_load_image(path.data(), &width, &height, nullptr, SOIL_LOAD_RGB);
            } else if constexpr(Tx == TexelType::RGBA) {
                imageData = SOIL_load_image(path.data(), &width, &height, nullptr, SOIL_LOAD_RGBA);
            }

            if (!imageData) {
                throw invalid_argument("Failed to load the image at the specified path");
            }

            Texture<DataType, Tx> tex (static_cast<size_t>(width), static_cast<size_t>(height));
            
            if constexpr (is_same_v<DataType, float> || is_same_v<DataType, double>) {
                for (size_t i{0}; i < tex._data.size(); i++) {
                    tex._data[i] = static_cast<DataType>(imageData[i]) / 255.0;
                }
            } else {
                copy(tex._data.begin(), tex._data.end(), imageData);
            }

            delete [] imageData;
            
            return tex;
        }

    private:
        vector<DataType> _data;
        size_t _width;
        size_t _height;
        size_t _bpp;
    };
}
