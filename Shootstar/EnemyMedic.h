#pragma once

#include <Godot.hpp>
#include <KinematicBody2D.hpp>
#include <Node2D.hpp>
#include "Enemy.h"

namespace godot {
	class EnemyMedic : public KinematicBody2D
	{
		// Godot structure
	private:
		GODOT_CLASS(Enemy, KinematicBody2D)
	public:
		static void _register_methods();
		void _init();
		void _ready();
		void _process(float delta);

		EnemyMedic();
		~EnemyMedic();
		// Gameplay variables
	private:
		Vector2 motion;
		int hp = 50;
		bool have_hit_player = false;

	public:
		int speed = 250;
		Node2D *player;
		// Gameplay methods
	private:
	public:
		void FollowPlayer();
		void hit(int);
		void kill();
		void ProcessCollision();
	};
}