#pragma once

#include <Godot.hpp>
#include <KinematicBody2D.hpp>
#include <KinematicCollision2D.hpp>
#include <Node2D.hpp>
#include "Player.h"
#include "Manager.h"

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
		int hp = 50;
		bool have_hit_player = false;

	public:
		int speed = 250;
		float heal_range = 70;
		Node2D *player;
		// Gameplay methods
	private:
	public:
		void FollowPlayer();
		void heal(int);
		void hit(int);
		void kill();
		void ProcessCollision();
	};
}
