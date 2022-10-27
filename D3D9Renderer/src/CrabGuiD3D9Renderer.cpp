
#include "CrabGuiDefine.h"
#include "CrabGuiD3D9Renderer.h"

#include "CrabGuiD3D9Texture.h"
#include "CrabGuiD3D9RenderTarget.h"


namespace CrabGui
{

	D3D9Renderer::D3D9Renderer(IDirect3DDevice9* pDev)
		:	_pDev(pDev)
	{
		_pDev->AddRef();
	}

	D3D9Renderer::~D3D9Renderer()
	{
		CrabRelease(_pDev);
	}


	// 开始渲染
	Bool D3D9Renderer::beginRender()
	{
		// 不使用 Shader
		_pDev->SetVertexShader(0);
		_pDev->SetPixelShader(0);

		// 禁用Z缓冲测试与写入，实心填充，关闭雾，关闭灯光
		_pDev->SetRenderState(D3DRS_ZENABLE,				D3DZB_FALSE);
		_pDev->SetRenderState(D3DRS_ZWRITEENABLE,			FALSE);
		_pDev->SetRenderState(D3DRS_FILLMODE,				D3DFILL_SOLID);
		_pDev->SetRenderState(D3DRS_ALPHATESTENABLE,		FALSE);
		_pDev->SetRenderState(D3DRS_FOGENABLE,				FALSE);
		_pDev->SetRenderState(D3DRS_CULLMODE,				D3DCULL_NONE);//D3DCULL_CCW);

		// 纹理使用夹持模式，防止左右边缘穿帮
		_pDev->SetSamplerState(0, D3DSAMP_ADDRESSU,			D3DTADDRESS_CLAMP);
		_pDev->SetSamplerState(0, D3DSAMP_ADDRESSV,			D3DTADDRESS_CLAMP);

		// 颜色相乘
		_pDev->SetTextureStageState(0, D3DTSS_COLORARG1,	D3DTA_TEXTURE);
		_pDev->SetTextureStageState(0, D3DTSS_COLORARG2,	D3DTA_DIFFUSE);
		_pDev->SetTextureStageState(0, D3DTSS_COLOROP,		D3DTOP_MODULATE);

		// Alpha值相乘
		_pDev->SetTextureStageState(0, D3DTSS_ALPHAARG1,	D3DTA_TEXTURE);
		_pDev->SetTextureStageState(0, D3DTSS_ALPHAARG2,	D3DTA_DIFFUSE);
		_pDev->SetTextureStageState(0, D3DTSS_ALPHAOP,		D3DTOP_MODULATE);

		// 双线过滤
		//_pDev->SetSamplerState(0, D3DSAMP_MINFILTER,		D3DTEXF_LINEAR);
		//_pDev->SetSamplerState(0, D3DSAMP_MAGFILTER,		D3DTEXF_LINEAR);
		// 不插值
		_pDev->SetSamplerState(0, D3DSAMP_MINFILTER,		D3DTEXF_POINT);
		_pDev->SetSamplerState(0, D3DSAMP_MAGFILTER,		D3DTEXF_POINT);

		// 不使用颜色混合
		_pDev->SetTextureStageState(1, D3DTSS_COLOROP,		D3DTOP_DISABLE);

		// 场景半透渲染
		_pDev->SetRenderState(D3DRS_ALPHABLENDENABLE,		TRUE);
		_pDev->SetRenderState(D3DRS_SRCBLEND,				D3DBLEND_SRCALPHA);
		_pDev->SetRenderState(D3DRS_DESTBLEND,				D3DBLEND_INVSRCALPHA);

		return True;
	}


	// 结束渲染
	void D3D9Renderer::endRender()
	{
		setScissorRect(False, Rect());
	}


	// 设置裁减矩形
	Bool D3D9Renderer::setScissorRect(Bool isEnable, const Rect& rcScissor)
	{
		HRESULT hr = _pDev->SetRenderState(D3DRS_SCISSORTESTENABLE, isEnable ? TRUE : FALSE);

		if (isEnable)
		{
			RECT rc;
			rc.left		= rcScissor.x1;
			rc.top		= rcScissor.y1;
			rc.right	= rcScissor.x2;
			rc.bottom	= rcScissor.y2;
			hr = _pDev->SetScissorRect(&rc);

			_rcScissor = rcScissor;
		}
		else
		{
			_rcScissor.setZero();
		}

		return True;
	}


	// 矩形渲染
	Bool D3D9Renderer::renderTexture(Texture* pTexture, Color cDiffuse, const Rect& rcTex, const Rect& rcScreen)
	{
		if (pTexture)
		{
			// 带贴图渲染
			IDirect3DTexture9* pTex = ((D3D9Texture*)pTexture)->_pTex;
			return _render(pTex, pTexture->getSize(), cDiffuse, rcTex, rcScreen);
		}
		else
		{
			// 纯色渲染
			Point ptTex(16, 16);
			return _render(0, ptTex, cDiffuse, rcTex, rcScreen);
		}
	}


	// 矩形渲染（渲染目标）
	Bool D3D9Renderer::renderRenderTarget(RenderTarget* pRT, Color cDiffuse, const Rect& rcTex, const Rect& rcScreen)
	{
		IDirect3DTexture9* pTex = ((D3D9RenderTarget*)pRT)->_pTex;
		return _render(pTex, pRT->getRenderSize(), cDiffuse, rcTex, rcScreen);
	}


