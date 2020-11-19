#pragma once

#include <Godot.hpp>
#include <Node.hpp>
#include <Node2D.hpp>
#include <vector>
#include <algorithm>

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
	public:
		static Manager *manager_singleton;
		std::vector<Node2D*> bullets;
		std::vector<Node2D*> enemies;
	// Gameplay methods
	private:
	public:
		void append_child(Node2D*, int);
		void remove_child(Node2D*, int);
	};
}