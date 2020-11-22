#include "QuadTree.h"
#include "Commons.h"

QuadTree::QuadTree(int _width, int _height, int _depth, Vector2 _pivot, int quadrant) {
	width = _width;
	height = _height;
	depth = _depth;
	pivot = _pivot;
	num_instance = 0;
	if (quadrant == -1) {
		pivot = Vector2(0, 0);
	}
	else if (quadrant == 0) {
		this->pivot += Vector2(_width, _height);
	}
	else if (quadrant == 1) {
		this->pivot += Vector2(_width, -_height);
	}
	else if (quadrant == 2) {
		this->pivot += Vector2(-_width, _height);
	}
	else if (quadrant == 3) {
		this->pivot += Vector2(-_width, -_height);
	}
	has_leaf = false;
	//Godot::print("{0} {1} {2} {3}", width, height, depth, pivot);
	if (depth == 0) {
		//Godot::print("We're at leaf of quadrant {0}", quadrant);
		//leaf.data.reserve(5);
	} else {
		//Godot::print("This quadrant {0} has pivot {1}", quadrant, pivot);
		for (int i = 0; i < 4; i++) {
			//Godot::print("From depth {0} we traverse quad {1}", depth, i);
			partition.push_back(new QuadTree(_width / 2, _height / 2, _depth - 1, pivot, i));
		}
	}
	
}

void QuadTree::DestroyRecursive(QuadTree* q) {
	if (q) {
		if (q->depth == 0) {
			if (q->has_leaf) {
				q->leaf.data.clear();
			}
		}
		else {
			while (!q->partition.empty()) {
				QuadTree* to_delete = *(q->partition.begin());
				q->partition.erase(q->partition.begin());
				DestroyRecursive(to_delete);
				
				delete to_delete;
			}
			q->partition.clear();
		}
	}
}

void QuadTree::ClearTreeRecursive(QuadTree* q) {
	if (q) {
		q->num_instance = 0;
		if (q->depth == 0) {
			if (q->has_leaf) {
				q->leaf.data.clear();
				q->has_leaf = false;
			}
		}
		else {
			for (int i = 0; i < 4; i++) {
				ClearTreeRecursive(q->partition[i]);
			}
		}
	}
}

QuadTree::~QuadTree() {
	DestroyRecursive(this);
}

void QuadTree::FillTree(Node2D* node) {
	Vector2 position = node->get_global_position();
	num_instance++;
	QuadTree* reference = this;
	int iter = depth;
	while (iter != 0) {
		reference = reference->partition[reference->FindQuadrant(position)];
		reference->num_instance++;
		iter--;
	}
	reference->has_leaf = true;
	reference->leaf.data.push_back(node);
}

QuadTree* QuadTree::GetData(Vector2 v) {
	if (num_instance > 0) {
		if (depth == 0) {
			if (has_leaf) {
				return this;
			}
			else {
				return nullptr;
			}
		}
		int quadrant = FindQuadrant(v);
		vector<int> order;
		order.push_back(quadrant);
		vector<int> normal = { 0,1,2,3 };
		for (auto it = normal.begin(); it != normal.end(); ++it) {
			if (std::find(order.begin(), order.end(), (*it)) == order.end()) {
				order.push_back(*it);
			}
		}
		auto iter = order.begin();
		while (iter != order.end()) {
			QuadTree* retval = partition[*iter]->GetData(v);
			if (retval) {
				return retval;
			}
			++iter;
		}
		return nullptr;
	}
	else {
		return nullptr;
	}

}

int QuadTree::FindQuadrant(Vector2 v) {
	if (v.x > pivot.x) {
		if (v.y > pivot.y) {
			return 0;
		}
		else {
			return 1;
		}
	}
	else {
		if (v.y > pivot.y) {
			return 2;
		}
		else {
			return 3;
		}
	}
}

void QuadTree::Print() {
	Godot::print("Print for pivot {0}", pivot);
	Godot::print("Instance is {0}", num_instance);
	if (depth == 0) {
		if (has_leaf) {
			Godot::print("leaf exist {0}", num_instance);
			for (int i = 0; i < leaf.data.size(); i++) {
				Godot::print(leaf.data[i]->get_global_position());
			}
		}
		else {
			Godot::print("leaf empty");
		}
	}
	else {
		if (num_instance != 0) {
			for (int i = 0; i < 4; i++) {
				Godot::print("Opening quadrant {0}", i);
				partition[i]->Print();
			}
		}
	}
}