	// 自定义顶点渲染
	Bool D3D9Renderer::renderRenderTargetVertices(RenderTarget* pRT, Int nPointSize, const PointReal* pPoints, const Color* pDiffuses, const PointReal* pTexPoints, Int nTriangleSize, const Int16* pTriangles)
	{
		IDirect3DTexture9* pTex = ((D3D9RenderTarget*)pRT)->_pTex;

		struct Vertex2D
		{
			float   x, y, z;
			Color	diffuse;
			float   u, v;
		};
		static const UInt FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;

		// DX 的 UV 坐标修正
		PointReal uv_add = { 0.5f / pRT->getRenderSize().x, 0.5f / pRT->getRenderSize().y };

		Vertex2D vtx[512] = {0};
		int nVertexSize = min(nPointSize, sizeof(vtx) / sizeof(vtx[0]));
		for (int i = 0; i < nVertexSize; ++i)
		{
			// 顶点信息
			vtx[i].x = pPoints[i].x;
			vtx[i].y = pPoints[i].y;
			vtx[i].z = 1.0f;

			// 顶点色
			vtx[i].diffuse = pDiffuses ? pDiffuses[i] : 0xFFFFFFFF;

			// 纹理
			vtx[i].u = (float)pTexPoints[i].x + uv_add.x;
			vtx[i].v = (float)pTexPoints[i].y + uv_add.y;
		}

		// 渲染
		_pDev->SetFVF(FVF);
		_pDev->SetTexture(0, pTex);

		HRESULT hr = _pDev->DrawIndexedPrimitiveUP(	D3DPT_TRIANGLELIST,
													0,
													nPointSize,			// 顶点数
													nTriangleSize / 3,	// 索引三角面数
													pTriangles,
													D3DFMT_INDEX16,
													vtx,
													sizeof(Vertex2D));

		return SUCCEEDED(hr);
	}


	// 渲染实现
	Bool D3D9Renderer::_render(IDirect3DTexture9* pTex, const Point& ptTexSize, Color cDiffuse, const Rect& rcTex, const Rect& rcScreen)
	{
		struct Vertex2D
		{
			float   x, y, z;
			Color	diffuse;
			float   u, v;
		};

		static const UInt FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;

		Vertex2D vtx[4] = {0};
		vtx[0].z = vtx[1].z = vtx[2].z = vtx[3].z = 1.0f;

		// 顶点信息
		vtx[0].x = (float)rcScreen.x1;
		vtx[0].y = (float)rcScreen.y1;
		vtx[1].x = (float)rcScreen.x2;
		vtx[1].y = (float)rcScreen.y1;
		vtx[2].x = (float)rcScreen.x1;
		vtx[2].y = (float)rcScreen.y2;
		vtx[3].x = (float)rcScreen.x2;
		vtx[3].y = (float)rcScreen.y2;

		// 顶点色
		vtx[0].diffuse = cDiffuse;
		vtx[1].diffuse = cDiffuse;
		vtx[2].diffuse = cDiffuse;
		vtx[3].diffuse = cDiffuse;

		// 纹理
		float rTexWidth  = (float)ptTexSize.x;
		float rTexHeight = (float)ptTexSize.y;

		// DX 的 UV 坐标修正
		float u_add = 0.5f / rTexWidth;
		float v_add = 0.5f / rTexHeight;

		vtx[0].u = (float)rcTex.x1 / rTexWidth  + u_add;
		vtx[0].v = (float)rcTex.y1 / rTexHeight + v_add;
		vtx[1].u = (float)rcTex.x2 / rTexWidth  + u_add;
		vtx[1].v = (float)rcTex.y1 / rTexHeight + v_add;
		vtx[2].u = (float)rcTex.x1 / rTexWidth  + u_add;
		vtx[2].v = (float)rcTex.y2 / rTexHeight + v_add;
		vtx[3].u = (float)rcTex.x2 / rTexWidth  + u_add;
		vtx[3].v = (float)rcTex.y2 / rTexHeight + v_add;

		// 渲染
		_pDev->SetFVF(FVF);
		_pDev->SetTexture(0, pTex);

		//static UInt16 idx[6] = { 0, 1, 2, 2, 1, 3 };
		//HRESULT hr = _pD3D9Device->DrawIndexedPrimitiveUP(	D3DPT_TRIANGLELIST,
		//													0,
		//													4,			// 顶点数
		//													6 / 3,		// 索引三角面数
		//													idx,
		//													D3DFMT_INDEX16,
		//													vtx,
		//													sizeof(Vertex2D));

		HRESULT hr = _pDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP,
											2,
											vtx,
											sizeof(Vertex2D));

		return SUCCEEDED(hr);
	}


	///////////////////////////////////////////////////////////////////////////////////////////////


	// 创建渲染目标
	RenderTarget* D3D9Renderer::createRenderTarget()
	{
		D3D9RenderTarget* pRT = CrabNew(D3D9RenderTarget)(_pDev);

		return pRT;
	}


	// 销毁渲染目标
	void D3D9Renderer::destroyRenderTarget(RenderTarget* pRT)
	{
		CrabDelete(pRT);
	}


	// 创建纹理接口
	Texture* D3D9Renderer::createTexture()
	{
		D3D9Texture* pTexture = CrabNew(D3D9Texture)(_pDev);

		return pTexture;
	}


	// 销毁纹理接口
	void D3D9Renderer::destroyTexture(Texture* pTexture)
	{
		CrabDelete(pTexture);
	}

}
