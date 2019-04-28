//
//  VertexArray.inl
//  WOGL
//
//  Created by Асиф Мамедов on 06/02/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

namespace WOGL
{
    class VertexArray
    {
    public:
        VertexArray() 
        {
            glGenVertexArrays(1, &_vertexArrayHandle);

            if (!_vertexArrayHandle) {
                throw runtime_error("Error create vertex array handle");
            }
        }

        VertexArray(VertexArray&& vertexArray) :
            _vertexArrayHandle{0}
        {
            swap(_vertexArrayHandle, vertexArray._vertexArrayHandle);
        }

        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;
        VertexArray& operator=(VertexArray&&) = delete;

        virtual ~VertexArray()
        {
            if (_vertexArrayHandle) {
                glDeleteVertexArrays(1, &_vertexArrayHandle);
            }
        }

        /**
         * Метод возвращающий дескриптор VAO.
         * Данный метод не сделан константным так как пользователь сможет повлиять на VAO с помощью функций OpenGL.
         *
         * @return дескриптор VAO
         */
        uint32_t id() noexcept
        {
            return _vertexArrayHandle;
        }

        inline void bind() const noexcept
        {
            glBindVertexArray(_vertexArrayHandle);
        }

        inline static void unbind() noexcept
        {
            glBindVertexArray(0);
        }

        /**
         * Метод позволяющий добавлять вершинный буфер.
         * 
         * @param vertexBuffer вершинный буффер
         * @param vboIndx индекс атрибута 
         * @template NumComponent количество компонентов в атрибуте (напрример в vec4 4-е компонента)
        */
        template<int32_t NumComponent, template<int32_t> typename VertexBufferType>
        void add(const VertexBufferType<NumComponent>& vertexBuffer, int32_t vboIndx) const noexcept
        {
            bind();
            vertexBuffer.bind();
            glEnableVertexAttribArray(vboIndx);
            glVertexAttribPointer(vboIndx, NumComponent, GL_FLOAT, GL_FALSE, 0, nullptr);

            unbind();
        }

    private:
        uint32_t _vertexArrayHandle;
    };

}
