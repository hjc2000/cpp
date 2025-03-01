#pragma once
#include <ffmpeg-wrapper/wrapper/AVFrameWrapper.h>
#include <sdl2-wrapper/SDL_Initializer.h>

namespace video
{
	class SDL_TextureWrapper final :
		public base::Wrapper<SDL_Texture>
	{
	private:
		SDL_Texture *_wrapped_obj = nullptr;

	public:
		SDL_TextureWrapper(SDL_Texture *texture);
		~SDL_TextureWrapper();

		/// <summary>
		///		使用 YUV 数组更新纹理
		/// </summary>
		///
		/// <param name="rect">一个矩形，表示要更新纹理的哪一部分。传入空指针表示更新整个纹理</param>
		///
		/// <param name="Yplane">Y 数组的头指针</param>
		/// <param name="Ysize">Y数组的长度</param>
		///
		/// <param name="Uplane">U 数组的头指针</param>
		/// <param name="Usize">U 数组的长度</param>
		///
		/// <param name="Vplane">V 数组的头指针</param>
		/// <param name="Vsize">V 数组的长度</param>
		///
		/// <returns>成功返回 0，失败返回错误代码</returns>
		int UpdateYUVTexture(
			SDL_Rect const *rect,
			Uint8 const *Yplane, int Ysize,
			Uint8 const *Uplane, int Usize,
			Uint8 const *Vplane, int Vsize);

		/// <summary>
		///		使用 AVFrameWrapper 更新纹理。
		/// </summary>
		/// <param name="rect">要更新的目标矩形。如果传入空指针，表示更新整个纹理</param>
		/// <param name="frame">AVFrameWrapper 对象</param>
		/// <returns>成功返回 0，失败返回错误代码</returns>
		int UpdateYUVTexture(SDL_Rect const *rect, AVFrameWrapper &frame);

		SDL_Texture *WrappedObj() const override
		{
			return _wrapped_obj;
		}
	};
} // namespace video
