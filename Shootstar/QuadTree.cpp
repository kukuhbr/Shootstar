#include "QuadTree.h"
#include "Commons.h"

QuadTree::QuadTree(int _width, int _height, int _depth, Vector2 _pivot, int quadrant) {
	width = _width;
	height = _height;
	depth = _depth;
	pivot = _pivot;
	if (depth = 0) {
		leaf.data.reserve(5);
	}
	else {
		if (quadrant == -1) {
			pivot = Vector2(0, 0);
		}
		else if (quadrant == 0) {
			this->pivot + Vector2(_width, _height);
		}
		else if (quadrant == 1) {
			this->pivot + Vector2(_width, -_height);
		}
		else if (quadrant == 2) {
			this->pivot + Vector2(-_width, _height);
		}
		else if (quadrant == 3) {
			this->pivot + Vector2(-_width, -_height);
		}
		has_leaf = false;
		for (int i = 0; i < 3; i++) {
			partition[i] = new QuadTree(_width / 2, _height / 2, _depth - 1, _pivot, i);
		}
	}
	
}

void QuadTree::DestroyRecursive(QuadTree*) {
	if (depth == 0) {
		leaf.data.clear();
	}
	else {
		for (int i = 0; i < 3; i++) {
			DestroyRecursive(partition[i]);
			delete partition[i];
		}
	}
}

QuadTree::~QuadTree() {
	DestroyRecursive(this);
}

void QuadTree::FillTree(Node2D* node) {
	Vector2 position = node->get_global_position();
	QuadTree* reference = this;
	int iter = depth;
	while (iter != 0) {
		reference = partition[FindQuadrant(position)];
		iter--;
	}
	reference->leaf.data.push_back(node);
}

QuadTree* QuadTree::GetData(QuadTree* q, Vector2 v) {
	if (depth == 0 && leaf.data.size() > 0) {
		return this;
	}
	else {
		return nullptr;
	}
	int quadrant = FindQuadrant(v);
	vector<int> order;
	order.push_back(quadrant);
	vector<int> temp = { 1, 2, 3, 4 };
	for (auto it = temp.begin(); it != temp.end(); ++it) {
		if (std::find(order.begin(), order.end(), quadrant) != order.end()) {
			order.push_back(*it);
		}
	}
	for (auto it = order.begin(); it != order.end(); ++it) {
		GetData(partition[*it], v);
	}
}

int QuadTree::FindQuadrant(Vector2 v) {
	if (v.x > pivot.x) {
		if (v.y > pivot.y) {
			return 3;
		}
		else {
			return 0;
		}
	}
	else {
		if (v.y > pivot.y) {
			return 2;
		}
		else {
			return 1;
		}
	}
}