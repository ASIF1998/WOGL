//
//  GearMatrixView.cpp
//  WOGL
//
//  Created by Асиф Мамедов on 02/03/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "ArrayView.hpp"
#include <vector>
#include <stdexcept>

template<typename DataType>
class GearMatrixView;

namespace WOGL
{
    /**
     * Данный класс является невладеющей обёрткой над сущностями
     * ведущими себя как одномерный массив и позваляет работать с ними
     * как с 2-мерным массивом. 
     * 
     * Данный класс удобно использовать например при указании на какую то
     * часть текустуры.
     * 
     * @template DataType тип (например float)
    */
    template<typename DataType>
    class GearMatrixView
    {
        using ArraysView = vector<ArrayView<DataType>>;

    public:
        /**
         * Конструкор.
         *
         * @param data указатель исходные данные
         * @param dataWidth количесиво столбцов в data
         * @param dataHeight количесиво сторк в data
         * @param offsetLine сдвиг по строке
         * @param offsetCol сдви по столбцу
         * @param width ширина срезки
         * @param height высота срезки
         * @throw invalid_argument в случае координаты срезки будут больше размеров срезки или если размер срезки больше data
        */
        template<typename T>
        explicit GearMatrixView(const T& data, size_t dataWidth, size_t dataHeight, size_t offsetLine, size_t offsetCol, size_t width, size_t height) :
            _view(width)
        {
            if (offsetLine > height || offsetCol > width || height > dataHeight || width > dataWidth) {
                throw invalid_argument("Incorrect arguments");
            }

            const DataType* begin;

            if constexpr (is_same_v<T, shared_ptr<DataType>> || is_same_v<T, weak_ptr<DataType>>) {
                begin = data.get() + (offsetLine * dataHeight + offsetCol);
            } else {
                begin = &data[0] + (offsetLine * dataHeight + offsetCol);
            }

            for (size_t i{0}; i < height; i++, begin += dataWidth) {
                _view.push_back(ArrayView<DataType>(begin, width));
            }
        }

        /**
         * Конструкор.
         *
         * @param data указатель исходные данные
         * @param dataWidth количесиво столбцов в data
         * @param dataHeight количесиво сторк в data
         * @param offsetLine сдвиг по строке
         * @param offsetCol сдви по столбцу
         * @param width ширина срезки
         * @param height высота срезки
         * @throw invalid_argument в случае координаты срезки будут больше размеров срезки или если размер срезки больше data
        */
        template<typename T>
        explicit GearMatrixView(T& data, size_t dataWidth, size_t dataHeight, size_t offsetLine, size_t offsetCol, size_t width, size_t height) :
            _view(width)
        {
            if (offsetLine > height || offsetCol > width || height > dataHeight || width > dataWidth) {
                throw invalid_argument("Incorrect arguments");
            }

            DataType* begin;

            if constexpr (is_same_v<T, shared_ptr<DataType>> || is_same_v<T, weak_ptr<DataType>>) {
                begin = data.get() + (offsetLine * dataHeight + offsetCol);
            } else {
                begin = &data[0] + (offsetLine * dataHeight + offsetCol);
            }

            for (size_t i{0}; i < height; i++, begin += dataWidth) {
                _view.push_back(ArrayView<DataType>(begin, width));
            }
        }

        /**
         * Конструкор.
         *
         * @param data указатель исходные данные
         * @param dataWidth количесиво столбцов в data
         * @param dataHeight количесиво сторк в data
         * @param offsetLine сдвиг по строке
         * @param offsetCol сдви по столбцу
         * @param width ширина срезки
         * @param height высота срезки
         * @throw invalid_argument в случае координаты срезки будут больше размеров срезки или если размер срезки больше data
        */
        explicit GearMatrixView(const unique_ptr<DataType>& data, size_t dataWidth, size_t dataHeight, size_t offsetLine, size_t offsetCol, size_t width, size_t height) :
            _view(width)
        {
            if (offsetLine > height || offsetCol > width || height > dataHeight || width > dataWidth) {
                throw invalid_argument("Incorrect arguments");
            }

            DataType* begin = data.get() + (offsetLine * dataHeight + offsetCol);

            for (size_t i{0}; i < height; i++, begin += dataWidth) {
                _view.push_back(ArrayView<DataType>(begin, width));
            }
        }

