
#ifndef __CrabGuiMesh_h__
#define __CrabGuiMesh_h__


namespace CrabGui
{

	/// 网格
	class CRABGUIEXPORT Mesh
	{
	public:
		Mesh();
		Mesh(UInt uVertexSize, UInt uTriangleSize);
		virtual ~Mesh();

		/// 清除数据
		void    clear();

		/// 设置顶点个数
		void 	setVertexSize(UInt uVertexSize);

		/// 设置顶点
		Bool 	setVertex(UInt uVertexIndex, const PointReal* pVertex, const PointReal* pTexVertex1, const PointReal* pTexVertex2);

		/// 获得顶点
		Bool 	getVertex(UInt uVertexIndex, PointReal* pVertex);

		/// 获取顶点个数
		UInt 	getVertexSize() const;

		/// 获取顶点
		const PointReal* getVertices() const;

		/// 获取所有纹理UV
		const PointReal* getTexVertices() const;

		/// 设置三角形个数
		void 	setTriangleSize(UInt uTriangleSize);
		
		/// 设置单个三角形
		Bool 	setTriangle(UInt uTriangleIndex, Int16 sIndex1, Int16 sIndex2, Int16 sIndex3);

		/// 获取三角形个数
		UInt	getTriangleSize() const;

		/// 获取三角形
		const Int16* getTriangles() const;


	public:

		/// 设置平面
		void	setPlane(Point ptGridSize, Point ptPos, Point ptSize);
		
		/// 获取平面顶点
		Bool	getPlaneVertex(Point ptGridSize, Point ptPos, Point ptSize, UInt uIndex, PointReal* pVertex, PointReal* pTexVertex);

		/// 设置圆
		void	setCircle(UInt uFanSize, Point ptPos, Point ptSize);


	protected:
		UInt  			_uVertexSize;
		PointReal*		_pVertices;
		PointReal*		_pTexVertices;

		UInt 			_uTriangleSize;
		Int16*			_pTriangles;
	};

}

#endif	//	__CrabGuiMesh_h__
