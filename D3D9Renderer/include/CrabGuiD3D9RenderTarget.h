
#ifndef __CrabGuiD3D9RenderTarget_h__
#define __CrabGuiD3D9RenderTarget_h__


#include "CrabGuiRenderTarget.h"

#include <d3dx9.h>


namespace CrabGui
{

	/// D3D9渲染器目标
	class CRABGUIEXPORT D3D9RenderTarget : public RenderTarget
	{
	public:
		D3D9RenderTarget(IDirect3DDevice9* pD3D9Device);
		virtual ~D3D9RenderTarget();

		/// 设置渲染目标大小
		virtual Bool			setRenderSize(const Point& ptSize);

		/// 获得渲染目标大小
		virtual const Point&	getRenderSize();

		/// 绑定渲染目标（必须与 unbind 成对使用）
		virtual Bool			bind(const Rect& rcDirty, Color cClear);

		/// 取消绑定渲染目标
		virtual void			unbind();


	private:

		friend class D3D9Renderer;

		IDirect3DDevice9*	_pDev;
		IDirect3DTexture9*  _pTex;
		IDirect3DSurface9*	_pBackSuf;

		float		_matProj[16];

		Point       _ptSize;
	};

}


#endif	//	__CrabGuiD3D9RenderTarget_h__
