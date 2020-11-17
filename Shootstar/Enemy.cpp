#include "Enemy.h"
#include "Commons.h"

void Enemy::_register_methods() {
	register_method((char*)"_process", &Enemy::_process);
	register_method((char*)"_ready", &Enemy::_ready);
	register_method((char*)"_init", &Enemy::_init);
}

void Enemy::_init() {
	Godot::print("init");
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
	Godot::print(get_global_position());
	Godot::print(player->get_global_position());
}

Enemy::Enemy() {

}

Enemy::~Enemy() {

}

void Enemy::_process(float delta) {
	FollowPlayer();
	move_and_slide(motion);
}

void Enemy::FollowPlayer() {
	motion = Vector2(0, 0);
	Vector2 result = player->get_global_position() - get_global_position();
	motion = result.normalized() * speed;
	//Godot::print(result.normalized());
	//player pos - enemy pos, normalize, * speed
}
