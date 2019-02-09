//
//  VertexArray.inl
//  WOGL
//
//  Created by Асиф Мамедов on 06/02/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

namespace WOGL
{
    inline namespace Render
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

            ~VertexArray()
            {
                if (_vertexArrayHandle) {
                    glDeleteVertexArrays(1, &_vertexArrayHandle);
                }
            }

            inline void bind() const noexcept
            {
                glBindVertexArray(_vertexArrayHandle);
            }

            inline static void unbind() noexcept
            {
                glBindVertexArray(0);
            }

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
}
