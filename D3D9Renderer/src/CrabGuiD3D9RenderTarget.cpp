
#include "CrabGuiDefine.h"
#include "CrabGuiD3D9RenderTarget.h"


//	http://blog.csdn.net/yacper/article/details/5214821


namespace CrabGui
{

	D3D9RenderTarget::D3D9RenderTarget(IDirect3DDevice9* pDev)
		:	_pDev(pDev)
		,	_pTex(0), _pBackSuf(0)
	{
		_pDev->AddRef();
	}

	D3D9RenderTarget::~D3D9RenderTarget()
	{
		CrabRelease(_pTex);
		CrabRelease(_pDev);
	}


	// 设置渲染目标大小
	Bool D3D9RenderTarget::setRenderSize(const Point& ptSize)
	{
		if (ptSize == _ptSize)
			return True;

		CrabRelease(_pTex);
		_ptSize.setZero();

		if (ptSize.isZero())
			return True;

		// 创建渲染到目标的纹理
		HRESULT hr = _pDev->CreateTexture(	ptSize.x,
											ptSize.y,
											1,
											D3DUSAGE_RENDERTARGET,
											D3DFMT_A8R8G8B8,
											D3DPOOL_DEFAULT,
											&_pTex,
											0);

		if (FAILED(hr))
			return False;

		_ptSize = ptSize;

		return True;
	}


	// 获得渲染目标大小
	const Point& D3D9RenderTarget::getRenderSize()
	{
		return _ptSize;
	}


	// 绑定渲染目标（必须与 unbind 成对使用）
	Bool D3D9RenderTarget::bind(const Rect& rcDirty, Color cClear)
	{
		IDirect3DSurface9* pTexSuf = 0;
		_pTex->GetSurfaceLevel(0, &pTexSuf);

		_pDev->GetRenderTarget(0, &_pBackSuf);
		_pDev->SetRenderTarget(0, pTexSuf);

		CrabRelease(pTexSuf);

		_pDev->GetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)_matProj);

		D3DXMATRIX mat;
		D3DXMatrixOrthoOffCenterLH(&mat, 0.0f, _ptSize.x, _ptSize.y, 0.0f, 0.0005f, 1000.0f);
		_pDev->SetTransform(D3DTS_PROJECTION, &mat);

		D3DRECT rc;
		rc.x1 = rcDirty.x1;
		rc.x2 = rcDirty.x2;
		rc.y1 = rcDirty.y1;
		rc.y2 = rcDirty.y2;
		HRESULT hr = _pDev->Clear(0, &rc, D3DCLEAR_TARGET, cClear, 1.0f, 0);

		return True;
	}


	// 取消绑定渲染目标
	void D3D9RenderTarget::unbind()
	{
		_pDev->SetRenderTarget(0, _pBackSuf);
		CrabRelease(_pBackSuf);

		_pDev->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)_matProj);
	}

}
