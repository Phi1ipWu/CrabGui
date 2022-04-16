
#ifndef __CrabGuiDefine_h__
#define __CrabGuiDefine_h__


namespace CrabGui
{

	//
	//	内存分配
	//
	#define		CrabNew(t)				new t
	#define		CrabNewArray(t, s)		new t[s]

	#define		CrabDelete(p)			if(p)	{ delete (p);     (p) = 0; }
	#define		CrabDeleteArray(p)		if(p)	{ delete[] (p);	  (p) = 0; }
	#define		CrabRelease(p)			if(p)	{ (p)->Release(); (p) = 0; }


	//
	//	导出宏
	//
	#ifdef WIN32
		#ifdef CRABGUI_DLL
			#define	CRABGUIEXPORT	__declspec(dllexport)
		#else
			#define	CRABGUIEXPORT	__declspec(dllimport)
		#endif
	#else
		#define	CRABGUIEXPORT
	#endif


	//
	//	数据类型定义
	//
	#define True			1
	#define False			0
	typedef int				Bool;
	typedef char			Char;
	typedef wchar_t			WChar;

	typedef unsigned char	UInt8;
	typedef unsigned short	UInt16;
	typedef unsigned int	UInt;
	typedef unsigned int	Color;
	typedef char			Int8;
	typedef short			Int16;
	typedef int				Int;
	typedef float			Real;

	typedef const char*		PCStr;
	typedef char*			PStr;
	typedef const wchar_t*	PCWStr;
	typedef wchar_t*		PWStr;
	typedef const void*		PCVoid;
	typedef void*			PVoid;


	/// 简单算法宏
	#define Min(a, b)	((a) < (b) ? (a) : (b))
	#define Max(a, b)	((a) > (b) ? (a) : (b))


	/// 鼠标按键
	enum MouseButton
	{
		MB_Left = 0,
		MB_Right,
		MB_Middle,
		MB_X1,
		MB_X2,

		MB_Size,
	};


	/// 键盘按键
	enum KeyButton
	{
		KB_Backspace	= 0x08,
		KB_Tab			= 0x09,

		KB_Clear		= 0x0C,
		KB_Return		= 0x0D,

		KB_Shift		= 0x10,
		KB_Ctrl			= 0x11,
		KB_Alt			= 0x12,

		KB_Space		= 0x20,
		KB_PageUp		= 0x21,
		KB_PageDown		= 0x22,
		KB_End			= 0x23,
		KB_Home			= 0x24,

		KB_Left			= 0x25,
		KB_Up			= 0x26,
		KB_Right		= 0x27,
		KB_Down			= 0x28,

		KB_Insert		= 0x2D,
		KB_Delete		= 0x2E,

		KB_0			= 0x30,
		KB_1,
		KB_2,
		KB_3,
		KB_4,
		KB_5,
		KB_6,
		KB_7,
		KB_8,
		KB_9,

		KB_A			= 0x41,
		KB_B,
		KB_C,
		KB_D,
		KB_E,
		KB_F,
		KB_G,
		KB_H,
		KB_I,
		KB_J,
		KB_K,
		KB_L,
		KB_M,
		KB_O,
		KB_P,
		KB_Q,
		KB_R,
		KB_S,
		KB_T,
		KB_U,
		KB_V,
		KB_W,
		KB_X,
		KB_Y,
		KB_Z,
	};


	/// 控制键枚举（鼠标键盘事件第四参数）
	enum ControlKey
	{
		CK_LButton	= (1 << 0),
		CK_RButton	= (1 << 1),
		CK_MButton	= (1 << 2),
		CK_Ctrl		= (1 << 3),
		CK_Alt		= (1 << 4),
		CK_Shift	= (1 << 5),
	};


	/// 鼠标指针
	enum MouseCursor
	{
		MC_Null	= 0,
		MC_Arrow,			// 箭头
		MC_ArrowDown,		// 箭头按下
		MC_EditBeam,		// 编辑框
		MC_Hand,			// 手型
		MC_SizeNESW,		// 右上，左下箭头
		MC_SizeNS,			// 上下箭头
		MC_SizeNWSE,		// 左上，右下箭头
		MC_SizeWE,			// 左右箭头

		MC_Size,
	};


	/// 像素格式
	enum PixelFmt
	{
		PF_ARGB = 0,		// A8 R8 G8 B8
		PF_RGB,				// R8 G8 B8

