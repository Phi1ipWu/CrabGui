
#ifndef __CrabGuiParser_h__
#define __CrabGuiParser_h__


namespace CrabGui
{

	class Window;
	class Image;
	class Font;


	/// 解析器
	class CRABGUIEXPORT Parser
	{
	public:
		Parser() {}
		virtual ~Parser() {}

		/// 从文件中创建
		virtual Window*		createFromFile(Window* pWndParent, PCStr pszFileName) = 0;

		/// 从内存中创建
		virtual Window*		createFromMemory(Window* pWndParent, void* pData, UInt nDataSize) = 0;

		/// 保存到文件
		virtual Bool		saveToFile(Window* pWindow, PCStr pszFileName) = 0;


	public:

		/// 创建图片接口
		virtual	Image*		createImage() = 0;

		/// 销毁图片接口
		virtual	void		destroyImage(Image* pImage) = 0;

		/// 创建字体接口
		virtual	Font*		createFont() = 0;

		/// 销毁图片接口
		virtual	void		destroyFont(Font* pFont) = 0;

	};
}


#endif	//	__CrabGuiParser_h__
