//
//  Texture3D.inl
//  WOGL
//
//  Created by Асиф Мамедов on 29/03/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "Texture.hpp"

#include <vector>

#include <stdexcept>

using namespace std;

namespace WOGL
{
    template<typename DataType, TexelType Tx>
    class Texture3D
    {
        using Data = vector<DataType>;

    public:
        explicit Texture3D(size_t width, size_t height, size_t depth) :
            _width{width},
            _height{height},
            _depth{depth}
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

            _data.resize(_width * _height * _depth * _bpp);
        }

        template<typename T>
        explicit Texture3D(const T& data, size_t width, size_t height, size_t depth) :
            _width{width},
            _height{height},
            _depth{depth}
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
            
            set(data, width, height, depth);
        }

        Texture3D(const Texture3D& texture) :
            _data{texture._data},
            _width{texture._width},
            _height{texture._height},
            _depth{texture._depth},
            _bpp{texture._bpp}
        {
        }

        Texture3D(Texture3D&& texture) :
            _data{forward<Data>(texture._data)},
            _width{texture._width},
            _height{texture._height},
            _depth{texture._depth},
            _bpp{texture._bpp}
        {
        }

        template<typename T>
        inline void set(const T& data, size_t width, size_t height, size_t depth)
        {
            size_t dataSize = width * height * depth * _bpp;

            if (_width * _height * _depth * _bpp!= dataSize) {
                _data.resize(dataSize);
            }

            _width = width;
            _height = height;
            _depth = depth;

            copy(data.get(), data.get() + dataSize, begin(_data));
        }

        template<template<typename> typename Container>
        inline void set(const Container<DataType>& data, size_t width, size_t height, size_t depth)
        {
            if (width * height * depth * _bpp != data.size()) {
                throw invalid_argument("The 'data' size does not match the 'width', 'height' and 'depth', considering the number of active channels");
            }

            if (_width * _height * _depth * _bpp != data.size()) {
                _data.resize(data.size());
            }

            _width = width;
            _height = height;
            _depth = depth;

            copy(begin(data), end(data), begin(_data));
        }

        template<typename A, template<typename, typename> typename Container>
        inline void set(const Container<DataType, A>& data, size_t width, size_t height, size_t depth)
        {
            if (width * height * depth * _bpp != data.size()) {
                throw invalid_argument("The 'data' size does not match the 'width', 'height' and 'depth', considering the number of active channels");
            }

            if (_width * _height * _depth * _bpp != data.size()) {
                _data.resize(data.size());
            }

            _width = width;
            _height = height;
            _depth = depth;

            copy(begin(data), end(data), begin(_data));
        }

        inline void set(const Texture3D& texture) 
        {
            if (_data.size() != texture._data.size()) {
                _data.resize(texture._data.size());
            }

            _width = texture._width;
            _height = texture._height;
            _depth = texture._depth;

            copy(begin(texture._data), end(texture._data), begin(_data));
        }

        inline void subSet(const Texture3D& texture, size_t offX, size_t offY, size_t offZ)
        {
            if (texture._width + offx > _width || texture._height + offY > _height || texture._depth + offZ > _depth) {
                throw invalid_argument("The arguments passed do not fit in the original texture");
            }

            
        }

    private:
        Data _data;
        size_t _width;
        size_t _height;
        size_t _depth;
        size_t _bpp;
    };
}
