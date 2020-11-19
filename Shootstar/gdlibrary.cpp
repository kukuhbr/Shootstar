#include "Player.h"
#include "Enemy.h"
#include "EnemyMedic.h"
#include "Bullet.h"
#include "Level.h"
#include "Manager.h"

using namespace godot;

extern "C" void GDN_EXPORT godot_gdnative_init(godot_gdnative_init_options *o) {
	Godot::gdnative_init(o);
}

extern "C" void GDN_EXPORT godot_gdnative_terminate(godot_gdnative_terminate_options *o) {
	Godot::gdnative_terminate(o);
}

extern "C" void GDN_EXPORT godot_nativescript_init(void *handle) {
	Godot::nativescript_init(handle);
	register_class<Player>();
	register_class<Enemy>();
	register_class<EnemyMedic>();
	register_class<Bullet>();
	register_class<Level>();
	register_class<Manager>();
}