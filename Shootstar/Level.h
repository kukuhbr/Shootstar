#pragma once

#include <Godot.hpp>
#include <Node2D.hpp>
#include <ResourceLoader.hpp>
#include <PackedScene.hpp>
#include <Timer.hpp>
#include <RandomNumberGenerator.hpp>
#include <vector>
#include "Player.h"
#include "Enemy.h"
#include "EnemyMedic.h"
#include "Manager.h"

namespace godot {
	class Level : public Node2D
	{
	// Godot structure
	private:
		GODOT_CLASS(Level, Node2D)
	public:
		static void _register_methods();
		void _init();
		void _ready();
		void _process(float delta);

		Level();
		~Level();

	// Gameplay variables
	private:
		Ref<PackedScene> enemy;
		Ref<PackedScene> enemy_medic;
		Timer* wave_delay;
		int x_dim = 1500;
		int y_dim = 900;
	public:
		float delay_time = 3.0f; // second
		float delay_shrink = .99f; // percentage
		float enemy_count = 3.0f;
		float enemy_grow = .3f; // percentage
	// Gameplay methods
	private:
	public:
		void on_timeout();
		void SummonEnemy(Vector2, int);
		void SummonEnemyWave(int);
	};
}
