#pragma once

#include <Godot.hpp>
#include <KinematicBody2D.hpp>
#include <Node2D.hpp>
#include <RandomNumberGenerator.hpp>
#include <vector>
#include "Manager.h"
#include "Player.h"
#include "Enemy.h"
#include "QuadTree.h"

namespace godot {
	class EnemyMedic : public KinematicBody2D
	{
		// Godot structure
	private:
		GODOT_CLASS(EnemyMedic, KinematicBody2D)
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
		int hp = 30;
		bool have_heal = false;
		real_t heal_range = 80;

	public:
		Timer* heal_delay;
		int heal_power = 5;
		real_t heal_frequency = 0.5;
		int speed = 350;
		Node2D *player;
		Node2D *target;
		// Gameplay methods
	private:
		real_t distance_to(Node2D*);
		Node2D* get_enemy(QuadTree*, int);
		void set_target(Node2D*);
		void set_target_injured(Node2D*);
		void unset_target();
	public:
		void on_timeout();
		bool is_target_exist();
		bool is_injured_exist();
		void FindTarget();
		void FollowTarget();
		void HealTarget();
		void CheckTargetHealer();
		void hit(int);
		void kill();
	};
}