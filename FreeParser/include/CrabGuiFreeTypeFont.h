
#ifndef __CrabGuiFreeTypeFont_h__
#define __CrabGuiFreeTypeFont_h__


#include "CrabGuiFont.h"


namespace CrabGui
{

    class CRABGUIEXPORT FreeTypeFont : public Font
    {
	public:
		FreeTypeFont();
		virtual ~FreeTypeFont();

		/// 从文件中载入字体
		virtual Bool	loadFont(PCStr pszFileName);

        /// 卸载字体
		virtual void    unloadFont();

		/// 是否已载入
		virtual Bool	isLoaded();

		///// 获得单个字的大小
		//virtual Bool	getCharSize(Point* ptSize, UInt uChar, UInt uHeight, UInt uStyle);

		/// 获得字像素大小
		virtual UInt	getCharInfo(PVoid, UInt uDataSize, Point* pDataSize, Point* pOffset, Point* pCharSize, UInt uChar, UInt uHeight, UInt uStyle);


    private:

		void*		_ftFace;
    };

}


#endif  //  __CrabGuiFreeTypeFont_h__
