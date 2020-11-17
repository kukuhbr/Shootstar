#include "Player.h"
#include "Commons.h"

void Player::_register_methods() {
	register_method((char*)"_process", &Player::_process);
	register_method((char*)"_input", &Player::_input);
	register_method((char*)"_ready", &Player::_ready);
	register_method((char*)"on_timeout", &Player::on_timeout);
	register_property((char*)"bullet_frequency", &Player::bullet_frequency, 0.2f);
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
}

Player::Player() {

}

Player::~Player() {

}

void Player::_input(InputEvent *e) {
	if (e->get_class() == String("InputEventMouseButton")) {
		HandleMouse((InputEventMouseButton*)e);
	}
}

void Player::HandleMouse(InputEventMouseButton *e) {
	/*int64_t buttonIndex = e->get_button_index();
	if (e->is_pressed()) {
		if (buttonIndex == GlobalConstants::BUTTON_LEFT) {
			Vector2 target = (get_global_mouse_position() - get_global_position()).normalized();
			real_t rotation = get_angle_to(get_global_mouse_position());
			Godot::print("Handle Mouse");
			Godot::print(target);
			ShootBullet(target, rotation);
		}
	}*/
}

void Player::_process(float delta) {
	if (is_alive) {
		UpdateMotionFromInput();
		move_and_slide(motion);
		TriggerShoot();
		ProcessCollision();
	}
}

void Player::on_timeout() {
	is_bullet_delayed = false;
}

void Player::ProcessCollision() {
	for (int i = 0; i < get_slide_count(); i++) {
		Ref<KinematicCollision2D> col = get_slide_collision(i);
		Node *n = Object::cast_to<Node>(col->get_collider());
		Godot::print("I collided with", n->get_name());
		if (Object::cast_to<Enemy>(n)) {
			Godot::print("collision with enemy");
			Object::cast_to<Enemy>(n)->kill();
			hp -= 10;
			if (hp <= 0) {
				kill();
			}
		}
	}
}

void Player::kill() {
	is_alive = false;
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

