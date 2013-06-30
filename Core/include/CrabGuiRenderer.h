
#ifndef __CrabGuiRenderer_h__
#define __CrabGuiRenderer_h__


namespace CrabGui
{

	class RenderTarget;
	class Texture;


	/// 渲染器
	class CRABGUIEXPORT Renderer
	{
	public:
		Renderer() {}
		virtual ~Renderer() {}

		/// 开始渲染
		virtual Bool	beginRender() = 0;

		/// 结束渲染
		virtual void	endRender() = 0;

		/// 设置裁减矩形
		virtual Bool	setScissorRect(Bool isEnable, const Rect& rcScissor) = 0;

		/// 渲染（纹理）
		virtual Bool	renderTexture(Texture* pTexture, Color cDiffuse, const Rect& rcTex, const Rect& rcScreen) = 0;

		/// 渲染（渲染目标）
		virtual Bool	renderRenderTarget(RenderTarget* pRT, Color cDiffuse, const Rect& rcTex, const Rect& rcScreen) = 0;


	public:

		/// 创建渲染目标
		virtual RenderTarget*	createRenderTarget() = 0;

		/// 销毁渲染目标
		virtual void			destroyRenderTarget(RenderTarget* pRT) = 0;

		/// 创建纹理接口
		virtual	Texture*		createTexture() = 0;

		/// 销毁纹理接口
		virtual	void			destroyTexture(Texture* pTexture) = 0;

	};
}


#endif	//	__CrabGuiRenderer_h__
