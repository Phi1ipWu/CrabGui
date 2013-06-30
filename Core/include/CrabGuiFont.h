
#ifndef __CrabGuiFont_h__
#define __CrabGuiFont_h__


namespace CrabGui
{

	/// 字体
	class CRABGUIEXPORT Font
	{
	public:
		Font() {}
		virtual ~Font() {}

		/// 从文件中载入图片
		virtual Bool	loadFont(PCStr pszFileName) = 0;

        /// 卸载字体
		virtual void    unloadFont() = 0;

		/// 是否已载入
		virtual Bool	isLoaded() = 0;

		///// 获得单个字的大小
		//virtual Bool	getCharSize(Point* ptSize, UInt uChar, UInt uHeight, UInt uStyle) = 0;

		/// 获得单个字的信息
		virtual UInt	getCharInfo(PVoid pData, UInt uDataSize, Point* pDataSize, Point* pOffset, Point* pCharSize, UInt uChar, UInt uHeight, UInt uStyle) = 0;


	public:
		static const UInt BF_Blod		= (1 << 0);
		static const UInt BF_Italic		= (1 << 1);
		static const UInt BF_Underline	= (1 << 2);
	};

}


#endif	//	__CrabGuiFont_h__
