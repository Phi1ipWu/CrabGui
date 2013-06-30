
#include "CrabGuiDefine.h"
#include "CrabGuiRBTree.h"


//	http://blog.csdn.net/walkingman321/article/details/6121486


extern "C"
{

	#ifdef _WIN32
	#pragma warning(disable : 4311)
	#pragma warning(disable : 4312)
	#endif

	#ifndef NULL
	#define NULL 0
	#endif

	#define container_of(ptr, type, member) ({					\
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})

	#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

	//
	// define
	//
	#define	RB_RED		0
	#define	RB_BLACK	1

	struct rb_node
	{
		unsigned long  rb_parent_color;
		struct rb_node *rb_right;
		struct rb_node *rb_left;
	};

	struct rb_root
	{
		struct rb_node *rb_node;
	};


	#define rb_parent(r)   ((struct rb_node *)((r)->rb_parent_color & ~3))
	#define rb_color(r)   ((r)->rb_parent_color & 1)
	#define rb_is_red(r)   (!rb_color(r))
	#define rb_is_black(r) rb_color(r)
	#define rb_set_red(r)  do { (r)->rb_parent_color &= ~1; } while (0)
	#define rb_set_black(r)  do { (r)->rb_parent_color |= 1; } while (0)

	static inline void rb_set_parent(struct rb_node *rb, struct rb_node *p)
	{
		rb->rb_parent_color = (rb->rb_parent_color & 3) | (unsigned long)p;
	}
	static inline void rb_set_color(struct rb_node *rb, int color)
	{
		rb->rb_parent_color = (rb->rb_parent_color & ~1) | color;
	}

	#define RB_ROOT	(struct rb_root) { NULL, }
	#define	rb_entry(ptr, type, member) container_of(ptr, type, member)

	#define RB_EMPTY_ROOT(root)	((root)->rb_node == NULL)
	#define RB_EMPTY_NODE(node)	(rb_parent(node) == node)
	#define RB_CLEAR_NODE(node)	(rb_set_parent(node, node))

	extern void rb_insert_color(struct rb_node *, struct rb_root *);
	extern void rb_erase(struct rb_node *, struct rb_root *);

	typedef void (*rb_augment_f)(struct rb_node *node, void *data);

	extern void rb_augment_insert(struct rb_node *node, rb_augment_f func, void *data);
	extern struct rb_node *rb_augment_erase_begin(struct rb_node *node);
	extern void rb_augment_erase_end(struct rb_node *node, rb_augment_f func, void *data);

	/* Find logical next and previous nodes in a tree */
	extern struct rb_node *rb_next(const struct rb_node *);
	extern struct rb_node *rb_prev(const struct rb_node *);
	extern struct rb_node *rb_first(const struct rb_root *);
	extern struct rb_node *rb_last(const struct rb_root *);

	/* Fast replacement of a single node without remove/rebalance/add/rebalance */
	extern void rb_replace_node(struct rb_node *victim, struct rb_node *repl, struct rb_root *root);

	static inline void rb_link_node(struct rb_node * node, struct rb_node * parent,	struct rb_node ** rb_link)
	{
		node->rb_parent_color = (unsigned long )parent;
		node->rb_left = node->rb_right = NULL;

		*rb_link = node;
	}

	//
	// implement
	//
	static void __rb_rotate_left(struct rb_node *node, struct rb_root *root)
	{
		struct rb_node *right = node->rb_right;
		struct rb_node *parent = rb_parent(node);

		if ((node->rb_right = right->rb_left))
			rb_set_parent(right->rb_left, node);
		right->rb_left = node;

		rb_set_parent(right, parent);

		if (parent)
		{
			if (node == parent->rb_left)
				parent->rb_left = right;
			else
				parent->rb_right = right;
		}
		else
			root->rb_node = right;
		rb_set_parent(node, right);
	}

	static void __rb_rotate_right(struct rb_node *node, struct rb_root *root)
	{
		struct rb_node *left = node->rb_left;
		struct rb_node *parent = rb_parent(node);

		if ((node->rb_left = left->rb_right))
			rb_set_parent(left->rb_right, node);
		left->rb_right = node;

		rb_set_parent(left, parent);

		if (parent)
		{
			if (node == parent->rb_right)
				parent->rb_right = left;
			else
				parent->rb_left = left;
		}
		else
			root->rb_node = left;
		rb_set_parent(node, left);
	}

	void rb_insert_color(struct rb_node *node, struct rb_root *root)
	{
		struct rb_node *parent, *gparent;

		while ((parent = rb_parent(node)) && rb_is_red(parent))
		{
			gparent = rb_parent(parent);

			if (parent == gparent->rb_left)
			{
				{
					register struct rb_node *uncle = gparent->rb_right;
					if (uncle && rb_is_red(uncle))
					{
						rb_set_black(uncle);
						rb_set_black(parent);
						rb_set_red(gparent);
						node = gparent;
						continue;
					}
				}

				if (parent->rb_right == node)
				{
					register struct rb_node *tmp;
					__rb_rotate_left(parent, root);
					tmp = parent;
					parent = node;
					node = tmp;
				}

				rb_set_black(parent);
				rb_set_red(gparent);
				__rb_rotate_right(gparent, root);
			} else {
				{
					register struct rb_node *uncle = gparent->rb_left;
					if (uncle && rb_is_red(uncle))
					{
						rb_set_black(uncle);
						rb_set_black(parent);
						rb_set_red(gparent);
						node = gparent;
						continue;
					}
				}

				if (parent->rb_left == node)
				{
					register struct rb_node *tmp;
					__rb_rotate_right(parent, root);
					tmp = parent;
					parent = node;
					node = tmp;
				}

				rb_set_black(parent);
				rb_set_red(gparent);
				__rb_rotate_left(gparent, root);
			}
		}

		rb_set_black(root->rb_node);
	}

	static void __rb_erase_color(struct rb_node *node, struct rb_node *parent,
					 struct rb_root *root)
	{
		struct rb_node *other;

		while ((!node || rb_is_black(node)) && node != root->rb_node)
		{
			if (parent->rb_left == node)
			{
				other = parent->rb_right;
				if (rb_is_red(other))
				{
					rb_set_black(other);
					rb_set_red(parent);
					__rb_rotate_left(parent, root);
					other = parent->rb_right;
				}
				if ((!other->rb_left || rb_is_black(other->rb_left)) &&
					(!other->rb_right || rb_is_black(other->rb_right)))
				{
					rb_set_red(other);
					node = parent;
					parent = rb_parent(node);
				}
				else
				{
					if (!other->rb_right || rb_is_black(other->rb_right))
					{
						rb_set_black(other->rb_left);
						rb_set_red(other);
						__rb_rotate_right(other, root);
						other = parent->rb_right;
					}
					rb_set_color(other, rb_color(parent));
					rb_set_black(parent);
					rb_set_black(other->rb_right);
					__rb_rotate_left(parent, root);
					node = root->rb_node;
					break;
				}
			}
			else
			{
				other = parent->rb_left;
				if (rb_is_red(other))
				{
					rb_set_black(other);
					rb_set_red(parent);
					__rb_rotate_right(parent, root);
					other = parent->rb_left;
				}
				if ((!other->rb_left || rb_is_black(other->rb_left)) &&
					(!other->rb_right || rb_is_black(other->rb_right)))
				{
					rb_set_red(other);
					node = parent;
					parent = rb_parent(node);
				}
				else
				{
					if (!other->rb_left || rb_is_black(other->rb_left))
					{
						rb_set_black(other->rb_right);
						rb_set_red(other);
						__rb_rotate_left(other, root);
						other = parent->rb_left;
					}
					rb_set_color(other, rb_color(parent));
					rb_set_black(parent);
					rb_set_black(other->rb_left);
					__rb_rotate_right(parent, root);
					node = root->rb_node;
					break;
				}
			}
		}
		if (node)
			rb_set_black(node);
	}

	void rb_erase(struct rb_node *node, struct rb_root *root)
	{
		struct rb_node *child, *parent;
		int color;

		if (!node->rb_left)
			child = node->rb_right;
		else if (!node->rb_right)
			child = node->rb_left;
		else
		{
			struct rb_node *old = node, *left;

			node = node->rb_right;
			while ((left = node->rb_left) != NULL)
				node = left;

			if (rb_parent(old)) {
				if (rb_parent(old)->rb_left == old)
					rb_parent(old)->rb_left = node;
				else
					rb_parent(old)->rb_right = node;
			} else
				root->rb_node = node;

			child = node->rb_right;
			parent = rb_parent(node);
			color = rb_color(node);

			if (parent == old) {
				parent = node;
			} else {
				if (child)
					rb_set_parent(child, parent);
				parent->rb_left = child;

				node->rb_right = old->rb_right;
				rb_set_parent(old->rb_right, node);
			}

			node->rb_parent_color = old->rb_parent_color;
			node->rb_left = old->rb_left;
			rb_set_parent(old->rb_left, node);

			goto color;
		}

		parent = rb_parent(node);
		color = rb_color(node);

		if (child)
			rb_set_parent(child, parent);
		if (parent)
		{
			if (parent->rb_left == node)
				parent->rb_left = child;
			else
				parent->rb_right = child;
		}
		else
			root->rb_node = child;

	 color:
		if (color == RB_BLACK)
			__rb_erase_color(child, parent, root);
	}

	static void rb_augment_path(struct rb_node *node, rb_augment_f func, void *data)
	{
		struct rb_node *parent;

	up:
		func(node, data);
		parent = rb_parent(node);
		if (!parent)
			return;

		if (node == parent->rb_left && parent->rb_right)
			func(parent->rb_right, data);
		else if (parent->rb_left)
			func(parent->rb_left, data);

		node = parent;
		goto up;
	}

	/*
	 * after inserting @node into the tree, update the tree to account for
	 * both the new entry and any damage done by rebalance
	 */
	void rb_augment_insert(struct rb_node *node, rb_augment_f func, void *data)
	{
		if (node->rb_left)
			node = node->rb_left;
		else if (node->rb_right)
			node = node->rb_right;

		rb_augment_path(node, func, data);
	}

	/*
	 * before removing the node, find the deepest node on the rebalance path
	 * that will still be there after @node gets removed
	 */
	struct rb_node *rb_augment_erase_begin(struct rb_node *node)
	{
		struct rb_node *deepest;

		if (!node->rb_right && !node->rb_left)
			deepest = rb_parent(node);
		else if (!node->rb_right)
			deepest = node->rb_left;
		else if (!node->rb_left)
			deepest = node->rb_right;
		else {
			deepest = rb_next(node);
			if (deepest->rb_right)
				deepest = deepest->rb_right;
			else if (rb_parent(deepest) != node)
				deepest = rb_parent(deepest);
		}

		return deepest;
	}

	/*
	 * after removal, update the tree to account for the removed entry
	 * and any rebalance damage.
	 */
	void rb_augment_erase_end(struct rb_node *node, rb_augment_f func, void *data)
	{
		if (node)
			rb_augment_path(node, func, data);
	}

	/*
	 * This function returns the first node (in sort order) of the tree.
	 */
	struct rb_node *rb_first(const struct rb_root *root)
	{
		struct rb_node	*n;

		n = root->rb_node;
		if (!n)
			return NULL;
		while (n->rb_left)
			n = n->rb_left;
		return n;
	}

	struct rb_node *rb_last(const struct rb_root *root)
	{
		struct rb_node	*n;

		n = root->rb_node;
		if (!n)
			return NULL;
		while (n->rb_right)
			n = n->rb_right;
		return n;
	}

	struct rb_node *rb_next(const struct rb_node *node)
	{
		struct rb_node *parent;

		if (rb_parent(node) == node)
			return NULL;

		/* If we have a right-hand child, go down and then left as far
		   as we can. */
		if (node->rb_right) {
			node = node->rb_right;
			while (node->rb_left)
				node=node->rb_left;
			return (struct rb_node *)node;
		}

		/* No right-hand children.  Everything down and left is
		   smaller than us, so any 'next' node must be in the general
		   direction of our parent. Go up the tree; any time the
		   ancestor is a right-hand child of its parent, keep going
		   up. First time it's a left-hand child of its parent, said
		   parent is our 'next' node. */
		while ((parent = rb_parent(node)) && node == parent->rb_right)
			node = parent;

		return parent;
	}

	struct rb_node *rb_prev(const struct rb_node *node)
	{
		struct rb_node *parent;

		if (rb_parent(node) == node)
			return NULL;

		/* If we have a left-hand child, go down and then right as far
		   as we can. */
		if (node->rb_left) {
			node = node->rb_left;
			while (node->rb_right)
				node=node->rb_right;
			return (struct rb_node *)node;
		}

		/* No left-hand children. Go up till we find an ancestor which
		   is a right-hand child of its parent */
		while ((parent = rb_parent(node)) && node == parent->rb_left)
			node = parent;

		return parent;
	}

	void rb_replace_node(struct rb_node *victim, struct rb_node *repl, struct rb_root *root)
	{
		struct rb_node *parent = rb_parent(victim);

		/* Set the surrounding nodes to point to the replacement */
		if (parent) {
			if (victim == parent->rb_left)
				parent->rb_left = repl;
			else
				parent->rb_right = repl;
		} else {
			root->rb_node = repl;
		}
		if (victim->rb_left)
			rb_set_parent(victim->rb_left, repl);
		if (victim->rb_right)
			rb_set_parent(victim->rb_right, repl);

		/* Copy the pointers/colour from the victim to the replacement */
		*repl = *victim;
	}

}


