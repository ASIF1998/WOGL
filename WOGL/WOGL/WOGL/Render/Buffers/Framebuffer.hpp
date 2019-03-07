//
//  Framebuffer.hpp
//  WOGL
//
//  Created by Асиф Мамедов on 03/03/2019.
//  Copyright © 2019 Asif Mamedov. All rights reserved.
//

#ifndef FrameBuffer_hpp
#define FrameBuffer_hpp

namespace WOGL
{
	enum class WritePixels
	{
		NoWrite,
		Texture,
		RenderBuffer
	};
}

#include "Framebuffer.inl"

#endif /* FrameBuffer_hpp */
