#include "Enemy.h"
#include "Commons.h"

void Enemy::_register_methods() {
	register_method((char*)"_process", &Enemy::_process);
	register_method((char*)"_ready", &Enemy::_ready);
	register_method((char*)"_init", &Enemy::_init);
}

void Enemy::_init() {

}

void Enemy::_ready() {

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
			if (p->is_alive) {
				Object::cast_to<Player>(n)->hit(10);
				kill();
			}
		}
	}
}

void Enemy::hit(int val) {
	if (hp > val) {
		hp -= val;
		speed += 30;
	}
	else {
		kill();
	}
}

void Enemy::kill() {
	queue_free();
}

void Enemy::FollowPlayer() {
	motion = Vector2(0, 0);
	Vector2 result = player->get_global_position() - get_global_position();
	motion = result.normalized() * speed;
}
