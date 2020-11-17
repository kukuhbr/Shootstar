#include "Bullet.h"
#include "Commons.h"

void Bullet::_register_methods() {
	register_method((char*)"_process", &Bullet::_process);
	register_method((char*)"_ready", &Bullet::_ready);
	register_method((char*)"on_timeout", &Bullet::on_timeout);
}

Bullet::Bullet() {
}

Bullet::~Bullet() {
}

void Bullet::_init() {}

void Bullet::_ready() {
	lifetime = Timer::_new();
	lifetime->connect("timeout", this, "on_timeout");
	lifetime->set_wait_time(5.0);
	lifetime->set_one_shot(true);
	add_child(lifetime);
	Godot::print("bullet timer started");
	lifetime->start();
}

void Bullet::set_target(Vector2 v, real_t r) {
	Godot::print("bullet");
	Godot::print(v);
	Godot::print(String::num(r));
	Godot::print(direction);
	direction = v;
	rotation = r;
	set_rotation(r);
}

void Bullet::_process(float delta) {
	Vector2 offset = direction * speed * delta;
	translate(offset);
	//Godot::print("process");
	//Godot::print(direction);
	//Godot::print(offset);
}

void Bullet::on_timeout() {
	Godot::print("bullet cleared!");
	queue_free();
}