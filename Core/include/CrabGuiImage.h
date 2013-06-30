
#ifndef __CrabGuiImage_h__
#define __CrabGuiImage_h__


namespace CrabGui
{

	/// 图片
	class CRABGUIEXPORT Image
	{
	public:
		Image() {}
		virtual ~Image() {}

		/// 从文件中载入图片
		virtual Bool	loadFromFile(PCStr pszFileName) = 0;

		/// 从内存中载入图片
		virtual Bool	loadFromMemory(PCVoid pData, UInt nDataSize) = 0;

        /// 卸载图片
        virtual void    unloadImage() = 0;

		/// 是否已载入
		virtual Bool	isLoaded() = 0;

		/// 获得文件名
		virtual PCStr	getFileName() = 0;

		/// 获得图片大小
		virtual Point	getSize() = 0;

		/// 设置坐标点颜色
		virtual Bool	setColor(const Point& ptPos, Color cColor) = 0;

		/// 获得坐标点的颜色
		virtual Color	getColor(const Point& ptPos) = 0;

		/// 获得插值颜色
		virtual Color	getBlurColor(float x, float y) = 0;


	public:

		/// 另存为PNG，接口可能会废
		virtual Bool	saveAsPNG(PCStr pszFileName, Rect* pRectClip, Point* pResize) = 0;

	};
}


#endif	//	__CrabGuiImage_h__