		PF_Size,
	};


	/// Alpha混合（待定）
	enum AlphaOp
	{
		AO_Src = 0,			// Sa = Src		（一般指：画布）
		AO_Dest,			// Sa = Dst		（一般指：图片，字体）
		AO_Blend,			// Sa = (1 - Alpha) * Src + (Alpha) * Dst

		AO_Size,

		AOP_Default		= 0,

		// RGBs * As + RGBd * (1 - As)
		AOP_CAMul_AMul,			// As * (1 - Ad)
		AOP_CAMul_AAdd,			// As + (1 - Ad)
		AOP_CAMul_ASrc,			// As
		AOP_CAMul_ADst,			// Ad

		AOP_CMul_AMul,
	};


	/// 对齐方式
	enum AlignType
	{
		AT_Center = 0,		// 居中对齐
		AT_Left,			// 左对齐（上下居中）
		AT_Top,				// 顶对齐（左右居中）
		AT_Right,			// 右对齐（上下居中）
		AT_Bottom,			// 底对齐（左右居中）
		AT_LeftTop,			// 左上对齐
		AT_RightTop,		// 右上对齐
		AT_LeftBottom,		// 左下对齐
		AT_RightBottom,		// 右下对齐

		AT_TopLeft,			// 上左对齐（仅窗体对齐时有区别）
		AT_TopRight,		// 上右对齐（仅窗体对齐时有区别）
		AT_BottomLeft,		// 下左对齐（仅窗体对齐时有区别）
		AT_BottomRight,		// 下右对齐（仅窗体对齐时有区别）

		AT_Size,
	};


	/// 日志等级
	enum LogLevel
	{
		LL_Disable = 0,
		LL_Low,
		LL_Normal,
		LL_High,
	};


	/// 滚动条信息
	struct CRABGUIEXPORT ScrollInfo
	{
		UInt		uDocSize;		// 文档大小
		UInt		uPageSize;		// 页大小

		UInt		uPos;			// 当前位置（0 - (文档大小 - 页大小)）
		UInt		uStep;			// 点上下按钮移动距离
	};


	/// 点
	struct CRABGUIEXPORT Point
	{
		Int16	x;
		Int16	y;

		Point();
		Point(Int16 x_, Int16 y_);
		Point(Int x_, Int y_);

		/// 设置点
		void	setPoint(Int16 x_, Int16 y_);

		/// 获得面积
		Int		getArea() const;

        /// 是否是零点
		Bool	isZero() const;

		/// 点设零
		void	setZero();

		/// 点偏移
		void	offset(const Point& pt);

		/// 相等操作
		Point&	operator=(const Point& pt);

		/// 点相加操作
		Point&	operator+=(const Point& pt);

		/// 点相等比较操作
		Bool	operator==(const Point& pt) const;

		/// 点不等比较操作
		Bool	operator!=(const Point& pt) const;
	};


	/// 浮点数点（仅渲染用）
	struct CRABGUIEXPORT PointF
	{
		float x, y;
	};


	/// 矩形
	struct CRABGUIEXPORT Rect
	{
		Int16	x1, y1;
		Int16	x2, y2;

		Rect();
		Rect(Int16 x1_, Int16 y1_, Int16 x2_, Int16 y2_);
		Rect(const Rect& rc, const Point& ptOffset);

		/// 设置矩型
		void	setRect(Int16 x1_, Int16 y1_, Int16 x2_, Int16 y2_);

		/// 获得宽度
		Int		getWidth() const;

		/// 获得高度
		Int		getHeight() const;

        /// 获得大小
		Point	getSize() const;

		/// 获得面积
		Int		getArea() const;

		/// 是否矩形相交
		Bool	isIntersect(const Rect& rc) const;

		/// 是否点在矩形内
		Bool	isInRect(const Point& ptPos);

		/// 是否是零矩形
        Bool	isZero() const;

		/// 矩型设零
		void	setZero();

		/// 矩形偏移
		void	offset(const Point& ptOffset);

		/// 矩形偏移
		void	offset(const Rect& rcOffset);

		/// 矩形合并
		Rect&	merge(const Rect& rc);

		/// 相交合并
		Rect&	intersect(const Rect& rc);
	};


    /// 字符串类
    class CRABGUIEXPORT String
    {
    public:
		String();
		String(PCStr pszText);
		virtual ~String();

