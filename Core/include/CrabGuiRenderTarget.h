
#ifndef __CrabGuiRenderTarget_h__
#define __CrabGuiRenderTarget_h__


namespace CrabGui
{

	/// 渲染目标
	class CRABGUIEXPORT RenderTarget
	{
	public:
		RenderTarget() {}
		virtual ~RenderTarget() {}

		/// 设置渲染目标大小
		virtual Bool			setRenderSize(const Point& ptSize) = 0;

		/// 获得渲染目标大小
		virtual const Point&	getRenderSize() = 0;

		/// 绑定渲染目标（必须与 unbind 成对使用）
		virtual Bool			bind(const Rect& rcDirty, Color cClear) = 0;

		/// 取消绑定渲染目标
		virtual void			unbind() = 0;
	};
}


#endif	//	__CrabGuiRenderTarget_h__
