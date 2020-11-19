#include "Player.h"
#include "Commons.h"

void Player::_register_methods() {
	register_method((char*)"_process", &Player::_process);
	register_method((char*)"_ready", &Player::_ready);
	register_method((char*)"on_timeout", &Player::on_timeout);
	register_property((char*)"bullet_frequency", &Player::bullet_frequency, 0.2f);
	register_property((char*)"hp", &Player::hp, 100);
	register_property((char*)"score", &Player::score, 0);
}

void Player::_init() {}

void Player::_ready() {
	ResourceLoader *loader = ResourceLoader::get_singleton();
	bullet = loader->load("res://scenes/Bullet.tscn");
	is_bullet_delayed = false;
	bullet_delay = Timer::_new();
	bullet_delay->connect("timeout", this, "on_timeout");
	bullet_delay->set_wait_time(bullet_frequency);
	bullet_delay->set_one_shot(false);
	add_child(bullet_delay);
	bullet_delay->start();
	score = 0;
}

Player::Player() {

}

Player::~Player() {

}

void Player::_process(float delta) {
	if (is_alive) {
		UpdateMotionFromInput();
		move_and_slide(motion);
		TriggerShoot();
	}
}

void Player::on_timeout() {
	is_bullet_delayed = false;
}

void Player::heal(int val) {
	if (hp + val <= 100) {
		hp += val;
	}
	else {
		hp = 100;
	}
}

void Player::hit(int val) {
	if (hp > val) {
		hp -= val;
	}
	else {
		kill();
	}
}

void Player::kill() {
	is_alive = false;
	hp = 0;
}

void Player::UpdateMotionFromInput() {
	motion = Vector2(0, 0);
	Input *i = Input::get_singleton();
	if (i->is_action_pressed("ui_up"))
		motion.y -= 1;
	if (i->is_action_pressed("ui_down"))
		motion.y += 1;
	if (i->is_action_pressed("ui_left"))
		motion.x -= 1;
	if (i->is_action_pressed("ui_right"))
		motion.x += 1;
	motion = motion.normalized() * speed;
}

void Player::TriggerShoot() {
	Input *i = Input::get_singleton();
	if (i->is_action_pressed("shoot") && !is_bullet_delayed) {
		Vector2 target = (get_global_mouse_position() - get_global_position()).normalized();
		real_t rotation = get_angle_to(get_global_mouse_position());
		ShootBullet(target, rotation);
		is_bullet_delayed = true;
	}
}

void Player::ShootBullet(Vector2 direction, real_t rotation) {
	Bullet *bullet_instance = Object::cast_to<Bullet>(bullet->instance());
	get_parent()->add_child(bullet_instance);
	bullet_instance->set_target(direction, rotation);
	bullet_instance->set_global_position(get_global_position());
}

void Player::AddScore(int val) {
	score += val;
}

