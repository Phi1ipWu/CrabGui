
#ifndef __CrabGuiD3D9Texture_h__
#define __CrabGuiD3D9Texture_h__


#include "CrabGuiTexture.h"

#include <d3dx9.h>


namespace CrabGui
{

	/// D3D9 纹理实现
	class CRABGUIEXPORT D3D9Texture : public Texture
    {
    public:
		D3D9Texture(IDirect3DDevice9* pDev);
		virtual ~D3D9Texture();

		/// 从文件中载入图片
		virtual Bool			loadFromFile(PCStr pszFileName);

		/// 从内存中载入图片
		virtual Bool			loadFromMemory(PCVoid pData, const Point& ptSize, UInt nComp);

		/// 保存到文件
		virtual Bool			saveToFile(PCStr pszFileName);

		/// 手动设置大小
		virtual Bool			setSize(const Point& ptSize);

		/// 获得纹理大小
		virtual const Point&	getSize();

        /// 获得像素格式
		virtual PixelFmt		getPixelFormat();

        /// 从内存中锁定数据
        virtual Bool			lock(void*& pData, UInt& nPitch);

		/// 内存数据解锁
		virtual Bool			unlock();


	private:

		friend class D3D9Renderer;

		IDirect3DDevice9*	_pDev;
		IDirect3DTexture9*  _pTex;

		Point       _ptSize;
        PixelFmt    _ePixelFmt;
    };

}


#endif	//	__CrabGuiD3D9Texture_h__
