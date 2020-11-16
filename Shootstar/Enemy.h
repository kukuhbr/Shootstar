#pragma once

#include <Godot.hpp>
#include <KinematicBody2D.hpp>
#include <Node2D.hpp>

namespace godot {
	class Enemy : public KinematicBody2D
	{
		// Godot structure
	private:
		GODOT_CLASS(Enemy, KinematicBody2D)
	public:
		static void _register_methods();
		void _init();
		void _ready();
		void _process(float delta);

		Enemy();
		~Enemy();
		// Gameplay variables
	private:
		Vector2 motion;

	public:
		int speed = 250;
		Node2D *player;
		// Gameplay methods
	private:
	public:
		void FollowPlayer();
	};
}
