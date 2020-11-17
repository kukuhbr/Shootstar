#pragma once

#include <Godot.hpp>
#include <KinematicBody2D.hpp>
#include <KinematicCollision2D.hpp>
#include <PackedScene.hpp>
#include <ResourceLoader.hpp>
#include <SceneTree.hpp>
#include <Input.hpp>
#include <InputEvent.hpp>
#include <InputEventMouseButton.hpp>
#include <GlobalConstants.hpp>
#include <Timer.hpp>
#include "Bullet.h"

namespace godot {
	class Player : public KinematicBody2D
	{
	// Godot structure
	private:
		GODOT_CLASS(Player, KinematicBody2D)
	public:
		static void _register_methods();
		void _init();
		void _input(InputEvent*);
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
		bool is_alive = true;

	public:
		real_t bullet_frequency = 0.2;
		int hp = 100;
		const int speed = 300;
		
	// Gameplay methods
	private:

	public:
		void on_timeout();
		void UpdateMotionFromInput();
		void HandleMouse(InputEventMouseButton*);
		void ShootBullet(Vector2, real_t);
		void TriggerShoot();
		void ProcessCollision();
		void kill();
	};
}


