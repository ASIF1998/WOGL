
//
//  Texture1D.inl
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
    class Texture1D
    {
        using Data = vector<DataType>;
    public:
        explicit Texture1D(size_t width) 
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

            _data.resize(width);
        }

        template<typename T>
        explicit Texture1D(const T& data, size_t width)
        {
            set(data, width);
        }

        Texture1D(const Texture1D& texture) :
            _data{texture._data},
            _bpp{_bpp}
        {
        }

        Texture1D(Texture1D&& texture) :
            _data{forward<Data>(texture._data)},
            _bpp{texture._bpp}
        {
        }

        Texture1D& operator=(const Texture1D&) = delete;
        Texture1D& operator=(Texture1D&&) = delete;

        inline size_t width() const noexcept
        {
            return _data.size();
        }

        constexpr inline TexelType texelType() const noexcept
        {
            return Tx;
        }

        template<typename T>
        inline void set(const T& data, size_t width)
        {
            using UPtrData = unique_ptr<DataType[]>;
            using SPtrData = shared_ptr<DataType[]>;
            using WPtrData = weak_ptr<DataType[]>;
            using PtrData = DataType*;

            _data.resize(width);

            if (!data) {
                throw invalid_argument("A null pointer is passed to the 'set' method of the Texture1D class");
            }

            if constexpr(is_same_v<T, UPtrData> || is_same_v<T, SPtrData> || is_same_v<T, WPtrData>) {
                copy(data.get(), data.get() + _data.size(), begin(_data));
            } else {
                copy(data, data + _data.size(), begin(_data));
            }
        }

        inline void set(const Texture1D& texture)
        {
            _data.resize(texture._data.size());
            copy(begin(texture._data), end(texture._data), begin(_data));
        }

        template<template<typename> typename Container>
        inline void set(const Container<DataType>& data)
        {
            _data.resize(data.size());
            copy(begin(data), end(data), begin(_data));
        }

        template<typename A, template<typename, typename> typename Container>
        inline void set(const Container<DataType, A>& data)
        {
            _data.resize(data.size());
            copy(begin(data), end(data), begin(_data));
        }

        inline void subSet(const Texture1D& texture, size_t x)
        {
            if (_data.size() - x < texture._data.size()) {
                throw invalid_argument("The 'texture' argument passed is too large.");
            }

            copy(begin(texture._data), end(texture._data), begin(_data) + x);
        }

        template<typename T>
        inline void subSet(const T& data, size_t width, size_t x)
        {
            using UPtrData = unique_ptr<DataType[]>;
            using SPtrData = shared_ptr<DataType[]>;
            using WPtrData = weak_ptr<DataType[]>;
            using PtrData = DataType*;

            if (_data.size() - x < width) {
                throw invalid_argument("The 'data' argument passed is too large.");
            } else if (!data) {
                throw invalid_argument("A null pointer is passed to the 'subSet' method of the Texture1D class");
            }

            if constexpr(is_same_v<T, UPtrData> || is_same_v<T, SPtrData> || is_same_v<T, WPtrData>) {
                copy(data.get(), data.get() + width, begin(_data) + x);
            } else {
                copy(data, data + width, begin(_data) + x);
            }
        };

        template<template<typename> typename Container>
        inline void subSet(const Container<DataType>& data, size_t x)
        {
            if (_data.size() - x < data.size()) {
                throw invalid_argument("The 'data' argument passed is too large.");
            }

            copy(begin(data), end(data), begin(_data) + x);
        }

        template<typename A, template<typename, typename> typename Container>
        inline void subSet(const Container<DataType, A>& data, size_t x)
        {
            _data.resize(data.size());
            copy(begin(data), end(data), begin(_data) + x);
        }

        template<Canal canal>
        DataType at(size_t i) const 
        {
            size_t indx  = i * _bpp;

            if (canal == Canal::GREEN && _bpp > 1) {
                indx++;
            } else if (canal == Canal::BLUE && _bpp > 2) {
                indx += 2;
            } else if (canal == Canal::ALPHA && _bpp > 3) {
                indx += 3;
            }

            return _data.at(indx);
        }

        template<Canal canal>
        DataType& at(size_t i) 
        {
            size_t indx  = i * _bpp;

            if (canal == Canal::GREEN && _bpp > 1) {
                indx++;
            } else if (canal == Canal::BLUE && _bpp > 2) {
                indx += 2;
            } else if (canal == Canal::ALPHA && _bpp > 3) {
                indx += 3;
            }

            return _data.at(indx);
        }

        template<Canal canal>
        DataType atNoexcept(size_t i) const 
        {
            size_t indx  = i * _bpp;

            if (canal == Canal::GREEN && _bpp > 1) {
                indx++;
            } else if (canal == Canal::BLUE && _bpp > 2) {
                indx += 2;
            } else if (canal == Canal::ALPHA && _bpp > 3) {
                indx += 3;
            }

            return _data[indx];
        }

        template<Canal canal>
        DataType& atNoexcept(size_t i) 
        {
            size_t indx  = i * _bpp;

            if (canal == Canal::GREEN && _bpp > 1) {
                indx++;
            } else if (canal == Canal::BLUE && _bpp > 2) {
                indx += 2;
            } else if (canal == Canal::ALPHA && _bpp > 3) {
                indx += 3;
            }

            return _data[indx];
        }

    private:
        Data _data;
        size_t _bpp;
    };
}
