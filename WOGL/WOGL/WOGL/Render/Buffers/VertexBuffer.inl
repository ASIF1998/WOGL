//
//  VertexBuffer.inl
//  WOGL
//
//  Created by Асиф Мамедов on 05/02/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include <GL/glew.h>

#include <stdexcept>

#include <array>
#include <vector>
#include <initializer_list>

#include <memory>

using namespace std;

namespace WOGL
{
    /**
     * @template NumComponent количество компонентов в элементе(напрример в vec4 4-е компонента)
    */
    template<int32_t NumComponent>
    class VertexBuffer
    {
        friend class VertexArray;

    public:
        /**
         * Конструктор который создаёт дескриптор вершинного буффера и помещает туда данные.
         *
         * @param data массив данных
         * @template Size размер массива
        */
        template<size_t Size>
        explicit VertexBuffer(const float(&data)[Size]) :
            _size{static_cast<int32_t>(Size)}
        {
            _createHandle();

            bind();
            glBufferData(GL_ARRAY_BUFFER, Size * sizeof(float), data, GL_STATIC_DRAW);
            unbind();
        }

        /**
         * Конструктор который создаёт дескриптор вершинного буффера и помещает туда данные.
         *
         * @param data объект типа std::array
         * @template Size размер массива внутри объекта std::array
        */
        template<size_t Size>
        explicit VertexBuffer(const array<float, Size>& data) :
            _size{static_cast<int32_t>(Size)}
        {
            _createHandle();

            bind();
            glBufferData(GL_ARRAY_BUFFER, Size * sizeof(float), &data[0], GL_STATIC_DRAW);
            unbind();
        }

        /**
         * Конструктор который создаёт дескриптор вершинного буффера и помещает туда данные.
         *
         * @param data объект типа std::initializer_list
        */
        explicit VertexBuffer(const initializer_list<float>& data) :
            _size{static_cast<int32_t>(data.size())}
        {
            _createHandle();

            bind();
            glBufferData(GL_ARRAY_BUFFER, _size * sizeof(float), begin(data), GL_STATIC_DRAW);
            unbind();
        }

        /**
         * Конструктор который создаёт дескриптор вершинного буффера и помещает туда данные.
         * Тип контейнера или указателя (T) должен содержать данные типа float !!!
         *
         * @param data контейнер или указателя с данными (например типа std::vector<float>)
        */
        template<typename T>
        explicit VertexBuffer(const T& data) :
            _size{static_cast<int32_t>(data.size())}
        {
            _createHandle();
            
            bind();
            glBufferData(GL_ARRAY_BUFFER, _size * sizeof(float), &data[0], GL_STATIC_DRAW);
            unbind();
        }

        /**
         * Конструктор который создаёт дескриптор вершинного буффера и помещает туда данные.
         * Тип контейнера или указателя (T) должен содержать данные типа float !!!
         *
         * @param data контейнер или указателя с данными (например типа std::unique_ptr<float[]>)
        */
        template<typename T>
        explicit VertexBuffer(const T& data, size_t size) :
            _size{static_cast<int32_t>(size)}
        {
            _createHandle();

            if (data && size) {
                bind();
                glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data.get(), GL_STATIC_DRAW);
                unbind();
            }
        }

        VertexBuffer(VertexBuffer&& buffer) :
            _vertexBufferHandle{0},
            _size{buffer._size}
        {
            swap(_vertexBufferHandle, buffer._vertexBufferHandle);
        }

        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;
        VertexBuffer& operator=(VertexBuffer&&) = delete;

        ~VertexBuffer()
        {
            if (_vertexBufferHandle) {
                glDeleteBuffers(1, &_vertexBufferHandle);
            }
        }

        /**
         * Метод делающий буффер текущим.
        */
        inline void bind() const noexcept
        {
            glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferHandle);
        }

        /**
         * Метод делающий буффер не текущим. По сути он деалет текущую цель привязки нулевой.
        */
        static inline void unbind() noexcept
        {
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        /**
         * Данный метод необходим для указания того, когда стоит осуществлять
         * выборку нового элемента из вершинного атрибута.
         * 
         * Данный метод делает вершинный атрибут инстансированным массивом.
         * 
         * @param indx индекс атрибута
         * @param divisor разделитель атрибута
        */
        void attributeDivisor(int32_t indx, uint32_t divisor) const noexcept
        {
            bind();
            glVertexAttribDivisor(indx, divisor);
        }

        constexpr int32_t size() const noexcept
        {
            return _size;
        }

        constexpr int32_t numbersComponent() const noexcept
        {
            return NumComponent;
        }

    private:
        inline void _createHandle() 
        {
            glGenBuffers(1, &_vertexBufferHandle);

            if (!_vertexBufferHandle) {
                throw runtime_error("Error create vertex buffer handle");
            }
        }

        uint32_t _vertexBufferHandle;
        int32_t _size;
    };
}
