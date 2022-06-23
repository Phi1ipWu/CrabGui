
#include "CrabGui.h"
#include "CrabGuiSample_FirstWindow.h"

#include "CrabGuiStbParser.h"


namespace CrabGui
{

	SampleFirstWindow::SampleFirstWindow()
	{
	}


	SampleFirstWindow::~SampleFirstWindow()
	{
	}


	// 逻辑层的初始化
	Bool SampleFirstWindow::init()
	{
		// test code
		{
			//_testManualCreate();
			//return True;

			//_testRBTree();
			//return True;

			//_testConvertPNG();
			//return True;
			
			//_testBatchConvertJPG();
			//return True;

			//_testClipPNG();
			//return True;
		}


		UInt uCanvasCellSize = 10;

		_frmWnd1 = (FrameWindow*)_pParser->createFromFile(_pSystem->getRootWindow(), "frmTest1.crabgui");
		_frmWnd1->setEventHandle(this);
		_frmWnd1->setUseCanvas(10);

		String strTemp;
		for (UInt i = 0; i < 4; ++i)
		{
			strTemp.format("btn_%d", i);
			_btn[i] = (Button*)_frmWnd1->getChild(strTemp.getStrPtr());
		}

		_frmWnd1->setTextAlignType(AT_Center);

		_frmWnd2 = (FrameWindow*)_pParser->createFromFile(_pSystem->getRootWindow(), "frmTest2.crabgui");
		_frmWnd2->setEventHandle(this);
		_frmWnd2->setUseCanvas(uCanvasCellSize);

		_edtText = (Editbox*)_frmWnd2->getChild("edtText");
		_lstItem = (Listbox*)_frmWnd2->getChild("lstItem");

		UInt uColumn = 4;
		String strText;
		for (UInt j = 0; j < uColumn; ++j)
		{
			strText.format("列%d", j + 1);
			_lstItem->insertColumn(j, strText.getStrPtr(), 40, False);
		}

		for (UInt i = 0; i < 10; ++i)
		{
			_lstItem->insertItemRow(i);

			for (UInt j = 0; j < uColumn; ++j)
			{
				strText.format("%d-%d", i + 1, j + 1);
				_lstItem->setItemText(i, j, strText.getStrPtr());

				//Color cColor = 0xFF000000 | (rand() & 255) << 16 | (rand() & 255) << 8 | (rand() & 255);
				//_lstItem->setItemColor(i, j, cColor);
			}
		}

		return True;
	}


	// 逻辑层的销毁
	void SampleFirstWindow::cleanup()
	{
		// test code
		_testSaveFontTexture();
	}


	// 逻辑层的每帧更新
	void SampleFirstWindow::update()
	{
	}


	// 响应事件
	Bool SampleFirstWindow::eventHandle(const EventDesc* pEvent)
	{
		if (pEvent->uEvent == Event_MouseClick)
		{
			if (pEvent->pWindow == _btn[0])
			{
				UInt uAlignType = _frmWnd1->getTextAlignType();
				uAlignType = (uAlignType + 1) % AT_Size;
				_frmWnd1->setTextAlignType((AlignType)uAlignType);

				//UInt uAlignType = _edtText->getTextAlignType();
				//uAlignType = (uAlignType + 1) % AT_Size;
				//_edtText->setTextAlignType((AlignType)uAlignType);
			}
			if (pEvent->pWindow == _btn[1])
			{
				_lstItem->setDrawHeader(!_lstItem->isDrawHeader());
			}
			if (pEvent->pWindow == _btn[2])
			{
				_edtText->setTextWrap(!_edtText->isTextWrap());
			}
			if (pEvent->pWindow == _btn[3])
			{
				_frmWnd2->setVisible(!_frmWnd2->isVisible());

				//Window* pWnd = _lstItem->getChild((UInt)0)->getChild((UInt)0);
				//_pSystem->destroyWindow(pWnd);
			}
		}

		return False;
	}


