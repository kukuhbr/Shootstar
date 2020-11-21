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
	private:
		int height;
		int width;
		int depth;
		int num_instance;
		bool has_leaf;
		Vector2 dimension;
		Vector2 pivot;
		std::vector<QuadTree*> partition;
	public:
		QuadTree(int, int, int, Vector2, int);
		~QuadTree();
		QuadTreeLeaf leaf;

	private:
		int FindQuadrant(Vector2);
	public:
		static void DestroyRecursive(QuadTree*);
		void FillTree(Node2D*);
		QuadTree* GetData(QuadTree*, Vector2);
		void Print();
	};
}


