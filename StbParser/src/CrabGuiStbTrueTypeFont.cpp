
#include "CrabGuiDefine.h"
#include "CrabGuiStbTrueTypeFont.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include <stdio.h>


namespace CrabGui
{

	StbTrueTypeFont::StbTrueTypeFont()
		:	_pFontInfo(0)
		,	_pFileBuffer(0)
	{
	}

	StbTrueTypeFont::~StbTrueTypeFont()
	{
		unloadFont();
	}


	// 从文件中载入图片
	Bool StbTrueTypeFont::loadFont(PCStr pszFileName)
	{
		unloadFont();

		// 加载字体文件
		long int nFileSize = 0;
		FILE* fpFont = fopen(pszFileName, "rb");
		if (fpFont == NULL)
			return False;

		fseek(fpFont, 0, SEEK_END);
		nFileSize = ftell(fpFont);
		fseek(fpFont, 0, SEEK_SET);

		_pFileBuffer = CrabNewArray(UInt8, nFileSize);
		fread(_pFileBuffer, nFileSize, 1, fpFont);
		fclose(fpFont);

		// 初始化字体
		_pFontInfo  = CrabNew(stbtt_fontinfo);
		int nOffset = stbtt_GetFontOffsetForIndex(_pFileBuffer, 0);
		if (!stbtt_InitFont(_pFontInfo, _pFileBuffer, nOffset))
		{
			unloadFont();
			return False;
		}

		return True;
	}


	// 释放字体
	void StbTrueTypeFont::unloadFont()
	{
		CrabDelete(_pFontInfo);
		CrabDeleteArray(_pFileBuffer);
	}


	// 是否已载入
	Bool StbTrueTypeFont::isLoaded()
	{
		return !!_pFontInfo;
	}


	// 获得单个字的大小
	UInt StbTrueTypeFont::getCharInfo(PVoid pData, UInt uDataSize, Point* pDataSize, Point* pOffset, Point* pCharSize, UInt uChar, UInt uHeight, UInt uStyle)
	{

		// "STB"的 unicode 编码
		//unsigned short word = 0x6211;

		// 计算字体缩放
		float pixels = uHeight;//64.0;                                    // 字体大小（字号）
		float scale = stbtt_ScaleForPixelHeight(_pFontInfo, pixels); // scale = pixels / (ascent - descent)

		// 获取垂直方向上的度量 
		// ascent：字体从基线到顶部的高度；
		// descent：基线到底部的高度，通常为负值；
		// lineGap：两个字体之间的间距；
		// 行间距为：ascent - descent + lineGap。
		int ascent = 0;
		int descent = 0;
		int lineGap = 0;
		stbtt_GetFontVMetrics(_pFontInfo, &ascent, &descent, &lineGap);

		// 根据缩放调整字高
		ascent  = ceil(ascent  * scale);
		descent = ceil(descent * scale);

		//int x = 0; //位图的x

		// 获取水平方向上的度量
		// advanceWidth：字宽；
		// leftSideBearing：左侧位置；
		int advanceWidth = 0;
		int leftSideBearing = 0;
		stbtt_GetCodepointHMetrics(_pFontInfo, uChar, &advanceWidth, &leftSideBearing);

		// 获取字符的边框（边界）
		int c_x1, c_y1, c_x2, c_y2;
		stbtt_GetCodepointBitmapBox(_pFontInfo, uChar, scale, scale, &c_x1, &c_y1, &c_x2, &c_y2);

		Point ptOffset(c_x1, uHeight - ascent);
		Point ptDataSize(abs(c_x2 - c_x1 + 1), abs(c_y2 - c_y1 + 1));
		Point ptCharSize(ptDataSize);

		if (pData && uDataSize >= ptDataSize.getArea())
		{
			stbtt_MakeCodepointBitmap(_pFontInfo, (UInt8*)pData, ptDataSize.x, ptDataSize.y, ptDataSize.x, scale, scale, uChar);
		}
		//// 计算位图的y (不同字符的高度不同）
		////int y = ascent + c_y1;

		// unsigned char* bitmap = (unsigned char*)malloc(abs(c_x2 - c_x1 + 1) * abs(c_y2 - c_y1 + 1));
		// stbtt_MakeCodepointBitmap(&info, bitmap, c_x2 - c_x1 + 1, c_y2 - c_y1 + 1, c_x2 - c_x1 + 1, scale, scale, word);

		//// 渲染字符
		// int byteOffset = x + roundf(leftSideBearing * scale) + (y * bitmap_w);
		// stbtt_MakeCodepointBitmap(&info, bitmap + byteOffset, c_x2 - c_x1, c_y2 - c_y1, bitmap_w, scale, scale, word[i]);

		//// 调整x
		// x += roundf(advanceWidth * scale);

		//// 调整字距
		// int kern;
		// kern = stbtt_GetCodepointKernAdvance(&info, word[i], word[i + 1]);
		// x += roundf(kern * scale);

		//for (int y = 0; y < c_y2 - c_y1 + 1; ++ y)
		//{
			//for (int x = 0; x < c_x2 - c_x1 + 1; ++x)
			//	putchar(" *"[bitmap[y * (c_x2 - c_x1 + 1) + x] >> 7]);
			//putchar('\n');
		//}

		//free(bitmap);
		//free(fontBuffer);

		if (pDataSize)	*pDataSize	= ptDataSize;
		if (pOffset)	*pOffset	= ptOffset;
		if (pCharSize)	*pCharSize	= ptCharSize;

		return ptDataSize.getArea();
	}
}
