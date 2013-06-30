
#ifndef __CrabGuiScrollWindow_h__
#define __CrabGuiScrollWindow_h__


#include "CrabGuiWindow.h"


namespace CrabGui
{

	/// 滚动窗体（组合窗体：逻辑层在上面创建子窗体）
	class CRABGUIEXPORT ScrollWindow : public Window
	{
	public:
		ScrollWindow(System* pSystem, UInt uID);
		virtual ~ScrollWindow();

		static PCStr	TypeName;

		/// 获得类型名
		virtual PCStr	getTypeName();


	public:	// 渲染相关



	public:	// 滚动条相关



	public:	// 提供给编辑器的属性操作

		/// 获得属性个数
		virtual UInt	getNumPropert();

		/// 设置窗体属性
		virtual Bool	setPropert(const String& strKey, const String& strValue);

		/// 获得窗体属性
		virtual Bool	getPropert(UInt uIndex, String& strKey, String& strValue);


	private:

	};

}


#endif	//	__CrabGuiScrollWindow_h__