	void SampleFirstWindow::_testManualCreate()
	{
		_frmWnd1 = (FrameWindow*)_pSystem->createWindow("FrameWindow");
		_frmWnd1->setParent(_pSystem->getRootWindow());
		_frmWnd1->setEventHandle(this);

		_frmWnd1->setName("frmWnd");

		_frmWnd1->setPropert("Pos",		"100,100");
		_frmWnd1->setPropert("Size",	"237,217");
		_frmWnd1->setPropert("Visible",	"1");
		_frmWnd1->setPropert("Enable",	"1");
		_frmWnd1->setPropert("Movable",	"1");

		_frmWnd1->setPropert("PicFile",		"test1.tga");
		_frmWnd1->setPropert("PicAlphaOp",	"1");
		_frmWnd1->setPropert("PicRect",		"0,0,237,217");

		_frmWnd1->setPropert("FontSize",		"16");
		_frmWnd1->setPropert("FontColor",		"4294967295");
		_frmWnd1->setPropert("FontShadowColor",	"4278190080");
		_frmWnd1->setPropert("FontAlphaOp",		"2");
		_frmWnd1->setPropert("Text",	"我人有的和\\t主产不为这\\n工要在地一上是中国同\\n\\\\经以发了民");

		// 必须要在设置大小后设置
		UInt uCanvasCellSize = 10;
		_frmWnd1->setUseCanvas(uCanvasCellSize);

		UInt uPicRect_y[4] = {0, 20, 40, 214};
		String strTemp;
		for (UInt i = 0; i < 4; ++i)
		{
			strTemp.format("btn_%d", i);

			_btn[i] = (Button*)_pSystem->createWindow("Button");
			_btn[i]->setName(strTemp.getStrPtr());
			_btn[i]->setParent(_frmWnd1);

			Point ptBtnSize(41, 20);
			_btn[i]->setSize(Point(41, 20));
			_btn[i]->setPos(Point(i * 50 + 20, 190));
			_btn[i]->setVisible(True);
			_btn[i]->setEnable(True);

			_btn[i]->setPropert("PicFile",		"test2.tga");
			_btn[i]->setPropert("PicAlphaOp",	"0");

			for (UInt j = 0; j < Button::BS_Size; ++j)
			{
				Rect rcPicRect, rcPicGrid;
				rcPicRect.x1 = ptBtnSize.x * j;
				rcPicRect.y1 = uPicRect_y[i];
				rcPicRect.x2 = rcPicRect.x1 + ptBtnSize.x;
				rcPicRect.y2 = rcPicRect.y1 + ptBtnSize.y;

				_btn[i]->setPicRect((ButtonState)j, rcPicRect, rcPicGrid);
			}
		}


		// 创建第二个窗体，并复制属性
		_frmWnd2 = (FrameWindow*)_pSystem->createWindow("FrameWindow");
		_frmWnd2->setParent(_pSystem->getRootWindow());
		_pSystem->copyProperty(_frmWnd2, _frmWnd1);	// 复制第一个窗体的属性
		_frmWnd2->setPos(Point(200, 200));
		_frmWnd2->setPropert("Text",	"");

		// 必须要在设置大小后设置
		_frmWnd2->setUseCanvas(10);

		_edtText = (Editbox*)_pSystem->createWindow("Editbox");
		_edtText->setParent(_frmWnd2);
		_edtText->setName("edtText");
		_edtText->setPropert("Pos",			"68,138");
		_edtText->setPropert("Size",		"110,28");
		_edtText->setPropert("Visible",		"1");
		_edtText->setPropert("Enable",		"1");
		_edtText->setPropert("MultiLine",	"1");
		_edtText->setPropert("TextWrap",	"1");

		_edtText->setPropert("FontSize",		"12");
		_edtText->setPropert("FontColor",		"4278190080");
		_edtText->setPropert("FontAlphaOp",		"2");
		_edtText->setPropert("LineHeight",		"14");
		_edtText->setText("我人有的和主产不为这工要在地一上是中国同经以发了民");

		_edtText->setPropert("FontEmptyText",		"空白编辑框");
		_edtText->setPropert("FontEmptyTextColor",	"4286545791");

		//// 把第一个窗体和谐掉
		//_pSystem->destroyWindow(_frmWnd);

		// 创建列表
		_lstItem = (Listbox*)_pSystem->createWindow("Listbox");
		_lstItem->setParent(_frmWnd2);

		_lstItem->setName("lstItem");
		_lstItem->setPos(Point(60, 29));
		_lstItem->setSize(Point(165, 87));
		_lstItem->setPropert("ListWindow",	"0,0,0,0");
		_lstItem->setDrawHeader(True);

		_lstItem->insertColumn(0, "测试一", 80, False);
		_lstItem->insertColumn(1, "测试二", 70, False);

		for (UInt i = 0; i < 10; ++i)
		{
			_lstItem->insertItemRow(i);

			_lstItem->setItemText(i, 0, "111111");
			_lstItem->setItemText(i, 1, "222222");
		}

		// 创建滚动条
		ScrollBar* pVscrList = (ScrollBar*)_pSystem->createWindow("ScrollBar");
		pVscrList->setName("vscrList");
		pVscrList->setParent(_lstItem);
		Point ptSize = _lstItem->getSize();ptSize.x = 8;
		pVscrList->setSize(ptSize);
		Point ptPos(_lstItem->getSize().x - ptSize.x, 0);
		pVscrList->setPos(ptPos);
		// 滚动条下的按钮，滑块
		Button* pBtnUp = (Button*)_pSystem->createWindow("Button");
		pBtnUp->setName("up");
		pBtnUp->setPropert("PicFile",		"test2.tga");
		pBtnUp->setPropert("PicAlphaOp",	"0");
		Button* pBtnDown = (Button*)_pSystem->createWindow("Button");
		pBtnDown->setName("down");
		pBtnDown->setPropert("PicFile",		"test2.tga");
		pBtnDown->setPropert("PicAlphaOp",	"0");
		Rect rcBtn, rcGrid;
		rcBtn.setRect(167, 0, 167 + 8, 8);
		pBtnUp->setPicRect  (BS_Normal,  rcBtn, Rect());
		pBtnDown->setPicRect(BS_Normal,  rcBtn, Rect());
		pBtnUp->setPicRect  (BS_Disable, rcBtn, Rect());
		pBtnDown->setPicRect(BS_Disable, rcBtn, Rect());
		rcBtn.setRect(178, 0, 178 + 8, 8);
		pBtnUp->setPicRect  (BS_Hover, rcBtn, Rect());
		pBtnDown->setPicRect(BS_Hover, rcBtn, Rect());
		rcBtn.setRect(189, 0, 189 + 8, 8);
		pBtnUp->setPicRect  (BS_Pushed, rcBtn, Rect());
		pBtnDown->setPicRect(BS_Pushed, rcBtn, Rect());

		Button* pBtnBar = (Button*)_pSystem->createWindow("Button");
		pBtnBar->setName("bar");
		pBtnBar->setPropert("PicFile",		"test2.tga");
		pBtnBar->setPropert("PicAlphaOp",	"0");
		rcBtn.setRect(168, 10, 168 + 8, 10 + 43);
		rcGrid.setRect(rcBtn.x1, rcBtn.y1 + 3, rcBtn.x2, rcBtn.y2 - 3);
		pBtnBar->setPicRect(BS_Normal,  rcBtn, rcGrid);
		pBtnBar->setPicRect(BS_Disable, rcBtn, rcGrid);
		rcBtn.setRect(177, 10, 177 + 8, 10 + 43);
		rcGrid.setRect(rcBtn.x1, rcBtn.y1 + 3, rcBtn.x2, rcBtn.y2 - 3);
		pBtnBar->setPicRect(BS_Hover,  rcBtn, rcGrid);
		rcBtn.setRect(186, 10, 186 + 8, 10 + 43);
		rcGrid.setRect(rcBtn.x1, rcBtn.y1 + 3, rcBtn.x2, rcBtn.y2 - 3);
		pBtnBar->setPicRect(BS_Pushed,  rcBtn, rcGrid);

		pBtnUp->setSize(Point(8, 8));
		pBtnBar->setSize(Point(8, 30));
		pBtnDown->setSize(Point(8, 8));
		pBtnUp->setPos(Point(0, 0));
		pBtnBar->setPos(Point(0, 8));
		pBtnDown->setPos(Point(0, ptSize.y - 8));

		pBtnUp->setParent(pVscrList);
		pBtnBar->setParent(pVscrList);
		pBtnDown->setParent(pVscrList);


		// 保存下~
		_pParser->saveToFile(_frmWnd1, "frmTest1.crabgui");
		_pParser->saveToFile(_frmWnd2, "frmTest2.crabgui");
	}


