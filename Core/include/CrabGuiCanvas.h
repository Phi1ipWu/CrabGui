
#ifndef __CrabGuiCanvas_h__
#define __CrabGuiCanvas_h__


namespace CrabGui
{

	class System;
	class Texture;
	class RenderTarget;
	class Queue;
	class Font;


	/// 画布
	class CRABGUIEXPORT Canvas
	{
	public:
		Canvas();
		virtual ~Canvas(void);


	public:	// 渲染相关

		/// 渲染（可以传入效果）
		void		render(const Point& ptPos, void* pFadeout);


	public:	// 矩形裁减相关

		/// 合并脏矩型
		void		mergeDirtyRect(const Rect& rcDirty);

		/// 获得脏矩型
		const Rect& getDirtyRect() const;

		/// 脏矩形清空
		void		setDirtyRectZero();


	public:	// 画布属性相关

		/// 重设大小
		void	setSize(Bool isVisible, Point ptSize);

		/// 绑定渲染目标
		void	bind();

		/// 解绑渲染目标
		void	unbind();


	public:	// 纯色渲染

		/// 画线
		static void lineTo(const Point& ptPos1, const Point ptPos2, Color cLine, AlphaOp eAlphaOp);

		/// 颜色填充
		static void	fillRect(const Rect& rcCanvas, Color cRect, AlphaOp eAlphaOp);


	public:	// 图片渲染

		/// 简单图片渲染
		static void	drawPic(const String& strName, const Rect& rcPic, const Rect& rcCanvas, Color cPic, AlphaOp eAlphaOp);

		/// 九宫格图片渲染
		static void	drawGridPic(const String& strName, const Rect& rcPic, const Rect& rcGrid, const Rect& rcCanvas, Color cPic, AlphaOp eAlphaOp);


	public:	// 字体渲染

		enum TextFormatBit
		{
			TFB_TextWrap	= (1 << 0),		// 自动换行
			TFB_Translate	= (1 << 1),		// 解释转义符
		};

		/// 简单文字渲染
		static void	drawChar(const String& strFontName, UInt uFontSize, UInt uCharCode, Point ptPos, Color cText, AlphaOp eAlphaOp);

		/// 字符串文字渲染
		static void	drawText(const String& strFontName, UInt uFontSize, String& strText, const Rect& rcText, UInt uLineHeight, AlignType eAlignType, UInt uTextFormat, Color cText, Color cShadow, AlphaOp eAlphaOp);

		/// 计算每个文字的显示位置（根据宽度，pTextRect 返回字符串文字的宽度和高度，pColRowSize最大返回行数和列数）
		static void	calcTextPixelSize(String& strText, Rect* pTextRect, Point* pColRowSize, 
			const String& strFontName, UInt uFontSize, UInt uLineHeight, AlignType eAlignType, const Point& ptFixSize, UInt uTextFormat);


	private:
		/// 将脏矩形区域刷到纹理上
		//void	_dataToTexture();


	private:
		Canvas(const Canvas&);
		Canvas& operator=(const Canvas&);

		RenderTarget*	_pRenderTarget;
		Rect			_rcDirty;	// 脏矩形，只更新脏矩形区域

		Point			_ptDivision;	// 矩形分割列
		Queue*			_pVerticesQueue;
		Queue*			_pTrianglesQueue;
		Queue*			_pColorQueue;
		Queue*			_pTexPointQueue;
	};

}

#endif	//	__CrabGuiCanvas_h__
