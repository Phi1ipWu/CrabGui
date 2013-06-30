
#ifndef __CrabGuiScrollBar_h__
#define __CrabGuiScrollBar_h__


#include "CrabGuiWindow.h"

//
//	Range:	Min: 0	Max: DocumentSize - PageSize
//

namespace CrabGui
{

	/// 滚动条（组合窗体）
	class CRABGUIEXPORT ScrollBar : public Window
	{
	public:
		ScrollBar(System* pSystem, UInt uID);
		virtual ~ScrollBar();

		static PCStr	TypeName;

		/// 获得类型名
		virtual PCStr	getTypeName();


	public:	// 渲染相关

		/// 逝去时间
		virtual Bool	update(UInt uElapse);


	public:	// 事件相关

		/// 事件响应函数
		virtual Bool	eventHandle(const EventDesc* pEvent);


	protected:	// 窗体父子层级相关

		/// 添加子窗体
		virtual Bool	_insertChild(Window* pWndChild);

		/// 移除子窗体
		virtual Bool	_removeChild(Window* pWndChild);


	public:	// 滚动条响应相关（仅滚动条响应用）

		/// 父窗体垂直滚动设置事件
		virtual Bool	onParentSetVertScroll(const ScrollInfo& stScroll);

		/// 父窗体水平滚动设置事件
		virtual Bool	onParentSetHorzScroll(const ScrollInfo& stScroll);


	public:	// 位属性相关

		enum ScrollBarBitState
		{
			SBS_Vertical	= 24,
			SBS_Manual,
		};

		/// 设置垂直滚动条
		virtual void	setVertical(Bool isVertical);

		/// 是否垂直滚动条
		virtual Bool	isVertical() const;

		/// 设置手动控制
		virtual void	setManual(Bool isManual);

		/// 是否手动控制
		virtual Bool	isManual();


	public:	// 提供给编辑器的属性操作

		/// 获得属性个数
		virtual UInt	getNumPropert();

		/// 设置窗体属性
		virtual Bool	setPropert(const String& strKey, const String& strValue);

		/// 获得窗体属性
		virtual Bool	getPropert(UInt uIndex, String& strKey, String& strValue);


	private: // 内部函数

		void	_updateBar();
		void	_updateParent();


	private:

		Window*		_pWndUp;
		Window*		_pWndBar;
		Window*		_pWndDown;

		// 滚动信息
		ScrollInfo	_stSI;

		// 鼠标点击移动滑块
		Point		_ptBarDownPos;		// 鼠标点击时，保存窗体坐标
		Point		_ptScrMouseDownPos;	// 鼠标点击时，保存屏幕坐标


	private:	// 编辑器属性

		String		_strPicFile;		// 图片文件名（底图）
		AlphaOp		_ePicAlphaOp;		// 图片半透混合方式
		Rect		_rcPicRect;			// 四态图片矩形
		Rect		_rcPicGrid;			// 四态图片九宫格中心矩形
		Rect		_rcPicWindow;		// 图片在窗体区域

		String		_strNameUp;			// 向上滚动
		String		_strNameBar;		// 滚动滑块
		String		_strNameDown;		// 向下滚动
	};

}


#endif	//	__CrabGuiScrollBar_h__