	void SampleFirstWindow::_testRBTree()
	{
		CrabGui::RBTree t;

		UInt a[1000];
		for (UInt i = 0; i < 1000; ++i)
		{
			a[i] = rand();
			t.insert(a[i], a + i);
		}

		//t.remove(3);
		//t.remove(5);
		for (UInt i = 0; i < 10; ++i)
		{
			UInt* pInt = (UInt*)t.find(i);

			int r;
			r = 0;
		}

		UInt uKey = 0xFFFFFFFF;
		for (void* pData = t.seekFirst(&uKey); pData; pData = t.seekNext(&uKey))
		{
			UInt uValue = *((UInt*)pData);

			int r;
			r = 0;
		}
		for (void* pData = t.seekLast(&uKey); pData; pData = t.seekPrev(&uKey))
		{
			UInt uValue = *((UInt*)pData);

			int r;
			r = 0;
		}
	}


	void SampleFirstWindow::_testConvertPNG()
	{
/*
		// 测试图片，洋红转Alpha
		Image* pImage = _pParser->createImage();
		if (pImage->loadFromFile("D:/Works/Mobile/Pocker/resource/textures/table.png"))
		{
			Point ptPos(0, 0), ptSize = pImage->getSize();
			for (ptPos.y = 0; ptPos.y < ptSize.y; ++ptPos.y)
			{
				for (ptPos.x = 0; ptPos.x < ptSize.x; ++ptPos.x)
				{
					Color cColor = pImage->getColor(ptPos);
					unsigned char r = (cColor >> 16) & 0xFF;
					unsigned char g = (cColor >>  8) & 0xFF;
					unsigned char b = cColor & 0xFF;

					// 洋红转 Alpha
					if ((cColor & 0xFFFFFF) == 0xFF00FF)
						pImage->setColor(ptPos, cColor & 0xFFFFFF);
				}
			}
			Point ptResize(960, 0);	// y = 0 自动按比例缩放
			pImage->saveToFile(IFF_PNG, "D:/Works/Mobile/Pocker/client/projects/Pocker/Resources/table.png", 0, &ptResize);
		}
		_pParser->destroyImage(pImage);
*/
		// 测试图片，白色变成Alpha通道
		Image* pImage = _pParser->createImage();
		if (pImage->loadFromFile("E:/map.png"))
		{
			Point ptPos;
			Point ptSize = pImage->getSize();
			for (ptPos.y = 0; ptPos.y < ptSize.y; ++ptPos.y)
			{
				for (ptPos.x = 0; ptPos.x < ptSize.x; ++ptPos.x)
				{
					Color cColor = pImage->getColor(ptPos);
					if ((cColor & 0x00FFFFFF) == 0x00FFFFFF)
					{
						pImage->setColor(ptPos, cColor & 0x00FFFFFF);
					}
				}
			}
			pImage->saveToFile(IFF_PNG, "E:/map1.png", 0, 0);
		}
		_pParser->destroyImage(pImage);
	}


