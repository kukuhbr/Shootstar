#include "Enemy.h"
#include "Commons.h"

void Enemy::_register_methods() {
	register_method((char*)"_process", &Enemy::_process);
	register_method((char*)"_ready", &Enemy::_ready);
	register_method((char*)"_init", &Enemy::_init);
	register_property((char*)"hp", &Enemy::hp, 50);
	register_property((char*)"speed", &Enemy::speed, 250);
}

void Enemy::_init() {

}

void Enemy::_ready() {
	Manager::manager_singleton->append_child(this, 2);

	Node2D *n;
	int64_t childCount = get_parent()->get_child_count();
	for (int64_t i = 0; i < childCount; i++) {
		n = (Node2D*)get_parent()->get_child(i);
		if (n->get_name() == "Player") {
			player = n;
		}
	}
}

Enemy::Enemy() {

}

Enemy::~Enemy() {

}

void Enemy::_process(float delta) {
	FollowPlayer();
	move_and_slide(motion);
	ProcessCollision();
}

void Enemy::ProcessCollision() {
	for (int i = 0; i < get_slide_count(); i++) {
		Ref<KinematicCollision2D> col = get_slide_collision(i);
		Node *n = Object::cast_to<Node>(col->get_collider());
		if (Object::cast_to<Player>(n)) {
			Player *p = Object::cast_to<Player>(n);
			if (p->is_alive && !have_hit_player) {
				have_hit_player = true;
				Object::cast_to<Player>(n)->hit(10);
				kill();
			}
		}
	}
}

void Enemy::hit(int val) {
	if (hp > val) {
		hp -= val;
		speed += 35;
		Object::cast_to<Player>(player)->AddScore(10);
	}
	else {
		Object::cast_to<Player>(player)->AddScore(60);
		Object::cast_to<Player>(player)->heal(5);
		kill();
	}
}

void Enemy::heal(int val) {
	if (hp + val <= 50) {
		hp += val;
	}
	else {
		hp = 50;
	}
}

void Enemy::modify_healer(int val) {
	if (val == 1) {
		healer += 1;
	}
	else {
		healer -= 1;
	}
}

void Enemy::kill() {
	hp = 0;
	Manager::manager_singleton->remove_child(this, 2);
	queue_free();
}

void Enemy::FollowPlayer() {
	motion = Vector2(0, 0);
	Vector2 result = player->get_global_position() - get_global_position();
	motion = result.normalized() * speed;
}