         /**
         * Конструкор.
         *
         * @param data указатель исходные данные
         * @param dataWidth количесиво столбцов в data
         * @param dataHeight количесиво сторк в data
         * @param offsetLine сдвиг по строке
         * @param offsetCol сдви по столбцу
         * @param width ширина срезки
         * @param height высота срезки
         * @throw invalid_argument в случае координаты срезки будут больше размеров срезки или если размер срезки больше data
        */
        explicit GearMatrixView(unique_ptr<DataType>& data, size_t dataWidth, size_t dataHeight, size_t offsetLine, size_t offsetCol, size_t width, size_t height) :
            _view(width)
        {
            if (offsetLine > height || offsetCol > width || height > dataHeight || width > dataWidth) {
                throw invalid_argument("Incorrect arguments");
            }

            DataType* begin = data.get() + (offsetLine * dataHeight + offsetCol);

            for (size_t i{0}; i < height; i++, begin += dataWidth) {
                _view.push_back(ArrayView<DataType>(begin, width));
            }
        }

        /**
         * Конструкор.
         *
         * @param data указатель исходные данные
         * @param dataWidth количесиво столбцов в data
         * @param dataHeight количесиво сторк в data
         * @param offsetLine сдвиг по строке
         * @param offsetCol сдви по столбцу
         * @param width ширина срезки
         * @param height высота срезки
         * @throw invalid_argument в случае координаты срезки будут больше размеров срезки или если размер срезки больше data
        */
        explicit GearMatrixView(const DataType* data, size_t dataWidth, size_t dataHeight, size_t offsetLine, size_t offsetCol, size_t width, size_t height) :
            _view(width)
        {
            if (offsetLine > height || offsetCol > width || height > dataHeight || width > dataWidth || !data) {
                throw invalid_argument("Incorrect arguments");
            }

            DataType* begin = data + (offsetLine * dataHeight + offsetCol);

            for (size_t i{0}; i < height; i++, begin += dataWidth) {
                _view.push_back(ArrayView<DataType>(begin, width));
            }
        }

        /**
         * Конструкор.
         *
         * @param data указатель исходные данные
         * @param dataWidth количесиво столбцов в data
         * @param dataHeight количесиво сторк в data
         * @param offsetLine сдвиг по строке
         * @param offsetCol сдви по столбцу
         * @param width ширина срезки
         * @param height высота срезки
         * @throw invalid_argument в случае координаты срезки будут больше размеров срезки или если размер срезки больше data
        */
        explicit GearMatrixView(DataType* data, size_t dataWidth, size_t dataHeight, size_t offsetLine, size_t offsetCol, size_t width, size_t height) :
            _view(width)
        {
            if (offsetLine > height || offsetCol > width || height > dataHeight || width > dataWidth || !data) {
                throw invalid_argument("Incorrect arguments");
            }

            DataType* begin = data + (offsetLine * dataHeight + offsetCol);

            for (size_t i{0}; i < height; i++, begin += dataWidth) {
                _view.push_back(ArrayView<DataType>(begin, width));
            }
        }

        GearMatrixView(const GearMatrixView& gmv) :
            _view{gmv._view}
        {
        }

        GearMatrixView(GearMatrixView&& gmv) :
            _view{move(gmv._view)}
        {
        }

        GearMatrixView& operator=(const GearMatrixView&) = delete;
        GearMatrixView& operator=(GearMatrixView&&) = delete;

        /**
         * Метод, возвращающий количство строк.
         * Что бы узнать размер строки нужно вызвать метод size у i-ой строки.
         *
         * @return количство строк
        */
        size_t numLines() const noexcept
        {
            return _view.size();
        }

        /**
         * Методы с помощью котроых можно итерироваться.
        */

        DataType& at(size_t i, size_t j) 
        {
            return _view[i][j];
        }

        const DataType& at(size_t i, size_t j) const 
        {
            return _view[i][j];
        }

        auto operator[](size_t i) noexcept
        {
            return _view[i];
        }

        const auto operator[](size_t i) const noexcept
        {
            return _view[i];
        }

        auto begin() noexcept
        {
            return _view.begin();
        }

        const auto cbegin() const noexcept 
        {
            return _view.cbegin();
        }

        auto end() noexcept
        {
            return _view.end();
        }

        const auto cend() const noexcept 
        {
            return _view.cend();
        }

    private:
        ArraysView _view;
    };
}

