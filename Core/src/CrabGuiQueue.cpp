
#include "CrabGuiDefine.h"
#include "CrabGuiQueue.h"

// 感谢牛人Jampe指点：GCC 的 memset 相关函式，是 string.h
#include <string.h>


namespace CrabGui
{

	// 默认增量
	const UInt Queue::DefaultIncrease = 16;


	Queue::Queue()
		:	_ppData(0), _uNum(0), _uMax(0), _uIncrease(DefaultIncrease)	// 默认增量
	{
	}

	Queue::Queue(UInt uIncrease)
		:	_ppData(0), _uNum(0), _uMax(0), _uIncrease(uIncrease)
	{
		if (_uIncrease < DefaultIncrease)
		{
			_uIncrease = DefaultIncrease;
		}
	}

	Queue::~Queue()
	{
		CrabDeleteArray(_ppData);
	}


	// 清空数据
	void Queue::clear()
	{
		memset(_ppData, 0, sizeof(void*) * _uMax);
		_uNum = 0;
	}


	// 在指定位置插入
	Bool Queue::insert(UInt uIndex, void* pUserData)
	{
		if (insertEmpty(uIndex, 1))
		{
			_ppData[uIndex] = pUserData;
			return True;
		}

		return False;
	}


	// 在指定位置插入多个空数据
	Bool Queue::insertEmpty(UInt uIndex, UInt uNum)
	{
		// 允许越界一个，作为尾插
		if(uIndex > _uNum)
			return False;

		// 需要扩充容量
		reserve(_uNum + uNum);

		// 队列数据向后移
		for (UInt i = _uNum; i > uIndex; --i)
		{
			_ppData[i] = _ppData[i - uNum];
		}

		memset(&_ppData[uIndex], 0, sizeof(void*) * uNum);
		_uNum += uNum;

		return True;
	}


	// 移除指定位置
	void* Queue::remove(UInt uIndex)
	{
		if(uIndex >= _uNum)
			return 0;

		void* pUserData = _ppData[uIndex];

		// 队列数据向前移一位
		for(UInt i = uIndex + 1; i < _uNum; ++i)
		{
			_ppData[i - 1] = _ppData[i];
		}
		_ppData[--_uNum] = 0;

		return pUserData;
	}


	// 头插
	Bool Queue::insertFront(void* pUserData)
	{
		return insert(0, pUserData);
	}


	// 尾插（效率最高）
	Bool Queue::insertEnd(void* pUserData)
	{
		return insert(_uNum, pUserData);
	}


	// 移除头
	void* Queue::removeFront()
	{
		return remove(0);
	}


	// 移除尾（效率最高）
	void* Queue::removeEnd()
	{
		//return remove(_uNum - 1);

		void* pUserData = _ppData[_uNum - 1];
		_ppData[-- _uNum] = 0;

		return pUserData;
	}


	// 获取指定位置
	void* Queue::getAt(UInt uIndex)
	{
		return (uIndex < _uNum) ? _ppData[uIndex] : 0;
	}


	// 数据交换
	Bool Queue::swap(UInt uIndex1, UInt uIndex2)
	{
		if (uIndex1 < _uNum && uIndex2 < _uNum)
		{
			void* pUserData  = _ppData[uIndex1];
			_ppData[uIndex1] = _ppData[uIndex2];
			_ppData[uIndex2] = pUserData;

			return True;
		}

		return False;
	}


	// 预备空间
	void Queue::reserve(UInt uNum)
	{
		// 需要扩充容量
		if (uNum >= _uMax)
		{
			UInt uNewMax = (uNum / _uIncrease + 1) * _uIncrease;
			void** ppNewData = CrabNewArray(void*, uNewMax);

			if(_ppData)
			{
				// 老数据迁移
				memcpy(ppNewData, _ppData, sizeof(void*) * _uMax);
				CrabDeleteArray(_ppData);

				// 新多出来的指针位置归 0
				memset(&ppNewData[_uNum], 0, sizeof(void*) * (uNewMax - _uMax));
			}
			else
			{
				// 全部的指针位置归 0
				memset(ppNewData, 0, sizeof(void*) * uNewMax);
			}

			_ppData	= ppNewData;
			_uMax	= uNewMax;
		}
	}


	///////////////////////////////////////////////////////////////////////////////////////////////


	// 获取个数
	UInt Queue::getNum() const
	{
		return _uNum;
	}

	
	// 下标设置
	Bool Queue::setAt(UInt uIndex, void* pData)
	{
		if(uIndex >= _uNum)
			return False;

		_ppData[uIndex] = pData;
		return True;
	}


	// 下标获取
	void* Queue::getAt(UInt uIndex) const
	{
		if(uIndex >= _uNum)
			return 0;

		return _ppData[uIndex];
	}

}

