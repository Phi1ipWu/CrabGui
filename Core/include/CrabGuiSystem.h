
#ifndef __CrabGuiSystem_h__
#define __CrabGuiSystem_h__


namespace CrabGui
{

	class Renderer;
	class Texture;
	class Image;
	class Parser;
	class Font;

	class Queue;
	class RBTree;

	class Window;
	class Canvas;
	class FontTexture;


	/// UI系统，一切的根源
	class CRABGUIEXPORT System
	{
	public:
		System(Renderer* pRenderer, Parser* pParser, PCStr pszLocale);
		virtual ~System(void);

		static System&	getSingleton();
		static System*	getSingletonPtr();

        /// 获得渲染器
        Renderer*	getRenderer();

        /// 获得解析器
        Parser*		getParser();

		/// 日志记录
		void		log(LogLevel eLogLevel, PCStr pszLogFmt, ...);

		/// 打印调试信息
		void		print(PCStr pszDbgFmt, ...);


	public:	// 资源相关

		/// 获得图片RAW
		Image*			acquireImage(const String& strName);

		/// 释放图片RAW
		void			releaseImage(Image* pImage);

		/// 获得纹理
		Texture*		acquireTexture(const String& strName);

		/// 释放纹理
		void			releaseTexture(Texture* pTexture);

		/// 销毁所有纹理
		void			destroyAllTexture();

		/// 获得字体
		FontTexture*	acquireFont(const String& strName);

		/// 释放字体
		void			releaseFont(FontTexture* pFont);

		/// 创建字体
		FontTexture*	createFont(PCStr pszFileName);

		/// 销毁所有字体
		void			destroyAllFont();


	public:	// 矩形图片相关

		/// 获得矩形图片
		Picture*		acquirePicture(const String& strName);

		/// 释放矩形图片
		void			releasePicture(const String& strName);

		/// 创建矩形图片
		Picture*		createPicture(const String& strName, const Rect& rcPicRect, const Rect& rcPicGrid, Color cPicColor);

		/// 销毁矩形图片
		void			destroyAllPicture();


	public: // 外部事件传入

		/// 鼠标坐标移动事件
		Bool	injectMousePosition(const Point& ptPos);

		/// 鼠标按键按下事件
		Bool	injectMouseKeyDown(UInt uMouseKey);

		/// 鼠标按键抬起事件
		Bool	injectMouseKeyUp(UInt uMouseKey);

		/// 鼠标滚轮事件（仅支持Y轴）
		Bool	injectMouseWheel(const Point& ptScroll);

		/// 键盘按键按下事件
		Bool	injectKeyDown(UInt uKey);

		/// 键盘按键抬起事件
		Bool	injectKeyUp(UInt uKey);

		/// 键盘字符事件
		Bool	injectKeyChar(UInt uKeyChar);

		/// 窗体大小变化事件
		Bool	injectResize(const Point& ptSize);

		/// 时间流逝事件（单位：毫秒）
		Bool	injectTimeElapse(UInt uElapse);

		/// 渲染GUI
		Bool	renderGui();


	public: // 窗体相关

		/// 创建窗体
		Window*		createWindow(PCStr pszWindowType);

		/// 销毁窗体
		void		destroyWindow(Window* pWindow);

		/// 更改窗体类型（重新创建一个新的）
		Window*		changeWindowType(Window* pWnd, PCStr pszWndType);

		/// 激活窗体
		void		focusWindow(Window* pWndFocus);

		/// 设置根窗体
		void		setRootWindow(Window* pWndRoot);

		/// 获得根窗体
		Window*		getRootWindow();

		/// 获得当前激活窗体
		Window*		getFocusWindow();

		/// 获得鼠标拾取窗体
		Window*		getMouseWindow();

		/// 获得拖拽窗体
		Window*		getDragWindow();

		/// 获得鼠标指针
		MouseCursor	getMouseCursor();

		/// 复制窗体属性
		UInt		copyProperty(Window* pWndDst, Window* pWndSrc);


	private:// 窗体特殊处理

		/// 创建窗体事件
		void		_createWindowEvent();

		/// 销毁窗体事件
		void		_destroyWindowEvent();


	public:	// 属性相关

		const Point&	getScreenMousePos() const;

		/// 获取CTRL/ALT/SHIFT/LBUTTON按键状态
		UInt			getCtrlKeyState() const;


	public:	// 编辑器相关

		/// 设置编辑器模式
		void		setEditMode(Bool isEditMode);

		/// 是否是编辑器模式
		Bool		isEditMode();


	private:
		System() {}
		System(const System&) {}

		static System*	s_pSystem;

		LogLevel		_eLogLevel;


	private:

		// 插件接口
        Renderer*		_pRenderer;
        Parser*			_pParser;

		// 资源快速索引
		RBTree*			_pRBTreeTexture;
		RBTree*			_pRBTreePicture;

		// 窗体创建列表和销毁队列（消息派发用）
		Queue*			_pQueueCreate;
		Queue*			_pQueueDestroy;

		// 根窗体，当前焦点窗体
		Window*			_pWndRoot;
		Window*			_pWndFocus;
		// 鼠标拾取窗体
		Window*			_pWndMouse;
		Point			_ptWndMousePos;		// 鼠标拾取窗体区域坐标
		Point			_ptScrMousePos;		// 鼠标屏幕坐标
		// 鼠标点击移动窗体
		Point			_ptWndDownPos;		// 鼠标点击时，保存窗体坐标
		Point			_ptWndMouseDownPos;	// 鼠标点击时，保存拾取窗体的坐标位置
		Point			_ptScrMouseDownPos;	// 鼠标点击时，保存屏幕坐标

		// 鼠标拖曳窗体，拖曳渲染用画布
		Window*			_pWndDrag;
		Canvas*			_pDragCanvas;

		UInt			_uCtrlKeyState;
		Bool			_isEditMode;

		// 临时字体
		Font*			_pFont;
		FontTexture*	_pFontTexture;

		// 调试控制台
		String			_strDebug;
	};

}


#endif	//	__CrabGuiSystem_h__
