
#ifndef __CrabGuiCombobox_h__
#define __CrabGuiCombobox_h__


#include "CrabGuiWindow.h"


namespace CrabGui
{

	/// 组合框（组合窗体）
	class CRABGUIEXPORT Combobox : public Window
	{
	public:
		Combobox(System* pSystem, UInt uID);
		virtual ~Combobox();

		static PCStr	TypeName;

		/// 获得类型名
		virtual PCStr	getTypeName();


	public:	// 事件相关

		/// 事件响应函数
		virtual Bool	eventHandle(const EventDesc* pEvent);


	protected:	// 窗体父子层级相关

		/// 添加子窗体
		virtual Bool	_insertChild(Window* pWndChild);

		/// 移除子窗体
		virtual Bool	_removeChild(Window* pWndChild);


	//public:	// 提供给编辑器的属性操作

	//	/// 获得属性个数
	//	virtual UInt	getNumPropert();

	//	/// 设置窗体属性
	//	virtual Bool	setPropert(const String& strKey, const String& strValue);

	//	/// 获得窗体属性
	//	virtual Bool	getPropert(UInt uIndex, String& strKey, String& strValue);


	private:

		Window*		_pWndEditbox;
		Window*		_pWndButton;
		Window*		_pWndDropList;

	};

}


#endif	//	__CrabGuiCombobox_h__
