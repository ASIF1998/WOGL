//
//  SkyBox.inl
//  WOGL
//
//  Created by Асиф Мамедов on 07/03/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "CubeMapTextureRenderer.hpp"

#include "../Buffers/VertexBuffer.hpp"
#include "../Buffers/IndexBuffer.hpp"
#include "../VertexArray.hpp"

#include <array>

namespace WOGL
{
	template<TexelFormat Tf>
	class SkyBox :
        public CubeMapTextureRenderer<Tf>
	{
	public:
		explicit SkyBox(int32_t widthAndHeight, int32_t positionBufferIndx) :
            CubeMapTextureRenderer<Tf>{widthAndHeight},
			_positionsBuffer {
				1.0f, 1.0f, -1.0f,
				-1.0f, 1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,

				1.0f, 1.0f, 1.0f,
				-1.0f, 1.0f, 1.0f,
				1.0f, -1.0f, 1.0f,
				-1.0f, -1.0f, 1.0f
			 },
			_ebo {
				1, 0, 3,
				0, 3, 2,

				0, 6, 2,
				0, 6, 4,

				4, 7, 6,
				4, 7, 5,

				1, 3, 7,
				1, 7, 5,

				7, 3, 2,
				7, 2, 6,

				0, 4, 5,
				0, 1, 5
			}
		{
			_vao.add(_positionsBuffer, positionBufferIndx);
		}

        inline void bindVertexArray() const noexcept
        {
            _vao.bind();
        }

        inline void bindIndexBuffer() const noexcept
        {
            _ebo.bind();
        }

        inline void unbindVertexArray() const noexcept
        {
            glBindVertexArray(0);
        }

        inline void unbindIndexBuffer() const noexcept
        {
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

	private:
		VertexBuffer<3> _positionsBuffer;
		VertexArray _vao;
		IndexBuffer _ebo;
	};
}
