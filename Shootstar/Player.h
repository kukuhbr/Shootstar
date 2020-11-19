#pragma once

#include <Godot.hpp>
#include <KinematicBody2D.hpp>
#include <PackedScene.hpp>
#include <ResourceLoader.hpp>
#include <SceneTree.hpp>
#include <Input.hpp>
#include <Timer.hpp>
#include "Bullet.h"
#include "Level.h"
#include "Manager.h"

namespace godot {
	class Player : public KinematicBody2D
	{
	// Godot structure
	private:
		GODOT_CLASS(Player, KinematicBody2D)
	public:
		static void _register_methods();
		void _init();
		void _ready();
		void _process(float);

		Player();
		~Player();

	// Gameplay variables
	private:
		Vector2 motion;
		Ref<PackedScene> bullet;
		bool is_bullet_delayed;
		Timer* bullet_delay;

	public:
		real_t bullet_frequency = 0.2;
		int hp = 100;
		bool is_alive = true;
		const int speed = 300;
		int score;
		
	// Gameplay methods
	private:
		void UpdateMotionFromInput();
		void TriggerShoot();
		void ShootBullet(Vector2, real_t);

	public:
		void on_timeout();
		void kill();
		void heal(int);
		void hit(int);
		void AddScore(int);
	};
}