namespace CrabGui
{

	struct RBTreeNode
	{
		struct rb_node node;

		UInt	uKey;
		PVoid	pValue;
	};


	///////////////////////////////////////////////////////////////////////////////////////////////


	RBTree::RBTree()
		: _uNum(0)
	{
		_pRBRoot = CrabNew(rb_root);
		_pRBRoot->rb_node = 0;
	}

	RBTree::~RBTree()
	{
		clear();
		CrabDelete(_pRBRoot);
	}


	// 清空数据
	void RBTree::clear()
	{
		struct rb_node* pNode = 0;
		while (pNode = rb_first(_pRBRoot))
		{
			rb_erase(pNode, _pRBRoot);

			struct RBTreeNode* pRBTreeNode = (struct RBTreeNode*)pNode;
			CrabDelete(pRBTreeNode);
		}

		_uNum = 0;
	}


	// 在指定位置插入
	Bool RBTree::insert(UInt uKey, void* pValue)
	{
		struct rb_node **ppNode = &(_pRBRoot->rb_node), *pParent = NULL;

		while (*ppNode)
		{
			pParent = *ppNode;
			UInt uNodeKey = ((RBTreeNode*)(*ppNode))->uKey;

			if (uKey < uNodeKey)
				ppNode = &((*ppNode)->rb_left);
			else if (uKey > uNodeKey)
				ppNode = &((*ppNode)->rb_right);
			else
				return False;
		}

		struct RBTreeNode* pNewNode = CrabNew(struct RBTreeNode);
		pNewNode->uKey	 = uKey;
		pNewNode->pValue = pValue;

		rb_link_node(&pNewNode->node, pParent, ppNode);
		rb_insert_color(&pNewNode->node, _pRBRoot);

		++_uNum;
		return True;
	}


