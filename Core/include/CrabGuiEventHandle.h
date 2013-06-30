
#ifndef __CrabGuiEventHandle_h__
#define __CrabGuiEventHandle_h__


namespace CrabGui
{

	class Window;


	enum EventDefine
	{
		Event_Null = 0,				// 事件描述				参数1		参数2		参数3		参数4

		Event_WindowCreate,			// 窗体创建
		Event_WindowDestroy,		// 窗体销毁
		Event_WindowClose,			// 窗体关闭

		// 键盘事件
		Event_KeyDown,				// 键盘键按下			KeyCode		0			0			CtrlKey
		Event_KeyUp,				// 键盘键松开			KeyCode		0			0			CtrlKey
		Event_KeyChar,				// 键盘输入字符			Char		0			0			CtrlKey

		// 鼠标事件
		Event_MouseMove,			// 鼠标移动				x			y			0			CtrlKey
		Event_MouseDown,			// 鼠标键按下			x			y			MouseKey	CtrlKey
		Event_MouseUp,				// 鼠标键松开			x			y			MouseKey	CtrlKey
		Event_MouseClick,			// 鼠标完整点击			x			y			MouseKey	CtrlKey
		Event_MouseScroll,			// 鼠标滚轮				scroll x	scroll y	0			CtrlKey

		// 滚动条事件
		Event_VertScroll,			// 垂直滚动条变化		Pos			0			0			0
		Event_HorzScroll,			// 垂直滚动条变化		Pos			0			0			0

		// 状态变化事件
		Event_FocusChanged,			// 焦点窗体变化			True/False
		Event_HoverChanged,			// 鼠标停留窗体变化		True/False
		//Event_SizeChanged,		// 窗体大小变化			new sx		new sy
		//Event_PosChanged,			// 窗体坐标变化			new x		new y
		//Event_StateChanged,		// 位状态变化			BitState	True/False

		// 鼠标拖曳事件
		Event_DragDrop,				// 拖拽事件				DragWindow

		// 道具项选择事件
		Event_ItemChanged,
		Event_ItemSelected,

		// 子控件扩展事件
		Event_ExtendStart,
	};


	struct CRABGUIEXPORT EventDesc
	{
		Window*		pWindow;
		Window*		pWindowOther;
		UInt		uEvent;
		Int			nParam[4];

		EventDesc();
	};


	/// 逻辑层的事件回调
	class CRABGUIEXPORT EventHandle
	{
	public:
		EventHandle();
		virtual ~EventHandle();

		/// 响应事件
		virtual Bool eventHandle(const EventDesc* pEvent);
	};

}


#endif	//	__CrabGuiEventHandle_h__
