
#ifndef __CrabGuiFontTexture_h__
#define __CrabGuiFontTexture_h__


namespace CrabGui
{

	class Font;
	class Texture;

	class Queue;
	class RBTree;


	/// 字体纹理
	class CRABGUIEXPORT FontTexture
	{
	public:
		FontTexture(Font* pFont);
		virtual ~FontTexture();

		struct FontChar
		{
			Texture*	pTexture;	// 字体纹理
			Rect		rcTexture;	// 字体切割
			Point		ptOffset;	// 字的原点偏移
			Point		ptCharSize;
		};

		/// 获得渲染用的字体
		FontChar*	getFontChar(UInt uChar, UInt uCharSize, UInt uStyle);

		/// 获得字的逻辑大小
		Bool		getFontSize(Point& ptSize, UInt uChar, UInt uCharSize, UInt uStyle);

		/// 缓存到纹理
		void		cacheToTexture(UInt uCharSize);

		/// 获取贴图
		Texture*	getTexture(UInt uCharSize, UInt uIndex);

	private:

		/// 根据字体大小获得 KEY
		UInt		_getKey(UInt uChar, UInt uCharSize, UInt uStyle);

		/// 获得渲染用的字体
		FontChar*	_createFontChar(UInt uChar, UInt uCharSize, UInt uStyle);


		/// 最小支持字体（必须大于 0）
		static const Int Min_Font_Size = 4;
		/// 最大支持字体大小（不能超过 255 = 0xFF）
		static const Int Max_Font_Size = 64;
		/// 边缘空间
		static const Int Font_EdgeSpace = 1;
		/// 纹理大小
		static const Int Font_TexSize = 256;

		/// 单个字号的缓存（同时会缓存多个字，一次性写入纹理）
		struct FontCache
		{
			Queue*	pQueueTexture;
			UInt8	szTextureData[Font_TexSize * Font_TexSize];

			Point	ptCurrPos;
			Point	ptNextRow;
			Bool	isDirty;
		};


	private:
		Font*		_pFont;
		RBTree*		_pRBTreeFont;

		UInt8*		_pBufferCache;
		UInt		_uBufferSize;

		FontCache	_stFontCaches[Max_Font_Size];
	};

}

#endif	//	__CrabGuiFontTexture_h__
