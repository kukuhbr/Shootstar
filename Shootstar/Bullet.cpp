#include "Bullet.h"
#include "Commons.h"

void Bullet::_register_methods() {
	register_method((char*)"_process", &Bullet::_process);
	register_method((char*)"_ready", &Bullet::_ready);
	register_method((char*)"on_timeout", &Bullet::on_timeout);
	register_method((char*)"on_body_entered", &Bullet::on_body_entered);
	register_property((char*)"speed", &Bullet::speed, 600);
}

Bullet::Bullet() {
}

Bullet::~Bullet() {
}

void Bullet::_init() {}

void Bullet::_ready() {
	Manager::manager_singleton->append_child(this, 1);

	lifetime = Timer::_new();
	lifetime->connect("timeout", this, "on_timeout");
	lifetime->set_wait_time(4.0);
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
		Object::cast_to<Enemy>(n)->hit(10);
		kill();
	}
	if (Object::cast_to<EnemyMedic>(n)) {
		Object::cast_to<EnemyMedic>(n)->hit(10);
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
	Manager::manager_singleton->remove_child(this, 1);
	queue_free();
}