	// 移除指定位置
	void* RBTree::remove(UInt uKey)
	{
		struct RBTreeNode* pRBTreeNode = _findNode(uKey);
		PVoid pValue = 0;

		if (pRBTreeNode)
		{
			pValue = pRBTreeNode->pValue;

			rb_erase(&pRBTreeNode->node, _pRBRoot);
			CrabDelete(pRBTreeNode);

			--_uNum;
	    }

		return pValue;
	}


	// 查找数据
	void* RBTree::find(UInt uKey) const
	{
		struct RBTreeNode* pRBTreeNode = _findNode(uKey);

		if (pRBTreeNode)
			return pRBTreeNode->pValue;

		return 0;
	}


	// 获取个数
	UInt RBTree::getNum() const
	{
		return _uNum;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////


	// 定位到最一个
	void* RBTree::seekFirst(UInt* puKey)
	{
		_pRBIterator = rb_first(_pRBRoot);

		if (_pRBIterator)
		{
			struct RBTreeNode* pRBTreeNode = (struct RBTreeNode*)_pRBIterator;

			if (puKey)
				*puKey = pRBTreeNode->uKey;

			return pRBTreeNode->pValue;
		}

		return 0;
	}


	// 遍历下一个
	void* RBTree::seekNext(UInt* puKey)
	{
		if (_pRBIterator && (_pRBIterator = rb_next(_pRBIterator)))
		{
			struct RBTreeNode* pRBTreeNode = (struct RBTreeNode*)_pRBIterator;

			if (puKey)
				*puKey = pRBTreeNode->uKey;

			return pRBTreeNode->pValue;
		}

		return 0;
	}


	// 定位到最后一个
	void* RBTree::seekLast(UInt* puKey)
	{
		_pRBIterator = rb_last(_pRBRoot);

		if (_pRBIterator)
		{
			struct RBTreeNode* pRBTreeNode = (struct RBTreeNode*)_pRBIterator;

			if (puKey)
				*puKey = pRBTreeNode->uKey;

			return pRBTreeNode->pValue;
		}

		return 0;
	}


	// 遍历前一个
	void* RBTree::seekPrev(UInt* puKey)
	{
		if (_pRBIterator && (_pRBIterator = rb_prev(_pRBIterator)))
		{
			struct RBTreeNode* pRBTreeNode = (struct RBTreeNode*)_pRBIterator;

			if (puKey)
				*puKey = pRBTreeNode->uKey;

			return pRBTreeNode->pValue;
		}

		return 0;
	}


	///////////////////////////////////////////////////////////////////////////////////////////////


	RBTreeNode* RBTree::_findNode(UInt uKey) const
	{
		struct rb_node *pNode = _pRBRoot->rb_node;

		while (pNode)
		{
			UInt uNodeKey = ((RBTreeNode*)(pNode))->uKey;

			if (uKey < uNodeKey)
				pNode = pNode->rb_left;
			else if (uKey > uNodeKey)
				pNode = pNode->rb_right;
			else
				return ((RBTreeNode*)(pNode));
		}

		return 0;
	}

}
