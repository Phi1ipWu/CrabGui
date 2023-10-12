
#ifndef __CrabGuiSample_FirstWindow_h__
#define __CrabGuiSample_FirstWindow_h__


#include "CrabGuiSample.h"
#include "CrabGuiEventHandle.h"


namespace CrabGui
{

	class SampleFirstWindow : public Sample, public EventHandle
	{
	public:
		SampleFirstWindow();
		virtual ~SampleFirstWindow();

		/// 逻辑层的初始化
		virtual Bool	init();

		/// 逻辑层的销毁
		virtual void	cleanup();

		/// 逻辑层的每帧更新
		virtual void	update();


	public:

		/// 响应事件
		virtual Bool eventHandle(const EventDesc* pEvent);


	private:

		void	_testManualCreate();
		void	_testRBTree();
		void	_testConvertPNG();
		void	_testClipPNG();
		void	_testBatchConvertJPG();
		void	_testSaveFontTexture();
		void	_testPixelReplace();


	private:

		FrameWindow*	_frmWnd1;
		Static*			_stcImg;
		Button*			_btn[4];

		FrameWindow*	_frmWnd2;
		Editbox*		_edtText;

		Listbox*		_lstItem;

	};

}


#endif	//	__CrabGuiSample_FirstWindow_h__
