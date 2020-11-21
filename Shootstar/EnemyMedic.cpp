#include "EnemyMedic.h"
#include "Commons.h"

void EnemyMedic::_register_methods() {
	register_method((char*)"_process", &EnemyMedic::_process);
	register_method((char*)"_ready", &EnemyMedic::_ready);
	register_method((char*)"on_timeout", &EnemyMedic::on_timeout);
	register_method((char*)"_init", &EnemyMedic::_init);
	register_property((char*)"hp", &EnemyMedic::hp, 30);
	register_property((char*)"heal_range", &EnemyMedic::heal_range, 80.0f);
	register_property((char*)"heal_frequency", &EnemyMedic::heal_frequency, 0.25f);
	register_property((char*)"heal_power", &EnemyMedic::heal_power, 5);
}

void EnemyMedic::_init() {

}

void EnemyMedic::_ready() {
	Manager::manager_singleton->append_child(this, 3);

	Node2D *n;
	int64_t childCount = get_parent()->get_child_count();
	for (int64_t i = 0; i < childCount; i++) {
		n = (Node2D*)get_parent()->get_child(i);
		if (n->get_name() == "Player") {
			player = n;
		}
	}

	heal_delay = Timer::_new();
	heal_delay->connect("timeout", this, "on_timeout");
	heal_delay->set_wait_time(heal_frequency);
	heal_delay->set_one_shot(false);
	add_child(heal_delay);
	heal_delay->start();
}

EnemyMedic::EnemyMedic() {

}

EnemyMedic::~EnemyMedic() {

}

void EnemyMedic::on_timeout() {
	have_heal = false;
}

void EnemyMedic::_process(float delta) {
	FindTarget();
	if (is_target_exist()) {
		FollowTarget();
		HealTarget();
		CheckTargetHealth();
	}
	move_and_slide(motion);
}

void EnemyMedic::hit(int val) {
	if (hp > val) {
		hp -= val;
		heal_power += 5;
		Object::cast_to<Player>(player)->AddScore(10);
	}
	else {
		Object::cast_to<Player>(player)->AddScore(30);
		Object::cast_to<Player>(player)->heal(10);
		kill();
	}
}

void EnemyMedic::kill() {
	hp = 0;
	Manager::manager_singleton->remove_child(this, 3);
	queue_free();
}

void EnemyMedic::FindTarget() {
	//Find closest injured enemy
	QuadTree* tree = Manager::manager_singleton->injured_tree;
	if (tree) {
		QuadTree* closest_tree = tree->GetData(get_global_position());
		if (closest_tree) {
			if (!closest_tree->leaf.data.empty()) {
				target = closest_tree->leaf.data.front();
				//Godot::print("Found my target!");
			}
			else {
				//Godot::print("no target");
				target = nullptr;
			}
		}
	}
}

bool EnemyMedic::is_target_exist() {
	if (target == nullptr) {
		return false;
	}
	std::vector<Node2D*> enemy = Manager::manager_singleton->enemies;
	if (std::find(enemy.begin(), enemy.end(), target) != enemy.end()) {
		return true;
	}
	else {
		target = nullptr;
		return false;
	}
}

real_t EnemyMedic::distance_to(Node2D* t) {
	return get_global_position().distance_to(t->get_global_position());
}

void EnemyMedic::FollowTarget() {
	motion = Vector2(0, 0);
	if (distance_to(target) > (heal_range - 20)) {
		Vector2 result = target->get_global_position() - get_global_position();
		motion = result.normalized() * speed;
	}
}

void EnemyMedic::HealTarget() {
	if (!have_heal && distance_to(target) <= heal_range) {
		have_heal = true;
		Object::cast_to<Enemy>(target)->heal(heal_power);
	}
}

void EnemyMedic::CheckTargetHealth() {
	if (Object::cast_to<Enemy>(target)->hp == Object::cast_to<Enemy>(target)->max_hp) {
		target = nullptr;
	}
}
