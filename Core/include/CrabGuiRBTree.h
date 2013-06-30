
#ifndef __CrabGuiRBTree_h__
#define __CrabGuiRBTree_h__


struct rb_root;
struct rb_node;
struct RBTReeNode;


namespace CrabGui
{

	/// 红黑树（类C风格，linux kernel 代码）
	class CRABGUIEXPORT RBTree
	{
	public:
		RBTree();
		virtual ~RBTree();

		/// 清空数据
		void	clear();

		/// 在指定位置插入
		Bool	insert(UInt uKey, void* pValue);

		/// 移除指定位置
		void*	remove(UInt uKey);

		/// 查找数据
		void*	find(UInt uKey) const;

		/// 获取个数
		UInt	getNum() const;


	public:
		/// 定位到最一个
		void*	seekFirst(UInt* puKey);

		/// 遍历下一个
		void*	seekNext(UInt* puKey);

		/// 定位到最后一个
		void*	seekLast(UInt* puKey);

		/// 遍历前一个
		void*	seekPrev(UInt* puKey);


	private:
		RBTree(const RBTree&);
		RBTree& operator=(const RBTree&);

		struct RBTreeNode*	_findNode(UInt uKey) const;


	private:
		struct rb_root*	_pRBRoot;
		struct rb_node*	_pRBIterator;
		UInt	_uNum;
	};

}


#endif	//	__CrabGuiRBTree_h__
