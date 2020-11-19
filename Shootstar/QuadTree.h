#pragma once

#include <vector>
#include <algorithm>
#include <Node2D.hpp>

namespace godot {
	struct QuadTreeLeaf {
		std::vector<Node2D*> data;
	};

	class QuadTree
	{
		QuadTree(int, int, int, Vector2, int);
		~QuadTree();

		int height;
		int width;
		int depth;
		Vector2 dimension;
		Vector2 pivot;

		/*QuadTree* AtasKanan;
		QuadTree* AtasKiri;
		QuadTree* BawahKanan;
		QuadTree* BawahKiri;*/
		vector<QuadTree*> partition;
		QuadTreeLeaf leaf;

		bool has_leaf;
		void FillTree(Node2D*);
		QuadTree* GetData(QuadTree*, Vector2);
		void DestroyRecursive(QuadTree*);
		int FindQuadrant(Vector2)
	};
}


