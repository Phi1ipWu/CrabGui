
#ifndef __CrabGuiSample_h__
#define __CrabGuiSample_h__


#define _WIN32_WINDOWS 0x500
#include <windows.h>

#include <d3d9.h>
#include <d3dx9tex.h>


namespace CrabGui
{

	class System;
	class D3D9Renderer;
	class FreeParser;


	class Sample
	{
	public:
		Sample();
		virtual ~Sample();

		static Sample*	ms_pSample;

		/// 逻辑层的初始化
		virtual Bool	init() = 0;

		/// 逻辑层的销毁
		virtual void	cleanup() = 0;

		/// 逻辑层的每帧更新
		virtual void	update() = 0;


	public:
		/// 运行样例
		UInt	run(const Point& ptWndSize, Bool isWindowed);


	public:
		/// 窗体大小变化
		void	resize(Int16 x, Int16 y);


	private:
		Bool	_initWindow();
		void	_cleanupWindow();

		Bool	_initDX9();
		void	_cleanupDX9();

		Bool	_initGui();
		void	_cleanupGui();

		Bool	_beginRender();
		void	_endRender();

		void	_loop();


		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


	protected:
		HWND		_hWnd;
		Point		_ptWndSize;
		Bool		_isWindowed;

		// DX 对象
		LPDIRECT3D9        _pD3dObject;     // D3D 对象
		LPDIRECT3DDEVICE9  _pD3dDevice;     // D3D 设备
		D3DPRESENT_PARAMETERS _stPresParams;

		// UI 对象
		System*			_pSystem;
		D3D9Renderer*	_pRenderer;
		FreeParser*		_pParser;

		Window*			_wndRoot;
	};

}


#endif	//	__CrabGuiSample_h__
