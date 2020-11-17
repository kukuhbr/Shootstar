#pragma once

#include <Godot.hpp>
#include <Node2D.hpp>
#include <ResourceLoader.hpp>
#include <PackedScene.hpp>
#include <Timer.hpp>
#include "Player.h"
#include "Enemy.h"

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
		Timer* wave_delay;
	public:
	// Gameplay methods
	private:
	public:
		void on_timeout();
		void SummonEnemy(Vector2, int);
		void SummonEnemyWave(int);
	};
}
