
#ifndef __CrabGuiTexture_h__
#define __CrabGuiTexture_h__


namespace CrabGui
{

	/// 纹理
	class CRABGUIEXPORT Texture
    {
    public:
		Texture() {}
		virtual ~Texture() {}

		/// 从文件中载入图片
		virtual Bool			loadFromFile(PCStr pszFileName) = 0;

		/// 从内存中载入图片
		virtual Bool			loadFromMemory(PCVoid pData, const Point& ptSize, UInt nComp) = 0;

		/// 保存到文件
		virtual Bool			saveToFile(PCStr pszFileName) = 0;

		/// 手动设置大小
		virtual Bool			setSize(const Point& ptSize) = 0;

		/// 获得纹理大小
		virtual const Point&	getSize() = 0;

        /// 获得像素格式
		virtual PixelFmt		getPixelFormat() = 0;

        /// 从内存中锁定数据
        virtual Bool			lock(void*& pData, UInt& nPitch) = 0;

		/// 内存数据解锁
		virtual Bool			unlock() = 0;
    };

}


#endif	// __CrabGuiTexture_h__
