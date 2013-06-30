
#ifndef __CrabGuiFreeParser_h__
#define __CrabGuiFreeParser_h__


#include "CrabGuiParser.h"

#include <stdio.h>


namespace CrabGui
{

	class System;


	/// Free解析器插件
    class CRABGUIEXPORT FreeParser : public Parser
    {
	public:
		FreeParser();
		~FreeParser();

		/// 从文件中创建
		virtual Window*		createFromFile(Window* pWndParent, PCStr pszFileName);

		/// 从内存中创建
		virtual Window*		createFromMemory(Window* pWndParent, void* pData, UInt nDataSize);

		/// 保存到文件
		virtual Bool		saveToFile(Window* pWindow, PCStr pszFileName);


	public:

		/// 创建图片接口
		virtual	Image*		createImage();

		/// 销毁图片接口
		virtual	void		destroyImage(Image* pImage);

		/// 创建字体接口
		virtual	Font*		createFont();

		/// 销毁图片接口
		virtual	void		destroyFont(Font* pFont);


	private:

		// 从字符串导入单个窗体（文件中一行即一个窗体的全部信息）
		Window*		_importWindow(UInt& uLayer, System* pSystem, const String& strLine);

		// 从窗体导出字符串
		void		_exportWindow(Window* pWindow, UInt uLayer);

		FILE*	_fp;
    };

}


#endif  //  __CrabGuiFreeParser_h__
