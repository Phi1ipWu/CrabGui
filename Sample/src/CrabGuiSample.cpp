
#include "CrabGui.h"
#include "CrabGuiSample.h"

#include "CrabGuiD3D9Renderer.h"
#include "CrabGuiFreeParser.h"

#include <windowsx.h>
#include <io.h>
#include <locale.h>


namespace CrabGui
{

	// MessageBox 简化宏
	#define ErrorMsgBox(p)  MessageBox(0, (p), TEXT("CrabGuiSample"), MB_ICONERROR);
	#define QuestionBox(p)  MessageBox(0, (p), TEXT("CrabGuiSample"), MB_ICONQUESTION | MB_YESNO) == IDYES;


	Sample* Sample::ms_pSample = 0;


	Sample::Sample()
		:	_hWnd(0)
		,	_pD3dObject(0), _pD3dDevice(0)
		,	_pSystem(0), _pRenderer(0), _pParser(0)
	{
		ms_pSample = this;
	}

	Sample::~Sample()
	{
		ms_pSample = 0;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////


	// 运行样例
	UInt Sample::run(const Point& ptWndSize, Bool isWindowed)
	{
		if(0 != _access("CrabGui_Core.dll", 0) || 0 != _access("CrabGui_Core_d.dll", 0))
		{
			SetCurrentDirectory("../../Output/");
		}

		_ptWndSize	= ptWndSize;
		_isWindowed	= isWindowed;

		if (!_initWindow())	return 0;
		if (!_initDX9())	return 0;
		if (!_initGui())	return 0;
		if (!init())		return 0;

		_loop();

		cleanup();
		_cleanupGui();
		_cleanupDX9();
		_cleanupWindow();

		return 0;
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////


	// 窗体大小变化
	void Sample::resize(Int16 x, Int16 y)
	{
		if (0 == _pD3dDevice)
			return;

		_stPresParams.BackBufferWidth  = _ptWndSize.x = x;
		_stPresParams.BackBufferHeight = _ptWndSize.y = y;

		HRESULT hr = _pD3dDevice->Reset(&_stPresParams);
		_pSystem->injectResize(_ptWndSize);
	}


	//////////////////////////////////////////////////////////////////////////////////////////////////


	Bool Sample::_initWindow()
	{
		// 注册窗体类
		WNDCLASSEX wcex;
		wcex.cbSize         = sizeof(WNDCLASSEX);
		wcex.style          = 0;
		wcex.lpfnWndProc    = (WNDPROC)WndProc;
		wcex.cbClsExtra     = 0;
		wcex.cbWndExtra     = 0;
		wcex.hInstance      = (HINSTANCE) GetModuleHandle(0);
		wcex.hIcon          = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
		wcex.hCursor        = LoadCursor(NULL, IDC_ARROW);
		wcex.hbrBackground  = 0;
		wcex.lpszMenuName   = 0;
		wcex.lpszClassName  = TEXT("CrabGuiSample");
		wcex.hIconSm        = 0;

		if(!RegisterClassEx(&wcex))
			return False;

		// 创建窗体
		_hWnd = CreateWindow(TEXT("CrabGuiSample"), TEXT("CrabGuiSample"), WS_OVERLAPPEDWINDOW,
		  CW_USEDEFAULT,CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, wcex.hInstance, NULL);

		if (0 == _hWnd)
			return False;

		// 设置客户区大小
		RECT  rcWork;
		RECT  rc;
		DWORD dwStyle;

		dwStyle  = GetWindowStyle(_hWnd);
		dwStyle &= ~WS_POPUP;
		dwStyle |= WS_OVERLAPPED | WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX;
		SetWindowLong(_hWnd, GWL_STYLE, dwStyle);

		SetRect(&rc, 0, 0, _ptWndSize.x, _ptWndSize.y);

		AdjustWindowRectEx(&rc, GetWindowStyle(_hWnd), GetMenu(_hWnd)!= NULL,
							GetWindowExStyle(_hWnd));

		SetWindowPos(_hWnd, NULL, 0, 0, rc.right-rc.left, rc.bottom-rc.top,
					  SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

		SetWindowPos(_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0,
					  SWP_NOSIZE | SWP_NOMOVE | SWP_NOACTIVATE);

		SystemParametersInfo(SPI_GETWORKAREA, 0, &rcWork, 0);
		GetWindowRect(_hWnd, &rc);
		if(rc.left < rcWork.left)rc.left = rcWork.left;
		if(rc.top  < rcWork.top) rc.top  = rcWork.top;
		SetWindowPos(_hWnd, NULL, rc.left, rc.top, 0, 0,
					  SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);

		return True;
	}


	void Sample::_cleanupWindow()
	{
		UnregisterClass(TEXT("CrabGuiSample"), GetModuleHandle(0));
	}


	Bool Sample::_initDX9()
	{
		// 创建 D3D 对象
		_pD3dObject = Direct3DCreate9(D3D_SDK_VERSION);
		if (0 == _pD3dObject)
		{
			ErrorMsgBox(TEXT("Create Main D3D Object Failed. "));
			return False;
		}

		// 填入表现形式参数，窗体模式
		memset(&_stPresParams, 0, sizeof(D3DPRESENT_PARAMETERS));

		HRESULT hr;

		if (_isWindowed)
		{
			// 窗体模式
			_stPresParams.Windowed         = TRUE;
			_stPresParams.hDeviceWindow    = _hWnd;
			_stPresParams.BackBufferWidth  = _ptWndSize.x;
			_stPresParams.BackBufferHeight = _ptWndSize.y;

			_stPresParams.SwapEffect       = D3DSWAPEFFECT_COPY;//D3DSWAPEFFECT_COPY;//D3DSWAPEFFECT_FLIP;//D3DSWAPEFFECT_DISCARD;
			_stPresParams.BackBufferFormat = D3DFMT_UNKNOWN;
			_stPresParams.BackBufferCount  = 1;
			_stPresParams.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;//D3DPRESENT_INTERVAL_IMMEDIATE;//D3DPRESENT_INTERVAL_DEFAULT;
		}
		else
		{
			// 获取当前Windows桌面的显示模式，保存到定义的d3ddm结构中
			D3DDISPLAYMODE d3ddm;
			memset(&d3ddm, 0, sizeof(D3DDISPLAYMODE));

			hr = _pD3dObject->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
			if(FAILED(hr))
			{
				ErrorMsgBox(TEXT("Get Adapter Display Mode Failed. "));
				return False;
			}

			// 全屏模式
			_stPresParams.Windowed         = FALSE;
			_stPresParams.hDeviceWindow    = _hWnd;
			_stPresParams.SwapEffect       = D3DSWAPEFFECT_DISCARD;
			_stPresParams.BackBufferWidth  = d3ddm.Width;
			_stPresParams.BackBufferHeight = d3ddm.Height;
			_stPresParams.BackBufferFormat = d3ddm.Format;
			_stPresParams.BackBufferCount  = 1;
			_stPresParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;//使用当前设定的此显示模式下的刷新率
		}

		// 创建 D3D 设备
		hr = _pD3dObject->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL, _hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING, &_stPresParams, &_pD3dDevice);
		if (FAILED(hr))
		{
			// 可能一些显卡不支持硬件顶点处理。因此尝试软件方式
			hr = _pD3dObject->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL, _hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING, &_stPresParams, &_pD3dDevice);
			if (FAILED(hr))
			{
				// 软件方式还是失败 -_-
				ErrorMsgBox(TEXT("Create Device Failed. "));
				return False;
			}
		}

		return True;
	}


	void Sample::_cleanupDX9()
	{
		CrabRelease(_pD3dDevice);
		CrabRelease(_pD3dObject);
	}


	Bool Sample::_initGui()
	{
		_pRenderer	= new CrabGui::D3D9Renderer(_pD3dDevice);
		_pParser	= new CrabGui::FreeParser();

		_pSystem	= new CrabGui::System(_pRenderer, _pParser, "chs");

		// 创建默认字体（临时）
		_pSystem->createFont("C:/Windows/Fonts/MSYH.ttf");

		// 根窗体
		_wndRoot	= _pSystem->createWindow("Window");

		_wndRoot->setName("wndRoot");
		_wndRoot->setPos(Point(0, 0));
		_wndRoot->setSize(_ptWndSize);
		_wndRoot->setVisible(True);
		_wndRoot->setMouseToParent(True);
		_wndRoot->setKeyToParent(True);

		_pSystem->setRootWindow(_wndRoot);

		return True;
	}


	void Sample::_cleanupGui()
	{
		delete _pSystem;
		delete _pRenderer;
		delete _pParser;
	}


	Bool Sample::_beginRender()
	{
		HRESULT hr = _pD3dDevice->TestCooperativeLevel();

		if (FAILED(hr))
		{
			if (D3DERR_DEVICENOTRESET == hr)
			{
				for (UInt i = 0; i < _pSystem->getRootWindow()->getNumChild(); ++i)
				{
					CrabGui::Window* pWnd = _pSystem->getRootWindow()->getChild(i);
				}

				hr = _pD3dDevice->Reset(&_stPresParams);
			}
		}

		// 清除后台缓冲
		hr = _pD3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
		if(FAILED(hr))
		{
			return False;
		}

		// 关闭灯光
		_pD3dDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
		// 打开深度测试
		_pD3dDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
		// 设置渲染状态
		_pD3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		// 双线过滤
		_pD3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		_pD3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		// 三线过滤
		_pD3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		// 设置矩阵
		D3DXMATRIX mat;
		D3DXVECTOR3 vLookAt(0, 0, 0);
		D3DXVECTOR3 vEye(0, 0, -1);
		D3DXVECTOR3 vUp(0, 1, 0);
		D3DXMatrixIdentity(&mat);
		_pD3dDevice->SetTransform(D3DTS_WORLD, &mat);
		D3DXMatrixLookAtLH(&mat, &vEye, &vLookAt, &vUp);
		_pD3dDevice->SetTransform(D3DTS_VIEW, &mat);
		D3DXMatrixOrthoOffCenterLH(&mat, 0.0f, _ptWndSize.x, _ptWndSize.y, 0.0f, 0.0005f, 1000.0f);
		_pD3dDevice->SetTransform(D3DTS_PROJECTION, &mat);

		// 设置材质
		D3DMATERIAL9 mtrl;
		memset(&mtrl, 0, sizeof(D3DMATERIAL9));
		mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
		mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
		mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
		mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
		_pD3dDevice->SetMaterial(&mtrl);
		hr = _pD3dDevice->BeginScene();

		return SUCCEEDED(hr);
	}


	void Sample::_endRender()
	{
        // 完成场景，刷新
        _pD3dDevice->EndScene();
        _pD3dDevice->Present(0, 0, 0, 0);
	}


	void Sample::_loop()
	{
		// 显示窗体
		ShowWindow(_hWnd, SW_SHOW);
		UpdateWindow(_hWnd);

		DWORD dwTick = timeGetTime();

		// 消息循环
		MSG msg;
		ZeroMemory(&msg, sizeof(msg));
		while(msg.message != WM_QUIT)
		{
			if(PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			update();
			_pSystem->injectTimeElapse(timeGetTime() - dwTick);
			dwTick = timeGetTime();

			if(_beginRender())
			{
				// 渲染GUI
				_pSystem->renderGui();

				_endRender();
			}
		}
	}


	LRESULT CALLBACK Sample::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_MOUSEMOVE:
			ms_pSample->_pSystem->injectMousePosition(Point((Int16) LOWORD(lParam), (Int16) HIWORD(lParam)));
			break;

		case WM_LBUTTONDOWN:
			ms_pSample->_pSystem->injectMouseKeyDown(CrabGui::MB_Left);
			break;

		case WM_RBUTTONDOWN:
			ms_pSample->_pSystem->injectMouseKeyDown(CrabGui::MB_Right);
			break;

		case WM_MBUTTONDOWN:
			ms_pSample->_pSystem->injectMouseKeyDown(CrabGui::MB_Middle);
			break;

		case WM_LBUTTONUP:
			ms_pSample->_pSystem->injectMouseKeyUp(CrabGui::MB_Left);
			break;

		case WM_RBUTTONUP:
			ms_pSample->_pSystem->injectMouseKeyUp(CrabGui::MB_Right);
			break;

		case WM_MBUTTONUP:
			ms_pSample->_pSystem->injectMouseKeyUp(CrabGui::MB_Middle);
			break;

		case WM_KEYDOWN:
		case WM_SYSKEYDOWN:
			ms_pSample->_pSystem->injectKeyDown((CrabGui::UInt) wParam);

			if(VK_ESCAPE == wParam)
				PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;

		case WM_KEYUP:
		case WM_SYSKEYUP:
			ms_pSample->_pSystem->injectKeyUp((CrabGui::UInt) wParam);
			break;

		case WM_CHAR:
		case WM_SYSCHAR:
			if (wParam >= 32)
				ms_pSample->_pSystem->injectKeyChar((CrabGui::UInt) wParam);
			break;

		case WM_MOUSEWHEEL:
			ms_pSample->_pSystem->injectMouseWheel(Point(LOWORD(wParam), HIWORD(wParam)));
			break;


		case WM_SIZE:
			ms_pSample->resize(LOWORD(lParam), HIWORD(lParam));
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}

}