		/// 获得字符串长度
		UInt	getLength() const;

		/// 获得宽字符串长度
		UInt	getWLength() const;

		/// 获得字符串指针
		PCStr	getStrPtr() const;

		/// 获得宽字符串指针
		PCWStr	getWStrPtr() const;

		/// 获得字符
		Char	getCharAt(UInt uIndex) const;

		/// 获得宽字符
		WChar	getWCharAt(UInt uIndex) const;

		/// 是否宽字符脏了（即ANSI有更改）
		Bool	isWCharDirty() const;

		/// 是否宽字符坐标位置脏了（需要新计算字的位置）
		Bool	isWCharPosDirty() const;

		/// 设置宽字符坐标脏了（一般是排版方式变更）
		void	setWCharPosDirty(Bool isDirty);

		/// 字符串格式化
		void	format(PCStr pszFmt, ...);

		/// 字符串清空
		void	setZero();

		/// 复制子串
		void	copySubstr(const String& strFull, UInt uStart, UInt uLength);

		/// 插入宽字符串（适用于 Editbox）
		void	insertWChar(UInt uPos, PCWStr pszWChar);

		/// 移除宽字符串（适用于 Editbox）
		void	removeWChar(UInt uPos, UInt uSize);

		/// 转换为整数
		Int		toInt() const;

		/// 转换为整数
		UInt	toUInt() const;

		/// 哈希码
		UInt	hashCode() const;

		/// 查找
		UInt	find(PCStr pszFind, UInt uStart = 0) const;

		/// 查找字符
		UInt	find(Char cFind, UInt uStart = 0) const;

		static const UInt NullPos = UInt(-1);


	public:	// 渲染相关

		enum WCharBitState
		{
			WCBS_Visible	= (1 << 0),		// 字符可见
			WCBS_Return		= (1 << 1),		// 回车
			WCBS_Eof		= (1 << 2),		// 结束符
		};

		struct WCharPos
		{
			Point	ptPos;
			Point	ptSize;

			UInt	uBitState;
		};

		/// 设置单个字的坐标
		Bool			setWCharPos(UInt uIndex, const WCharPos& stWCharPos);

		/// 获得单个字的坐标
		const WCharPos*	getWCharPos(UInt uIndex) const;


	public:

		/// 字符串赋值
		String&	operator=(const String& strText);

		/// 字符串赋值操作
		String&	operator=(PCStr pszText);

		/// 字符串赋值操作
		String&	operator=(PCWStr pszText);

		///// 字符串添加操作
		//String& operator+(PCStr pszText);

		/// 字符串添加操作
		String&	operator+=(const String& strText);

		/// 字符串添加操作
		String&	operator+=(Char cChar);

		/// 字符串与字符串比较操作
		Bool	operator==(String& strText) const;

		/// 字符串与字符指针比较操作
		Bool	operator==(PCStr pszText) const;

		/// 字符串比较
		Bool	operator!=(PCStr pszText) const;

		/// 字符串小于比较
		Bool	operator<(const String& strText) const;

		/// 字符串大于比较
		Bool	operator>(const String& strText) const;

		/// 字符串取值
		Char&	operator[](UInt uIndex);


	private:

		/// 默认增速
		static const UInt DefaultIncrease;

		/// 字符串大小重设
		void _resizeChar(UInt uSize) const;

		/// 宽字符串大小重设
		void _resizeWChar(UInt uWSize) const;


	private:

		mutable Char*	_pChar;
		mutable UInt	_uLength;
		mutable UInt	_uSize;

		mutable WChar*	_pWChar;
		mutable UInt	_uWLength;
		mutable UInt	_uWSize;

		mutable WCharPos*	_pWCharPos;

		mutable UInt8	_isCharDirty;
		mutable UInt8	_isWCharDirty;
		mutable UInt8	_isWCharPosDirty;
		mutable UInt8	_isReserve;


    };


	/// 图片信息
	struct CRABGUIEXPORT Picture
	{
		String		strName;		// 图片文件名/自定义名
		Rect		rcPicRect;		// 图片矩形切割（Zero = 全部）
		Rect		rcPicGrid;		// 图片九宫格（Zero = 不切割）
		Color		cPicColor;		// 图片颜色
	};

}


#endif	//	__CrabGuiDefine_h__

