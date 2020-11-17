#include "Bullet.h"
#include "Commons.h"

void Bullet::_register_methods() {
	register_method((char*)"_process", &Bullet::_process);
	register_method((char*)"_ready", &Bullet::_ready);
	register_method((char*)"on_timeout", &Bullet::on_timeout);
	register_method((char*)"on_body_entered", &Bullet::on_body_entered);
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
	lifetime->start();
	connect("body_entered", this, "on_body_entered");
}

void Bullet::set_target(Vector2 v, real_t r) {
	direction = v;
	rotation = r;
	set_rotation(r);
}

void Bullet::on_body_entered(PhysicsBody2D *n) {
	if (Object::cast_to<Enemy>(n)) {
		Object::cast_to<Enemy>(n)->hit();
		kill();
	}
}

void Bullet::_process(float delta) {
	Vector2 offset = direction * speed * delta;
	translate(offset);
}

void Bullet::on_timeout() {
	kill();
}

void Bullet::kill() {
	queue_free();
}