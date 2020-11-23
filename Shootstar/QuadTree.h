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
	// Properties
	private:
		int height;
		int width;
		int depth;
		int num_instance;
		bool has_leaf;
		Vector2 pivot;
		std::vector<QuadTree*> partition;
	public:
		QuadTree(int, int, int, Vector2, int);
		~QuadTree();
		QuadTreeLeaf leaf;

	// Methods
	private:
		int FindQuadrant(Vector2);
	public:
		static void DestroyRecursive(QuadTree*);
		static void ClearTreeRecursive(QuadTree*);
		void FillTree(Node2D*);
		QuadTree* GetData(Vector2);
		void Print();
	};
}


