#pragma once

#include <Godot.hpp>
#include <Node.hpp>
#include <Node2D.hpp>
#include <vector>
#include <algorithm>
#include <Timer.hpp>
#include "Enemy.h"
#include "QuadTree.h"

namespace godot {
	class Manager : public Node
	{
	// Godot structure
	private:
		GODOT_CLASS(Manager, Node)
	public:
		static void _register_methods();
		void _init();
		void _ready();
		void _process(float delta);

		Manager();
		~Manager();

		// Gameplay variables
	private:
		Timer* delay;
	public:
		static Manager *manager_singleton;
		std::vector<Node2D*> bullets;
		std::vector<Node2D*> enemies;
		std::vector<Node2D*> medics;
		std::vector<Node2D*> injured;
		QuadTree* injured_tree;
		QuadTree* healthy_tree;
		int x_dim = 1500;
		int y_dim = 900;
	// Gameplay methods
	private:
	public:
		void append_child(Node2D*, int);
		void remove_child(Node2D*, int);
		void CollectInjured();
		void on_timeout();
	};
}