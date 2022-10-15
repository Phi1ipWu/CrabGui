
#ifndef __CrabGuiWindow_h__
#define __CrabGuiWindow_h__


namespace CrabGui
{

	class System;
	class Queue;
	class Canvas;
	class Mesh;

	struct EventDesc;
	class EventHandle;


	/// 窗体基础类
	class CRABGUIEXPORT Window
	{
	public:
		Window(System* pSystem, UInt uID);
		virtual ~Window();

		static PCStr	TypeName;

		/// 获得类型名
		virtual PCStr	getTypeName();

		/// 获得创建此窗体的系统
		System*			getSystem();


	public:	// 核心接口（慎用）

		/// 设置窗体名字（允许重名）
		void		setName(PCStr pszName);

		/// 获取窗体名字
		PCStr		getName();

		/// 设置脏矩型
		void		setDirtyRect(const Rect* pDirtyRect);

		/// 设置是否使用画布（0不使用，1简单矩形，>=2网格化）
		void		setUseCanvas(UInt uGridSize);

		/// 是否使用画布
		Bool		isUseCanvas() const;

		/// 获得画布矩形窗体（用于渲染图片，文字）
		void		getCanvasRect(Rect& rcClient, const Rect& rcWindowOffset, const Point& ptCanvasBase);


	public:	// 渲染相关

		/// 获得鼠标指针
		virtual UInt		getMouseCursor() const;

		/// 获得窗体矩形
		virtual void		getWindowRect(Rect& rcWindow) const;

		/// 逝去时间
		virtual Bool		update(UInt uElapse);

		/// 窗体裁减
		virtual void		setClipRect(Canvas* pCanvas, const Point& ptBase, const Rect& rcWindowClip);

		/// 获取网格
		virtual const Mesh*	getMesh() const;

		/// 准备渲染
		virtual Bool		preRender(Canvas*& pCanvas, Point& ptBase);

		/// 结束渲染
		virtual void		postRender(Canvas* pCanvas, const Point& ptBase);

		/// 渲染
		virtual void		render(Canvas* pCanvas, const Point& ptBase);


	public:	// 事件相关

		/// 事件响应函数
		virtual Bool	eventHandle(const EventDesc* pEvent);

		/// 设置逻辑层事件回调
		void			setEventHandle(EventHandle* pEventHandle);

		/// 获得逻辑层事件回调
		EventHandle*	getEventHandle() const;


	public:		// 窗体父子层级相关

		/// 设置父窗体
		Bool		setParent(Window* pWndParent);

		/// 获取父窗体
		Window*		getParent() const;

		/// 获得子窗体个数
		UInt		getNumChild() const;

		/// 获得子窗体
		Window*		getChild(UInt uIndex) const;

		/// 获得子窗体
		Window*		getChild(PCStr pszName) const;

		/// 获得邻居窗体
		Window*		getSibling(PCStr pszName) const;

		/// 激活窗体
		void		setFocus();

		/// 移到同一层的顶端
		void		moveTop();


	protected:	// 窗体父子层级相关

		/// 添加子窗体
		virtual Bool	_insertChild(Window* pWndChild);

		/// 移除子窗体
		virtual Bool	_removeChild(Window* pWndChild);


	public:	// 属性相关

		enum ZLayer
		{
			ZLayerModal			= 0,		// 模态层
			ZLayerTopMost		= 1,		// 永远最上层
			ZLayerDefault		= 64,		// 默认中间层
			ZLayerBottomMost	= 255,		// 永远最下层
		};

		/// 获得窗体层级
		void			setZLayer(UInt8 u8ZLayer);

        /// 设置窗体层级
        UInt8			getZLayer() const;

		/// 设置窗体位置
		void			setPos(const Point& ptPos);

		/// 获得窗体位置
		const Point&	getPos() const;

		/// 设置窗体大小
		void			setSize(const Point& ptSize);

		/// 获得窗体大小
		const Point&	getSize() const;

		/// 设置文字
		virtual void	setText(PCStr pszText);

		/// 获取文字
		virtual PCStr	getText() const;

		/// 鼠标命中测试（支持非矩形透明窗体）
		virtual Bool	hitTest(const Point& ptPos) const;


	//public:	// 滚动条相关（即将废除）

	//	/// 获得垂直滚动信息
	//	virtual Bool	getVertScroll(ScrollInfo& stScrollInfo);

	//	/// 设置垂直滚动条信息
	//	virtual Bool	setVertScroll(UInt uVertPos);

	//	/// 获得水平滚动条信息
	//	virtual Bool	getHorzScroll(ScrollInfo& stScrollInfo);

	//	/// 设置水平滚动条信息
	//	virtual Bool	setHorzScroll(UInt uHorzPos);


	// 新的滚动条接口，以上四个滚动条接口会废除
	public:	// 滚动条相关

		/// 设置垂直滚动条
		Bool	setVertScroll(const ScrollInfo& stScroll);

		/// 设置水平滚动条
		Bool	setHorzScroll(const ScrollInfo& stScroll);


	public:	// 滚动条响应相关（仅滚动条响应用）

		/// 父窗体垂直滚动设置事件
		virtual Bool	onParentSetVertScroll(const ScrollInfo& stScroll);

