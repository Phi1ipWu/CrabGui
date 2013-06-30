
#include "CrabGuiDefine.h"
#include "CrabGuiFreeTypeFont.h"

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

// http://hi.baidu.com/fuxiaodefeng/blog/item/1bcb770f48cd1ee9ab6457fb.html
// http://my.unix-center.net/~Simon_fu/?p=372


namespace CrabGui
{

	static UInt			gs_uNumFont = 0;
	static FT_Library	gs_ftLib	= 0;


	FreeTypeFont::FreeTypeFont()
		:	_ftFace(0)
	{
		if(0 == gs_uNumFont++)
		{
			// 加载 FreeImage
			FT_Init_FreeType(&gs_ftLib);
		}
	}

	FreeTypeFont::~FreeTypeFont()
	{
		unloadFont();

		if(0 == --gs_uNumFont)
		{
			// 释放 FreeImage
			FT_Done_FreeType(gs_ftLib);
			gs_ftLib = 0;
		}
	}


	// 从文件中载入图片
	Bool FreeTypeFont::loadFont(PCStr pszFileName)
	{
		if(0 != FT_New_Face(gs_ftLib, pszFileName, 0, (FT_Face*)(&_ftFace)))
		{
			return False;
		}

		if(0 != FT_Select_Charmap((FT_Face)_ftFace, FT_ENCODING_UNICODE))
		{
			return False;
		}

		return True;
	}


	// 释放字体
	void FreeTypeFont::unloadFont()
	{
		if(_ftFace)
		{
			FT_Done_Face((FT_Face)_ftFace);
			_ftFace = 0;
		}
	}


	// 是否已载入
	Bool FreeTypeFont::isLoaded()
	{
		return !!_ftFace;
	}


	// 获得单个字的大小
	UInt FreeTypeFont::getCharInfo(PVoid pData, UInt uDataSize, Point* pDataSize, Point* pOffset, Point* pCharSize, UInt uChar, UInt uHeight, UInt uStyle)
	{
		if (0 != FT_Set_Pixel_Sizes((FT_Face)_ftFace, 0, uHeight))
		{
			return False;
		}

		Bool isGray = True;
		FT_Int32 nFlag = FT_LOAD_FORCE_AUTOHINT;
		nFlag |= isGray ? FT_LOAD_TARGET_NORMAL : FT_LOAD_TARGET_MONO;
		if (0 != FT_Load_Char((FT_Face)_ftFace, uChar, nFlag))
		{
			return False;
		}

		FT_Glyph ftGlyph;
		if (0 != FT_Get_Glyph(((FT_Face)_ftFace)->glyph, &ftGlyph))
		{
			return False;
		}

		// 转化成位图
		FT_Render_Glyph(((FT_Face)_ftFace)->glyph, FT_RENDER_MODE_NORMAL);//FT_RENDER_MODE_LCD);//FT_RENDER_MODE_NORMAL);
		FT_Glyph_To_Bitmap(&ftGlyph, FT_RENDER_MODE_NORMAL, 0, 1);	// 自动销毁老的Glyph
		FT_BitmapGlyph ftBitmapGlyph = (FT_BitmapGlyph)ftGlyph;
		FT_Bitmap& ftBitmap = ftBitmapGlyph->bitmap;

		// 字的完整大小
		const FT_GlyphSlot& ftGlyphSlot  = ((FT_Face)_ftFace)->glyph;
		const FT_Size_Metrics& ftMetrics = ((FT_Face)_ftFace)->size->metrics;

		Point ptCharSize(ftGlyphSlot->advance.x / 64, ftMetrics.y_ppem);
		Point ptDataSize(ftBitmap.width, ftBitmap.rows);
		Point ptOffset(ftGlyphSlot->bitmap_left,  ftMetrics.y_ppem - ftGlyphSlot->bitmap_top - 1);

		UInt uLoadSize = ptDataSize.getArea();
		if (pData && uDataSize >= uLoadSize)
		{
			// 复制文字点阵
			memcpy(pData, ftBitmap.buffer, ftBitmap.width * ftBitmap.rows);
		}
		else
		{
			// 缓存不够
			memset(pData, 0, uDataSize);
		}

		FT_Done_Glyph(ftGlyph);

		if (pDataSize)	*pDataSize	= ptDataSize;
		if (pOffset)	*pOffset	= ptOffset;
		if (pCharSize)	*pCharSize	= ptCharSize;

		return uLoadSize;
	}
}
