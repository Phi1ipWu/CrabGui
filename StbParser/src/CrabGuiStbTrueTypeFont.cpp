
#include "CrabGuiDefine.h"
#include "CrabGuiStbTrueTypeFont.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"


namespace CrabGui
{

	StbTrueTypeFont::StbTrueTypeFont()
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
		return False;
	}


	// 释放字体
	void StbTrueTypeFont::unloadFont()
	{
	}


	// 是否已载入
	Bool StbTrueTypeFont::isLoaded()
	{
		return False;
	}


	// 获得单个字的大小
	UInt StbTrueTypeFont::getCharInfo(PVoid pData, UInt uDataSize, Point* pDataSize, Point* pOffset, Point* pCharSize, UInt uChar, UInt uHeight, UInt uStyle)
	{
		return 0;
	}
}