		/// 父窗体水平滚动设置事件
		virtual Bool	onParentSetHorzScroll(const ScrollInfo& stScroll);


	public:	// 位属性相关

		/// 位属性枚举（涉及编辑器及文件保存，不可修改）
		enum BitState
		{
			BS_Visible = 0,
			BS_Enable,
			BS_Movable,
			BS_Dragable,
			BS_Resizable,
			BS_ZChangeable,
			BS_KeyToParent,
			BS_MouseToParent,
			BS_AcceptDrop,

			BS_Size,

			BS_ExtendStart = 24,
		};

		/// 设置是否可见
		virtual void	setVisible(Bool isVisible);

		/// 设置是否可用
		virtual void	setEnable(Bool isEnable);

		/// 设置是否可移动
		virtual void	setMovable(Bool isMovable);

		/// 设置是否可拖曳
		virtual void	setDragable(Bool isDragable);

		/// 设置是否可缩放
		virtual void	setResizable(Bool isResizable);

		/// 设置是否允许变换层次
		virtual void	setZChangeable(Bool isZChangeable);

		/// 设置是否按键事件 发送父窗体
		virtual void	setKeyToParent(Bool isKeyToParent);

		/// 设置是否鼠标事件 发送父窗体
		virtual void	setMouseToParent(Bool isMouseToParent);

		/// 是否可见
		virtual Bool	isVisible() const;

		/// 是否可用
		virtual	Bool	isEnable() const;

		/// 是否可移动
		virtual Bool	isMovable() const;

		/// 是否可拖曳
		virtual Bool	isDragable() const;

		/// 是否可缩放
		virtual Bool	isResizable() const;

		/// 是否允许变换层次
		virtual Bool	isZChangeable() const;

		/// 是否按键事件 发送父窗体
		virtual Bool	isKeyToParent() const;

		/// 是否鼠标事件 发送父窗体
		virtual Bool	isMouseToParent() const;


	protected:

		/// 获得位状态属性
		Bool	_getBitState(UInt uBit) const;

		/// 设置位状态属性
		void	_setBitState(UInt uBit, Bool isTrue);


	public:	// 提供给编辑器的属性操作

		/// 窗体属性类型枚举
		enum PropertyType
		{
			PT_Null = 0,
			PT_String,
			PT_Int8,
			PT_UInt8,
			PT_Int16,
			PT_UInt16,
			PT_Int,
			PT_UInt,
			PT_Color,
			PT_Point,
			PT_Rect,

			PT_Bit0	= 0x80,
			PT_Bit1,
			PT_Bit2,
			PT_Bit3,
			PT_Bit4,
			PT_Bit5,
			PT_Bit6,
			PT_Bit7,
			PT_Bit8,
			PT_Bit9,
			PT_Bit10,
			PT_Bit11,
			PT_Bit12,
			PT_Bit13,
			PT_Bit14,
			PT_Bit15,
			PT_Bit16,
			PT_Bit17,
			PT_Bit18,
			PT_Bit19,
			PT_Bit20,
			PT_Bit21,
			PT_Bit22,
			PT_Bit23,
			PT_Bit24,
			PT_Bit25,
			PT_Bit26,
			PT_Bit27,
			PT_Bit28,
			PT_Bit29,
			PT_Bit30,
			PT_Bit31,
		};


		/// 窗体属性
		struct Property
		{
			PCStr			szKey;
			void*			pData;
			PropertyType	eType;
			PCStr			szOption;
		};


		/// 获得属性个数
		virtual UInt	getNumPropert();

		/// 设置窗体属性
		virtual Bool	setPropert(const String& strKey, const String& strValue);

		/// 获得窗体属性
		virtual Bool	getPropert(UInt uIndex, String& strKey, String& strValue);


	protected:

		/// 根据属性数组 设置属性值（子窗体也会用到）
		Bool	setPropertyImpl(Property* pPropList, UInt uNumProp, const String& strKey, const String& strValue);

		/// 根据属性数组 获得属性值（子窗体也会用到）
		Bool	getPropertyImpl(Property* pPropList, UInt uNumProp, UInt uIndex, String& strKey, String& strValue);

		/// 分割（Int16 类型取决于 Point, Rect 的数据结构）
		static void splitString(const String& strText, Char cSplit, Int16* pArray, UInt uArraySize);


	protected:

		System* const	_pSystem;
		UInt			_uID;

		Canvas*			_pCanvas;			// 窗体画布指针
		Mesh*			_pMesh;				// 窗体网格

		Queue*			_pQueueChilds;		// 子窗体队列
		Window*			_pWndParent;		// 父窗体指针

		EventHandle*	_pEventHandle;		// 逻辑层事件回调


	protected:	// 编辑器属性

		String			_strName;
		Point			_ptPos;
		Point			_ptSize;
		Point			_ptGridSize;	// 网格分割块大小

		UInt			_uBitState;
		UInt8			_u8ZLayer;
		UInt8			_u8TabStop;
		UInt8			_u8Reserve1, _u8Reserve2;

		String			_strText;
	};

}


#endif	// __CrabGuiWindow_h__
