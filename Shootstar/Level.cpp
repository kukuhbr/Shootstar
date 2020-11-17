#include "Level.h"
#include "Commons.h"

void Level::_register_methods() {
	register_method((char*)"_ready", &Level::_ready);
	register_method((char*)"_process", &Level::_process);
	register_method((char*)"on_timeout", &Level::on_timeout);
}

void Level::_init() {}

void Level::_ready() {
	ResourceLoader *loader = ResourceLoader::get_singleton();
	enemy = loader->load("res://scenes/Enemy.tscn");
	wave_delay = Timer::_new();
	wave_delay->connect("timeout", this, "on_timeout");
	wave_delay->set_wait_time(2.0);
	wave_delay->set_one_shot(false);
	add_child(wave_delay);
	wave_delay->start();
}

void Level::_process(float delta) {
	
}

Level::Level() {
}

Level::~Level() {

}

void Level::on_timeout() {
	//Godot::print("timeout reached");
	SummonEnemyWave(1);
}

void Level::SummonEnemyWave(int n) {
	//Godot::print("summon wave called!");
	for (int i = 0; i < n; i++) {
		Vector2 pos = Vector2(0, 0); //Random
		SummonEnemy(pos, 0);
	}
}

void Level::SummonEnemy(Vector2 pos, int type) {
	//Godot::print("summon enemy called");
	Enemy* enemy_instance = (Enemy*)enemy->instance();
	add_child(enemy_instance);
	enemy_instance->set_global_position(pos);
}