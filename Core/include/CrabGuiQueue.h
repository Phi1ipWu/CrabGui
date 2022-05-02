
#ifndef __CrabGuiQueue_h__
#define __CrabGuiQueue_h__


namespace CrabGui
{

	/// 队列（类C风格）
	class CRABGUIEXPORT Queue
	{
	public:
		Queue();
		Queue(UInt uIncrease);
		virtual ~Queue();

		/// 清空数据
		void	clear();

		/// 在指定位置插入
		Bool	insert(UInt uIndex, void* pUserData);

		/// 在指定位置插入多个空数据
		Bool	insertEmpty(UInt uIndex, UInt uNum);

		/// 移除指定位置
		void*	remove(UInt uIndex);

		/// 头插
		Bool	insertFront(void* pUserData);

		/// 尾插（效率最高）
		Bool	insertEnd(void* pUserData);

		/// 移除头
		void*	removeFront();

		/// 移除尾（效率最高）
		void*	removeEnd();

		/// 获取指定位置
		void*	getAt(UInt uIndex);

		/// 数据交换
		Bool	swap(UInt uIndex1, UInt uIndex2);

		/// 预备空间
		void	reserve(UInt uNum);


	public:
		/// 获取个数
		UInt	getNum() const;

		/// 下标设置
		Bool	setAt(UInt uIndex, void* pData);

		/// 下标获取
		void*	getAt(UInt uIndex) const;

		/// 默认增速
		static const UInt DefaultIncrease;


	private:
		Queue(const Queue&);
		Queue& operator=(const Queue&);

	private:
		void**	_ppData;
		UInt	_uNum;
		UInt	_uMax;
		UInt	_uIncrease;
	};

}


#endif	//	__CrabGuiQueue_h__

