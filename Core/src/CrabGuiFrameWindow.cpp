
#include "CrabGuiDefine.h"
#include "CrabGuiFrameWindow.h"

#include "CrabGuiSystem.h"


namespace CrabGui
{

	FrameWindow::FrameWindow(System* pSystem, UInt uID)
		: Static(pSystem, uID)
	{
		// 默认可以切换Z层次
		setZChangeable(True);
	}

	FrameWindow::~FrameWindow()
	{
	}


	PCStr FrameWindow::TypeName = "FrameWindow";


	// 获得类型名
	PCStr FrameWindow::getTypeName()
	{
		return TypeName;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 渲染相关


	///////////////////////////////////////////////////////////////////////////////////////////////
	// 位属性相关


	// 设置是否可见
	void FrameWindow::setVisible(Bool isVisible)
	{
		Static::setVisible(isVisible);

		if(isVisible)
		{
			_pSystem->focusWindow(this);
		}
	}

}
