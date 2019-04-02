//
//  Framebuffer.inl
//  WOGL
//
//  Created by Асиф Мамедов on 03/03/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#include <vector>

#include "../Texture/TextureRenderer.hpp"

#include <memory>

namespace WOGL
{
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

		friend class Context;
	public: 
		/**
		 * Конструктор.
		 *
		 * @param width ширина 
		 * @param height высота 
		 * @param numColorBuffers количество буфферов цвета 
		 * @throw runtime_error в случае если не удалось создать дескриптор для кадрового буффера
		*/
		explicit BaseFramebuffer(int32_t width, int32_t height, int32_t numColorBuffers)
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
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _colorBuffer[i]._textureRendererHandle, 0);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
		explicit BaseFramebuffer(const Window& window, int32_t numColorBuffers) 
		{
			glGenFramebuffers(1, &_framebufferHandle);

			if (!_framebufferHandle) {
				throw runtime_error("Error create handle for framebuffer");
			}

			auto[windowWidth, windowHeight] = window.size();

			for (int32_t i{0}; i < numColorBuffers; i++) {
				_colorBuffer.push_back(TextureRenderer<Tf>{windowWidth, windowHeight});
			}

			_colorBuffer.shrink_to_fit();

			glBindFramebuffer(GL_FRAMEBUFFER, _framebufferHandle);

			for (int32_t i{0}, size = static_cast<int32_t>(_colorBuffer.size()); i < size; i++) {
				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, _colorBuffer[i]._textureRendererHandle, 0);
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
		 * Методы необходимые для считывания значений пикселя из кадрового буфера.
		 *
		 * @param data массив куда будут записаны значения пикселей
		 * (т. к. на каждый пиксель выделяется по три канала то размер массива должен быть width * height * 3)
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
		 * Методы необходимые для считывания значений пикселя из кадрового буфера.
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

		TextureRenderer<Tf>& colorBuffer(size_t i)
		{
			return _colorBuffer.at(i);
		}

		const TextureRenderer<Tf>& colorBuffer(size_t i) const 
		{
			return _colorBuffer.at(i);
		}

		size_t numberOfColorBuffers() const noexcept
		{
			return _colorBuffer.size();
		}

	protected:
		GLenum _type() const noexcept
		{
			return GL_INT;
		}

		TextureRenderers _colorBuffer;
		uint32_t _framebufferHandle;
	};

	template<TexelFormat Tf, WritePixels Depth, WritePixels Stensil>
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
		explicit Framebuffer(int32_t width, int32_t height, int32_t numColorBuffers = 1) :
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
		explicit Framebuffer(const Window& window, int32_t numColorBuffers = 1) :
			BaseFramebuffer<Tf>(window, numColorBuffers)
		{
		}

		Framebuffer(Framebuffer&& framebuffer) :
			BaseFramebuffer<Tf>(move(framebuffer))
		{
		}

		Framebuffer(const Framebuffer&) = delete;
		Framebuffer& operator=(const Framebuffer&) = delete;
		Framebuffer& operator=(Framebuffer&&) = delete;
	 };

	 template<TexelFormat Tf>
	 class Framebuffer<Tf, WritePixels::Texture, WritePixels::NoWrite> :
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
		 * @throw runtime_error в случае если не удалось создать дескриптор для текстуры в которой будут хранится значения глубины фрагмента
		*/
		explicit Framebuffer(int32_t width, int32_t height, int32_t numColorBuffers = 1) :
			BaseFramebuffer<Tf>(width, height, numColorBuffers)
		{
			glGenTextures(1, &_depthTextureHandle);

			if (!_depthTextureHandle) {
				throw runtime_error("Error create descriptor for depth texture");
			}

			glBindTexture(GL_TEXTURE_2D, _depthTextureHandle);
			glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, width, height);

			glBindFramebuffer(GL_FRAMEBUFFER, BaseFramebuffer<Tf>::_framebufferHandle);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTextureHandle, 0);

			glBindTexture(GL_TEXTURE_2D, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		/**
		 * Конструктор.
		 * Ширина и воста будут браться из объекта window.
		 *
		 * @param window окно 
		 * @param numColorBuffers количество буфферов цвета 
		 * @throw runtime_error в случае если не удалось создать дескриптор для кадрового буффера
		 * @throw runtime_error в случае если не удалось создать дескриптор для текстуры в которой будут хранится значения глубины фрагмента
		*/
		template<typename Window>
		explicit Framebuffer(const Window& window, int32_t numColorBuffers = 1) :
			BaseFramebuffer<Tf>(window, numColorBuffers)
		{
			glGenTextures(1, &_depthTextureHandle);

			if (!_depthTextureHandle) {
				throw runtime_error("Error create descriptor for depth texture");
			}

			auto[windowWidth, windowHeight] = window.size();

			glBindFramebuffer(GL_FRAMEBUFFER, BaseFramebuffer<Tf>::_framebufferHandle);
			glBindTexture(GL_TEXTURE_2D, _depthTextureHandle);

			glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, windowWidth, windowHeight);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTextureHandle, 0);

			glBindTexture(GL_TEXTURE_2D, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		Framebuffer(Framebuffer&& framebuffer) :
			BaseFramebuffer<Tf>(move(framebuffer)),
			_depthTextureHandle{0}
		{
			swap(_depthTextureHandle, framebuffer._depthTextureHandle);
		}

		Framebuffer(const Framebuffer&) = delete;
		Framebuffer& operator=(const Framebuffer&) = delete;
		Framebuffer& operator=(Framebuffer&&) = delete;

		virtual ~Framebuffer()
		{
			if (_depthTextureHandle) {
				glDeleteTextures(1, &_depthTextureHandle);
			}
		}

		void bindDepthTexture(int32_t slot) const noexcept
		{
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, _depthTextureHandle);
		}

		void unbindDepthTexture(int32_t slot) const noexcept
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

	 private:
	 	uint32_t _depthTextureHandle;
	 };

	template<TexelFormat Tf>
	class Framebuffer<Tf, WritePixels::NoWrite, WritePixels::RenderBuffer> :
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
		 * @throw runtime_error в случае если не удалось создать дескриптор для renderbuffer'а в которой будут хранится значения трафарета
		*/
		explicit Framebuffer(int32_t width, int32_t height, int32_t numColorBuffers = 1) :
			BaseFramebuffer<Tf>(width, height, numColorBuffers)
		{
			glGenRenderbuffers(1, &_stencilBufferHandle);

			if (!_stencilBufferHandle) {
				throw runtime_error("Error create descriptor for depth buffer");
			}

			glBindFramebuffer(GL_FRAMEBUFFER, BaseFramebuffer<Tf>::_framebufferHandle);
			glBindRenderbuffer(GL_RENDERBUFFER, _stencilBufferHandle);

			glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _stencilBufferHandle);

			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		/**
		 * Конструктор.
		 * Ширина и воста будут браться из объекта window.
		 *
		 * @param window окно 
		 * @param numColorBuffers количество буфферов цвета 
		 * @throw runtime_error в случае если не удалось создать дескриптор для кадрового буффера
		 * @throw runtime_error в случае если не удалось создать дескриптор для renderbuffer'а в которой будут хранится значения трафарета
		*/
		template<typename Window>
		explicit Framebuffer(const Window& window, int32_t numColorBuffers = 1) :
			BaseFramebuffer<Tf>(window, numColorBuffers)
		{
			glGenRenderbuffers(1, &_stencilBufferHandle);

			if (!_stencilBufferHandle) {
				throw runtime_error("Error create descriptor for depth buffer");
			}

			auto[windowWidth, windowHeight] = window.size();

			glBindFramebuffer(GL_FRAMEBUFFER, BaseFramebuffer<Tf>::_framebufferHandle);
			glBindRenderbuffer(GL_RENDERBUFFER, _stencilBufferHandle);

			glRenderbufferStorage(GL_RENDERBUFFER, GL_STENCIL_INDEX8, windowWidth, windowHeight);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _stencilBufferHandle);

			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		Framebuffer(Framebuffer&& framebuffer) :
			BaseFramebuffer<Tf>(move(framebuffer)),
			_stencilBufferHandle{0}
		{
			swap(_stencilBufferHandle, framebuffer._depthBufferHandle);
		}

		Framebuffer(const Framebuffer&) = delete;
		Framebuffer& operator=(const Framebuffer&) = delete;
		Framebuffer& operator=(Framebuffer&&) = delete;

		virtual ~Framebuffer()
		{
			if (_stencilBufferHandle) {
				glDeleteRenderbuffers(1, &_stencilBufferHandle);
			}
		}

		inline void bindStencilBuffer() const noexcept
		{
			glBindRenderbuffer(GL_RENDERBUFFER, _stencilBufferHandle);
		}

		inline void unbindStencilBuffer() const noexcept
		{
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}

	private:
		uint32_t _stencilBufferHandle;
	};

	/**
	 * Данная реализация класса Framebuffer хранит значения глубины и трафарета в текстуре,
	 * где для каждого фрагмета выделена 4-х байтная ячейка. Значения глубины занимает 
	 * 24 бита, а значение трафарета 8 бит.
	*/
	template<TexelFormat Tf>
	class Framebuffer<Tf, WritePixels::Texture, WritePixels::Texture> :
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
		 * @throw runtime_error в случае если не удалось создать дескриптор для текстуры в которой будут хранится значения глубины и трафарета 
		*/
		explicit Framebuffer(int32_t width, int32_t height, int32_t numColorBuffers = 1) :
			BaseFramebuffer<Tf>(width, height, numColorBuffers) 
		{
			glGenTextures(1, &_depthAndStencilTextureHandle);

			if (!_depthAndStencilTextureHandle) {
				throw runtime_error("Error create descriptor for depth and stencil texture");
			}

			glBindFramebuffer(GL_FRAMEBUFFER, BaseFramebuffer<Tf>::_framebufferHandle);
			glBindTexture(GL_TEXTURE_2D, _depthAndStencilTextureHandle);

			glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, width, height);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _depthAndStencilTextureHandle, 0);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		/**
		 * Конструктор.
		 * Ширина и воста будут браться из объекта window.
		 *
		 * @param window окно 
		 * @param numColorBuffers количество буфферов цвета 
		 * @throw runtime_error в случае если не удалось создать дескриптор для кадрового буффера
		 * @throw runtime_error в случае если не удалось создать дескриптор для текстуры в которой будут хранится значения глубины и трафарета 
		*/
		template<typename Window>
		explicit Framebuffer(const Window& window, int32_t numColorBuffers = 1) :
			BaseFramebuffer<Tf>(window, numColorBuffers)
		{
			glGenTextures(1, &_depthAndStencilTextureHandle);

			if (!_depthAndStencilTextureHandle) {
				throw runtime_error("Error create descriptor for depth and stencil texture");
			}

			auto[windowWidth, windowHeight] = window.size();

			glBindFramebuffer(GL_FRAMEBUFFER, BaseFramebuffer<Tf>::_framebufferHandle);
			glBindTexture(GL_TEXTURE_2D, _depthAndStencilTextureHandle);

			glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, windowWidth, windowHeight);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, _depthAndStencilTextureHandle, 0);

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		Framebuffer(Framebuffer&& framebuffer) :
			BaseFramebuffer<Tf>(move(framebuffer)),
			_depthAndStencilTextureHandle{0}
		{
			swap(_depthAndStencilTextureHandle, framebuffer._depthAndStencilTextureHandle);
		}

		Framebuffer(const Framebuffer&) = delete;
		Framebuffer& operator=(const Framebuffer&) = delete;
		Framebuffer& operator=(Framebuffer&&) = delete;

		virtual ~Framebuffer()
		{
			if (_depthAndStencilTextureHandle) {
				glDeleteTextures(1, &_depthAndStencilTextureHandle);
			}
		}

		inline void bindDepthAndStencilTexture() const noexcept
		{
			glBindTexture(GL_TEXTURE_2D, _depthAndStencilTextureHandle);
		}

		inline void unbindDepthAndStencilTexture() const noexcept
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

	private:
		uint32_t _depthAndStencilTextureHandle;
	};

	template<TexelFormat Tf>
	class Framebuffer<Tf, WritePixels::RenderBuffer, WritePixels::NoWrite> :
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
		 * @throw runtime_error в случае если не удалось создать дескриптор для renderbuffer'а в которой будут хранится значения глубины
		*/
		explicit Framebuffer(int32_t width, int32_t height, int32_t numColorBuffers = 1) :
			BaseFramebuffer<Tf>(width, height, numColorBuffers) 
		{
			glGenRenderbuffers(1, &_depthBufferHandle);

			if (!_depthBufferHandle) {
				throw runtime_error("Error create descriptor for depth buffer");
			}

			glBindFramebuffer(GL_FRAMEBUFFER, BaseFramebuffer<Tf>::_framebufferHandle);
			glBindRenderbuffer(GL_RENDERBUFFER, _depthBufferHandle);

			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBufferHandle);

			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		/**
		 * Конструктор.
		 * Ширина и воста будут браться из объекта window.
		 *
		 * @param window окно 
		 * @param numColorBuffers количество буфферов цвета 
		 * @throw runtime_error в случае если не удалось создать дескриптор для кадрового буффера
		 * @throw runtime_error в случае если не удалось создать дескриптор для renderbuffer'а в которой будут хранится значения глубины
		*/
		template<typename Window>
		explicit Framebuffer(const Window& window, int32_t numColorBuffers = 1) :
			BaseFramebuffer<Tf>(window, numColorBuffers)
		{
			glGenRenderbuffers(1, &_depthBufferHandle);

			if (!_depthBufferHandle) {
				throw runtime_error("Error create descriptor for depth buffer");
			}

			auto[windowWidth, windowHeight] = window.size();

			glBindFramebuffer(GL_FRAMEBUFFER, BaseFramebuffer<Tf>::_framebufferHandle);
			glBindRenderbuffer(GL_RENDERBUFFER, _depthBufferHandle);

			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth, windowHeight);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthBufferHandle);

			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		Framebuffer(Framebuffer&& framebuffer) :
			BaseFramebuffer<Tf>(move(framebuffer)),
			_depthBufferHandle{0}
		{
			swap(_depthBufferHandle, framebuffer._depthBufferHandle);
		}

		Framebuffer(const Framebuffer&) = delete;
		Framebuffer& operator=(const Framebuffer&) = delete;
		Framebuffer& operator=(Framebuffer&&) = delete;

		virtual ~Framebuffer()
		{
			if (_depthBufferHandle) {
				glDeleteRenderbuffers(1, &_depthBufferHandle);
			}
		}

		inline void bindDepthBuffer() const noexcept
		{
			glBindRenderbuffer(GL_RENDERBUFFER, _depthBufferHandle);
		}

		inline void unbindDepthBuffer() const noexcept
		{
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
		}

	private:
		uint32_t _depthBufferHandle;
	};

	/**
	 * Данная реализация класса Framebuffer хранит значения глубины и трафарета в renderbuffer'е,
	 * где для каждого фрагмета выделена 4-х байтная ячейка. Значения глубины занимает 
	 * 24 бита, а значение трафарета 8 бит.
	*/
	template<TexelFormat Tf>
	class Framebuffer<Tf, WritePixels::RenderBuffer, WritePixels::RenderBuffer> :
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
		 * @throw runtime_error в случае если не удалось создать дескриптор для renderbuffer'а в которой будут хранится значения глубины и трафарета
		*/
		explicit Framebuffer(int32_t width, int32_t height, int32_t numColorBuffers = 1) :
			BaseFramebuffer<Tf>(width, height, numColorBuffers)
		{
			glGenRenderbuffers(1, &_depthAndStencilBufferHandle);

			if (!_depthAndStencilBufferHandle) {
				throw runtime_error("Error create descriptor for depth buffer");
			}

			glBindFramebuffer(GL_FRAMEBUFFER, BaseFramebuffer<Tf>::_framebufferHandle);
			glBindRenderbuffer(GL_RENDERBUFFER, _depthAndStencilBufferHandle);

			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthAndStencilBufferHandle);

			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		/**
		 * Конструктор.
		 * Ширина и воста будут браться из объекта window.
		 *
		 * @param window окно 
		 * @param numColorBuffers количество буфферов цвета 
		 * @throw runtime_error в случае если не удалось создать дескриптор для кадрового буффера
		 * @throw runtime_error в случае если не удалось создать дескриптор для renderbuffer'а в которой будут хранится значения глубины и трафарета
		*/
		template<typename Window>
		explicit Framebuffer(const Window& window, int32_t numColorBuffers = 1) :
			BaseFramebuffer<Tf>(window, numColorBuffers)
		{
			glGenRenderbuffers(1, &_depthAndStencilBufferHandle);

			if (!_depthAndStencilBufferHandle) {
				throw runtime_error("Error create descriptor for depth buffer");
			}

			auto[windowWidth, windowHeight] = window.size();

			glBindFramebuffer(GL_FRAMEBUFFER, BaseFramebuffer<Tf>::_framebufferHandle);
			glBindRenderbuffer(GL_RENDERBUFFER, _depthAndStencilBufferHandle);

			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowWidth,windowHeight);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthAndStencilBufferHandle);

			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		Framebuffer(Framebuffer&& framebuffer) :
			BaseFramebuffer<Tf>(move(framebuffer)),
			_depthAndStencilBufferHandle{0}
		{
			swap(_depthAndStencilBufferHandle, framebuffer._depthAndStencilBufferHandle);
		}

		Framebuffer(const Framebuffer&) = delete;
		Framebuffer& operator=(const Framebuffer&) = delete;
		Framebuffer& operator=(Framebuffer&&) = delete;

		virtual ~Framebuffer()
		{
			if (_depthAndStencilBufferHandle) {
				glDeleteRenderbuffers(1, &_depthAndStencilBufferHandle);
			}
		}

		inline void bindDepthAndStencilBuffer() const noexcept
		{
			glBindRenderbuffer(GL_RENDERBUFFER, _depthAndStencilBufferHandle);
		}

		inline void unbindDepthAndStencilBuffer() const noexcept
		{
			glBindRenderbuffer(GL_RENDERBUFFER, _depthAndStencilBufferHandle);
		}

	private:
		uint32_t _depthAndStencilBufferHandle;
	};

	/**
	 * Данный класс представляет из себя кадровый буффер к которому подключена
	 * только текстура, в которой хранятся значения глубины каждого фрагмента.
	 * С помощью этого класса можно создавать карты теней.
	*/
	template<uint8_t NumberOfBitsPerFragment, int32_t Width, int32_t Height>
	class ShadowMapRenderer :
		public IFramebuffer
	{
	public:
		explicit ShadowMapRenderer()
		{
			glGenFramebuffers(1, &_framebufferHandle);
			glGenTextures(1, &_depthTextureHandle);

			if (!_depthTextureHandle || !_framebufferHandle) {
				throw runtime_error("Error create descriptor for depth texture and framebuffer");
			}

			glBindFramebuffer(GL_FRAMEBUFFER, _framebufferHandle);
			glBindTexture(GL_TEXTURE_2D, _depthTextureHandle);

			if constexpr(NumberOfBitsPerFragment >= 32) {
				glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32, Width, Height);
			} else if constexpr (NumberOfBitsPerFragment >= 24)  {
				glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT24, Width, Height);
			} else {
				glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT16, Width, Height);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthTextureHandle, 0);

			glDrawBuffer(GL_NONE);
			glReadBuffer(GL_NONE);

			glBindTexture(GL_TEXTURE_2D, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		ShadowMapRenderer(ShadowMapRenderer&& smr) :
			_framebufferHandle{0},
			_depthTextureHandle{0}
		{
			swap(_framebufferHandle, smr._framebufferHandle);
			swap(_depthTextureHandle, smr._depthTextureHandle);
		}

		ShadowMapRenderer(const ShadowMapRenderer&) = delete;
		ShadowMapRenderer operator=(const ShadowMapRenderer&&) = delete;
		ShadowMapRenderer operator=(ShadowMapRenderer&&) = delete;

		virtual ~ShadowMapRenderer() 
		{
			if (_framebufferHandle) {
				glDeleteTextures(1, &_depthTextureHandle);
				glDeleteFramebuffers(1, &_framebufferHandle);
			}
		}

		virtual inline void bind() const noexcept override
		{
			glBindFramebuffer(GL_FRAMEBUFFER, _framebufferHandle);
		}

		virtual inline void unbind() const noexcept override
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		inline void bindDepthTexture(int32_t slot) const noexcept
		{
			glActiveTexture(GL_TEXTURE0 + slot);
			glBindTexture(GL_TEXTURE_2D, _depthTextureHandle);
		}

		inline void unbindDepthTexture(int32_t slot) const noexcept
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		/**
		 * Методы необходимые для считывания значений пикселя из кадрового буфера.
		 *
		 * @param data массив куда будут записаны значения пикселей
		 * (т. к. на каждый пиксель выделяется по три канала то размер массива должен быть width * height * 3)
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
		 * Методы необходимые для считывания значений пикселя из кадрового буфера.
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

	private:
		uint32_t _framebufferHandle;
		uint32_t _depthTextureHandle;
	};
}
