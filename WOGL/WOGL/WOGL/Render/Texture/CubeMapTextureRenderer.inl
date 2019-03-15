//
//  CubeMapTextureRenderer.inl
//  WOGL
//
//  Created by Асиф Мамедов on 07/03/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include "../../Data/Texture.hpp"

#include <stdexcept>

#include <GL/glew.h>

namespace WOGL
{
	class InitializeCubeMapTextureRenderer
	{
	protected:
		explicit InitializeCubeMapTextureRenderer(int32_t widthAndHeight, GLenum Tf) :
			_widthAndHeight{widthAndHeight}
		{
			glGenTextures(1, &_cubeMapTextureRendererHandle);

			if (!_cubeMapTextureRendererHandle) {
				throw runtime_error("Error create handle for cube map texture renderer");
			}

			glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMapTextureRendererHandle);
			glTexStorage2D(GL_TEXTURE_CUBE_MAP, 1, Tf, _widthAndHeight, _widthAndHeight);
		}

		InitializeCubeMapTextureRenderer(InitializeCubeMapTextureRenderer&& ictr) :
			_cubeMapTextureRendererHandle{0},
			_widthAndHeight{ictr._widthAndHeight}
		{
			swap(_cubeMapTextureRendererHandle, ictr._cubeMapTextureRendererHandle);
		}

		InitializeCubeMapTextureRenderer(const InitializeCubeMapTextureRenderer&) = delete;
		InitializeCubeMapTextureRenderer& operator=(const InitializeCubeMapTextureRenderer&) = delete;
		InitializeCubeMapTextureRenderer& operator=(InitializeCubeMapTextureRenderer&&) = delete;

		virtual ~InitializeCubeMapTextureRenderer()
		{
			if (_cubeMapTextureRendererHandle) {
				glDeleteTextures(1, &_cubeMapTextureRendererHandle);
			}
		}

		template<typename DataType, TexelType Tx>
		inline void _setTexture(GLenum target, const Texture<DataType, Tx>& texture)
		{
			auto dataType = InitializeTextureRenderer::_type<DataType>();
			glTexSubImage2D(target, 0, 0, 0, _widthAndHeight, _widthAndHeight, static_cast<GLenum>(Tx), dataType, &texture._data[0]);
		}

		uint32_t _cubeMapTextureRendererHandle;
		int32_t _widthAndHeight;
	};

	template<TexelFormat Tf>
	class CubeMapTextureRenderer :
		public InitializeCubeMapTextureRenderer
	{
	public:
		explicit CubeMapTextureRenderer(int32_t widthAndHeight) :
			InitializeCubeMapTextureRenderer(widthAndHeight, static_cast<GLenum>(Tf))
		{	
		}

		CubeMapTextureRenderer(CubeMapTextureRenderer&& cmtr) :
			InitializeCubeMapTextureRenderer(move(cmtr))
		{
		}

		CubeMapTextureRenderer(const CubeMapTextureRenderer&) = delete;
		CubeMapTextureRenderer& operator=(const CubeMapTextureRenderer&) = delete;
		CubeMapTextureRenderer& operator=(CubeMapTextureRenderer&&) = delete;

		inline void bind(int32_t slot) const noexcept
		{
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMapTextureRendererHandle);
		}

		inline void unbind() const noexcept
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}

		template<typename Texture>
		inline void setPositiveTextureX(const Texture& texture) 
		{
			_setTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X, texture);
		}

		template<typename Texture>
		inline void setPositiveTextureY(const Texture& texture) 
		{
			_setTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, texture);
		}

		template<typename Texture>
		inline void setPositiveTextureZ(const Texture& texture) 
		{
			_setTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, texture);
		}

		template<typename Texture>
		inline void setNegativeTextureX(const Texture& texture) 
		{
			_setTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, texture);
		}

		template<typename Texture>
		inline void setNegativeTextureY(const Texture& texture) 
		{
			_setTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, texture);
		}

		template<typename Texture>
		inline void setNegativeTextureZ(const Texture& texture) 
		{
			_setTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, texture);
		}

		int32_t widthAndHeight() const noexcept
		{
			return _widthAndHeight;
		}

		/**
		 * Метод необходимый для определения способа увелечения текстуры.
		 *
		 * @param mf способа минимизации текстуры
		*/
		static inline void magFilter(const TextureFilter mf) noexcept
        {
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(mf));
		}

		/**
		 * Метод необходимый для определения способа минимизации текстуры.
		 *
		 * @param mf способа увелечения текстуры
		*/
		void minFilter(const TextureFilter mf) noexcept
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(mf));
		}

		/**
		 * Метод отвечающий за определение способа оптекания текстуры по оси S(т. е. X).
		 *
		 * @param ws определяет способ оптекания по оси S(определение TextureWrapping находится выше)
		*/
		void textureWrappingS(const TextureWrapping ws) noexcept
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, static_cast<GLenum>(ws));
		}

		/**
		 * Метод отвечающий за определение способа оптекания текстуры по оси T(т. е. Y).
		 *
		 * @param wt определяет способ оптекания по оси T(определение TextureWrapping находится выше)
		*/
		void textureWrappingT(const TextureWrapping wt) noexcept
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, static_cast<GLenum>(wt));
		}

		/**
		 * Устанавливает режим сравнения текстур для текущих привязанных текстур глубины.
		*/
		void textureCompareMode(const TextureCompareMode cm) noexcept
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_MODE, static_cast<GLenum>(cm));
		}

		/**
		 * Устанавливает оператор сравнения, используемый, когда для TextureCompareMode установлено значение COMPARE_REF_TO_TEXTURE.
		*/
		void textureCompareFunc(const TextureCompareFunc cf) noexcept
		{
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_COMPARE_FUNC, static_cast<GLenum>(cf));
		}
	};
}
