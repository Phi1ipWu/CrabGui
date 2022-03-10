
#include "CrabGuiDefine.h"
#include "CrabGuiD3D9Texture.h"


namespace CrabGui
{

	D3D9Texture::D3D9Texture(IDirect3DDevice9* pDev)
		:	_pDev(pDev)
		,	_pTex(0)
	{
		_pDev->AddRef();
	}

	D3D9Texture::~D3D9Texture()
	{
		CrabRelease(_pTex);
		CrabRelease(_pDev);
	}


	// 从文件中载入图片
	Bool D3D9Texture::loadFromFile(PCStr pszFileName)
	{
		CrabRelease(_pTex);
		_ptSize.setZero();

		D3DXIMAGE_INFO imgInfo;
		HRESULT hr = D3DXCreateTextureFromFileEx(
				_pDev,
				pszFileName,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				1,
				0,
				D3DFMT_UNKNOWN,
				D3DPOOL_DEFAULT,
				D3DX_FILTER_NONE,
				D3DX_FILTER_NONE,
				0,
				&imgInfo,
				NULL,
				&_pTex);

		if (FAILED(hr))
		{
			return False;
		}

		//static bool isFirst = true;
		//if (isFirst)
		//{
		//	D3DXSaveTextureToFile("C:\\text.png", D3DXIFF_PNG, _pTex, 0);
		//	isFirst = false;
		//}

		_ptSize.x = (Int16)imgInfo.Width;
		_ptSize.y = (Int16)imgInfo.Height;

		return True;
	}


	// 从内存中载入图片
	Bool D3D9Texture::loadFromMemory(PCVoid pData, const Point& ptSize, UInt nComp)
	{
		CrabRelease(_pTex);
		_ptSize.setZero();
/*
		D3DXIMAGE_INFO imgInfo;
		HRESULT hr = D3DXCreateTextureFromFileInMemoryEx(
				_pDev,
				pData,
				nDataSize,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				1,
				0,
				D3DFMT_UNKNOWN,
				D3DPOOL_DEFAULT,
				D3DX_FILTER_NONE,
				D3DX_FILTER_NONE,
				0,
				&imgInfo,
				0,
				&_pTex);

		if (SUCCEEDED(hr))
		{
			return True;
		}
*/
		HRESULT hr = _pDev->CreateTexture(
				ptSize.x,
				ptSize.y,
				1,
				0,
				D3DFMT_UNKNOWN,
				D3DPOOL_DEFAULT,
				&_pTex,
				0);

		Color* pLockData = 0;
		UInt uPitch  = 0;
		UInt area = abs(ptSize.getArea());
		if (this->lock((void*&)pLockData, uPitch))
		{
			for (UInt i = 0; i < area; ++i)
			{
				pLockData[i] = ((UInt*)pData)[i];//(uAlpha << 24) | 0x00FFFFFF;
			}

			this->unlock();
		}

		_ptSize = ptSize;
		return True;
	}


	// 保存到文件
	Bool D3D9Texture::saveToFile(PCStr pszFileName)
	{
		if (_pTex)
		{
			HRESULT hr = D3DXSaveTextureToFile(pszFileName, D3DXIFF_PNG, _pTex, 0);

			if (SUCCEEDED(hr))
			{
				return True;
			}
		}

		return False;
	}


	// 手动设置大小
	Bool D3D9Texture::setSize(const Point& ptSize)
	{
		CrabRelease(_pTex);
		_ptSize.setZero();

		HRESULT hr = D3DXCreateTexture(	_pDev,
										ptSize.x,
										ptSize.y,
										1,
										0,
										D3DFMT_A8R8G8B8,
										D3DPOOL_MANAGED,
										&_pTex);

		if (SUCCEEDED(hr))
		{
			_ptSize = ptSize;
			return True;
		}

		return False;
	}


	// 获得纹理大小
	const Point& D3D9Texture::getSize()
	{
		return _ptSize;
	}


	// 获得像素格式
	PixelFmt D3D9Texture::getPixelFormat()
	{
		return _ePixelFmt;
	}


	// 从内存中锁定数据
	Bool D3D9Texture::lock(void*& pData, UInt& nPitch)
	{
        D3DLOCKED_RECT stLockedRect;
        HRESULT hr = _pTex->LockRect(	0,
										&stLockedRect,
										0,
										D3DLOCK_DISCARD);
		if(SUCCEEDED(hr))
		{
			pData  = stLockedRect.pBits;
			nPitch = stLockedRect.Pitch;
		}

		return SUCCEEDED(hr);
	}


	// 内存数据解锁
	Bool D3D9Texture::unlock()
	{
        HRESULT hr = _pTex->UnlockRect(0);
		return SUCCEEDED(hr);
	}

}
