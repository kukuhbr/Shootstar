#pragma once

#include <Godot.hpp>
#include <Area2D.hpp>
#include <Node2D.hpp>
#include <Timer.hpp>
#include "Enemy.h"

namespace godot {
	class Bullet : public Area2D
	{
		// Godot structure
	private:
		GODOT_CLASS(Bullet, Area2D)
	public:
		static void _register_methods();
		void _init();
		void _ready();
		void _process(float);

		Bullet();
		~Bullet();
		// Gameplay variables
	private:
		Vector2 direction;
		real_t rotation;
		const int speed = 600;
	public:
		Timer* lifetime;
		// Gameplay methods
	private:
	public:
		void set_target(Vector2, real_t);
		void on_body_entered(PhysicsBody2D *n);
		void on_timeout();
		void kill();
	};
}

