
#ifndef __CrabGuiD3D9Renderer_h__
#define __CrabGuiD3D9Renderer_h__


#include "CrabGuiRenderer.h"

#include <d3dx9.h>


namespace CrabGui
{

	/// D3D9渲染器插件
	class CRABGUIEXPORT D3D9Renderer : public Renderer
	{
	public:
		D3D9Renderer(IDirect3DDevice9* pDev);
		virtual ~D3D9Renderer();

		/// 开始渲染
		virtual Bool	beginRender();

		/// 结束渲染
		virtual void	endRender();

		/// 设置裁减矩形
		virtual Bool	setScissorRect(Bool isEnable, const Rect& rcScissor);

		/// 矩形渲染（纹理）
		virtual Bool	renderTexture(Texture* pTexture, Color cDiffuse, const Rect& rcTex, const Rect& rcScreen);

		/// 矩形渲染（渲染目标）
		virtual Bool	renderRenderTarget(RenderTarget* pRT, Color cDiffuse, const Rect& rcTex, const Rect& rcScreen);

		/// 自定义顶点渲染（渲染目标）
		virtual Bool	renderRenderTargetVertices(RenderTarget* pRT, Int nPointSize, const PointReal* pPoints, const Color* pDiffuses, const PointReal* pTexPoints, Int nTriangleSize, const Int16* pTriangles);

		/// 渲染实现
		Bool	_render(IDirect3DTexture9* pTex, const Point& ptTexSize, Color cDiffuse, const Rect& rcTex, const Rect& rcScreen);


	public:

		/// 创建渲染目标
		virtual RenderTarget*	createRenderTarget();

		/// 销毁渲染目标
		virtual void			destroyRenderTarget(RenderTarget* pRT);

		/// 创建纹理接口
		virtual	Texture*		createTexture();

		/// 销毁纹理接口
		virtual	void			destroyTexture(Texture* pTexture);


	private:
		IDirect3DDevice9*	_pDev;
		Rect				_rcScissor;
	};

}


#endif	//	__CrabGuiD3D9Renderer_h__
