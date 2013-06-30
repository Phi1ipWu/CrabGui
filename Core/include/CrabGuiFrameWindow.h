
#ifndef __CrabGuiFrameWindow_h__
#define __CrabGuiFrameWindow_h__


#include "CrabGuiStatic.h"


namespace CrabGui
{

	/// 静态窗体
	class CRABGUIEXPORT FrameWindow : public Static
	{
	public:
		FrameWindow(System* pSystem, UInt uID);
		virtual ~FrameWindow();

		static PCStr	TypeName;

		/// 获得类型名
		virtual PCStr	getTypeName();


	public:	// 渲染相关

        // 准备加入函数来渲染画布
        // canvas->render 的包装


	public:	// 位属性相关

		/// 设置是否可见
		virtual void	setVisible(Bool isVisible);


	};

}


#endif	//	__CrabGuiFrameWindow_h__
