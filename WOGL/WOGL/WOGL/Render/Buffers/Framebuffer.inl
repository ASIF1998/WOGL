//
//  Framebuffer.inl
//  WOGL
//
//  Created by Асиф Мамедов on 03/03/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include <vector>

#include "../TextureRenderer.hpp"

#include <memory>

using namespace std;

namespace WOGL
{
	enum class WritePixels
	{
		NoWrite,
		Texture,
		RenderBuffer
	};

	class IFramebuffer
	{
	public:
		virtual inline void bind() const noexcept = 0;
		virtual inline void unbind() const noexcept = 0;
		virtual void readPixels(float*, int32_t, int32_t, int32_t, int32_t) const noexcept = 0;
		virtual vector<float> readPixels(int32_t, int32_t, int32_t, int32_t) const noexcept = 0;
	};

	template<TexelFormat Tf>
	class BaseFramebuffer :
		public IFramebuffer
	{
		using TextureRenderers = vector<TextureRenderer<Tf>>;

	public: 
		BaseFramebuffer(int32_t width, int32_t height, int32_t numColorBuffers)
		{
			glGenFramebuffers(1, &_framebufferHandle);

			if (!_framebufferHandle) {
				throw runtime_error("Error create handle for framebuffer");
			}

			for (int32_t i{0}; i < numColorBuffers; i++) {
				_colorBuffer.push_back(TextureRenderer<Tf>{width, height});
			}

			_colorBuffer.shrink_to_fit();

			glBindFramebuffer(GL_FRAMEBUFFER, _framebufferHandle);

			for (int32_t i{0}, size = static_cast<int32_t>(_colorBuffer.size()); i < size; i++) {
				_colorBuffer[i].bind();
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _colorBuffer[i]._textureRendererHandle, 0);
				_colorBuffer[i].unbind();
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		BaseFramebuffer(BaseFramebuffer&& bf) :
			_colorBuffer{move(bf._colorBuffer)},
			_framebufferHandle{0}
		{
			swap(_framebufferHandle, bf._framebufferHandle);
		}

		BaseFramebuffer(const BaseFramebuffer&) = delete;
		BaseFramebuffer& operator=(const BaseFramebuffer&) = delete;
		BaseFramebuffer& operator=(BaseFramebuffer&&) = delete;

		virtual ~BaseFramebuffer()
		{
			if (_framebufferHandle) {
				glDeleteFramebuffers(1, &_framebufferHandle);
			}
		}

		/**
		 * Метод делающий кадровый буффер текущим.
		*/
		virtual inline void bind() const noexcept override
		{
			glBindFramebuffer(GL_FRAMEBUFFER, BaseFramebuffer<Tf>::_framebufferHandle);
		}

		/**
		 * Метод делающий кадровый буффер не текущим. По сути он деалет текущую цель привязки нулевой.
		*/
		virtual inline void unbind() const noexcept override
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);	
		}

		/**
		 * Метод необходимый для считывания значений пикселя из кадрового буфера.
		 *
		 * @param data массив куда будут записаны значения пикселей
		 * (т. к. на каждый пикснль выделяется по три канала то размер массива должен быть width * height * 3)
		 * @param width ширна
		 * @param height высота
		 * @param x координата по оси X (необходима для выявления того, откуда нужно начинать считывать по оси X)
		 * @param y координата по оси Y (необходима для выявления того, откуда нужно начинать считывать по оси Y)
		*/
		virtual void readPixels(float* data, int32_t width, int32_t height, int32_t x, int32_t y) const noexcept override
		{
			glReadPixels(x, y, width, height, GL_RGB, GL_FLOAT, data);
		}

		/**
		 * Метод необходимый для считывания значений пикселя и фрейм буфера.
		 *
		 * @param width ширна
		 * @param height высота
		 * @param x координата по оси X (необходима для выявления того, откуда нужно начинать считывать по оси X)
		 * @param y координата по оси Y (необходима для выявления того, откуда нужно начинать считывать по оси Y)
		 * @return вектор пикслелей (т. к. на каждый пикснль выделяется по три канала то размер возвращаемого вектора
		 * будет равен width * height * 3)
		*/
		virtual vector<float> readPixels(int32_t width, int32_t height, int32_t x, int32_t y) const noexcept override
		{
			vector<float> data(width * height * 3);
			glReadPixels(x, y, width, height, GL_RGB, GL_FLOAT, &data[0]);
			return data;
		}

		TextureRenderers& colorBuffer(size_t i) 
		{
			return _colorBuffer.at(i);
		}

		const TextureRenderers& colorBuffer(size_t i) const 
		{
			return _colorBuffer.at(i);
		}

		size_t numberOfColorBuffers() const noexcept
		{
			return _colorBuffer.size();
		}

	protected:
		TextureRenderers _colorBuffer;
		uint32_t _framebufferHandle;
	};

	template<TexelFormat Tf, WritePixels DepthBuffer, WritePixels StensilBuffer>
	class Framebuffer;

	template<TexelFormat Tf>
	class Framebuffer<Tf, WritePixels::NoWrite, WritePixels::NoWrite> :
		public BaseFramebuffer<Tf>
	{
	public:
		/**
		 * Конструктор.
		 *
		 * @param width ширина 
		 * @param height высота 
		 * @param numColorBuffers количество буфферов цвета 
		 * @throw runtime_error в случае если не удалось создать дескриптор для кадрового буффера
		*/
		Framebuffer(int32_t width, int32_t height, int32_t numColorBuffers = 1) :
			BaseFramebuffer<Tf>(width, height, numColorBuffers)
		{
		}

		/**
		 * Конструктор.
		 * Ширина и воста будут браться из объекта window.
		 *
		 * @param window окно 
		 * @param numColorBuffers количество буфферов цвета 
		 * @throw runtime_error в случае если не удалось создать дескриптор для кадрового буффера
		*/
		template<typename Window>
		Framebuffer(const Window& window, int32_t numColorBuffers = 1) :
			BaseFramebuffer<Tf>(get<0>(window.size()), get<1>(window.size()), numColorBuffers)
		{
		}
	 };
}