	void SampleFirstWindow::_testClipPNG()
	{

		// 测试图片，裁减，缩放
		Image* pImage = _pParser->createImage();
		if (pImage->loadFromFile("D:/Works/Mobile/cocos2d-2.0-x-2.0.4/MobileTest/Resources/Male.png"))
		{
			const Int16 nRow	= 4;
			const Int16 nCol	= 4;
			const Int16 nWidth	= 32;
			const Int16 nHeight	= 48;

			Rect rcClip;
			String strFile;
			for (Int y = 0; y < nRow; ++y)
			{
				for (Int x = 0; x < nCol; ++x)
				{
					rcClip.setRect(x * nWidth, y * nHeight, (x + 1) * nWidth, (y + 1) * nHeight);
					strFile.format("%s_%d_%d.png", "D:/Works/Mobile/cocos2d-2.0-x-2.0.4/MobileTest/TestResources/Male", y, x);

					pImage->saveToFile(IFF_PNG, strFile.getStrPtr(), &rcClip, 0);
				}
			}
		}
		_pParser->destroyImage(pImage);
	}


	void SampleFirstWindow::_testBatchConvertJPG()
	{
		Char szFilePath[] = "C:\\0803_XJW\\";	// 文件路径，最后必须以 \\ 结尾（拼字符串）
		Char szFileFmt[]  = "*.jpg";			// 文件格式
		Char szTmpFile[]  = "C:\\tmp.txt";

		String strCmd;
		strCmd.format("dir %s%s /b > %s", szFilePath, szFileFmt, szTmpFile);
		system(strCmd.getStrPtr());

		FILE* fp = fopen(szTmpFile, "r");
		if (!fp)
			return;

		Char szTmp[256] = {0};
		Image* pImage = _pParser->createImage();
		for (Char* pszTmp = 0; !!(pszTmp = fgets(szTmp, 256, fp)); )
		{
			String strFile;
			strFile.format("%s%s", szFilePath, pszTmp);

			if (!pImage->loadFromFile(strFile.getStrPtr()))
				continue;

			Point ptSize = pImage->getSize();
			if (ptSize.x < ptSize.y)
				continue;

			ptSize.x = ptSize.y / 2 * 3;
			pImage->saveToFile(IFF_JPG, strFile.getStrPtr(), 0, &ptSize);
		}
		_pParser->destroyImage(pImage);

		fclose(fp);
	}


	void SampleFirstWindow::_testSaveFontTexture()
	{
		FontTexture* pFontTexture = _pSystem->acquireFont("");
		for (UInt j = 0; j <= 9999; ++j)
		{
			Texture* pTexture = pFontTexture ? pFontTexture->getTexture(16, j) : 0;
			if (!pTexture)
				return;

			Color* pData;
			UInt uPitch;
			if (pTexture->lock((void*&)pData, uPitch))
			{
				for (Int i = 0; i < pTexture->getSize().getArea(); ++i)
				{
					UInt8 cAlpha = pData[i] >> 24;
					pData[i] = 0xFF << 24 | cAlpha << 16 | cAlpha << 8 | cAlpha;
				}

				pTexture->unlock();
			}
			String strFileName;
			strFileName.format("./FontTexture_16_%d.png", j);
			pTexture->saveToFile(strFileName.getStrPtr());
		}
	}

}	// end namespace


INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, INT)
{
	CrabGui::SampleFirstWindow sample;
	return sample.run(CrabGui::Point(1024, 768), True);
}
