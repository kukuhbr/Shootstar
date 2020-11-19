#include "Level.h"
#include "Commons.h"

void Level::_register_methods() {
	register_method((char*)"_ready", &Level::_ready);
	register_method((char*)"_process", &Level::_process);
	register_method((char*)"on_timeout", &Level::on_timeout);

	register_property((char*)"delay_time", &Level::delay_time, 5.0f);
	register_property((char*)"delay_shrink", &Level::delay_shrink, .08f);
	register_property((char*)"enemy_count", &Level::enemy_count, 3.0f);
	register_property((char*)"enemy_grow", &Level::enemy_grow, .3f);
}

void Level::_init() {}

void Level::_ready() {
	ResourceLoader *loader = ResourceLoader::get_singleton();
	enemy = loader->load("res://scenes/Enemy.tscn");
	wave_delay = Timer::_new();
	wave_delay->connect("timeout", this, "on_timeout");
	wave_delay->set_wait_time(delay_time);
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
	SummonEnemyWave(enemy_count);
	delay_time *= delay_shrink;
	enemy_count += enemy_grow;
	wave_delay->set_wait_time(delay_time);
	wave_delay->start();
}

void Level::SummonEnemyWave(int n) {
	for (int i = 0; i < n; i++) {
		Ref<RandomNumberGenerator> rng = RandomNumberGenerator::_new();
		rng->randomize();
		int rand_x = rng->randi_range(20-x_dim, x_dim-20);
		int rand_y = rng->randi_range(20-y_dim, y_dim-20);
		Vector2 pos = Vector2(rand_x, rand_y);
		SummonEnemy(pos, 0);
	}
}

void Level::SummonEnemy(Vector2 pos, int type) {
	total_enemy_count += 1;
	Enemy* enemy_instance = Object::cast_to<Enemy>(enemy->instance());
	Manager::manager_singleton->append_child(enemy_instance, 2);
	add_child(enemy_instance);
	enemy_instance->set_global_position(pos);
}