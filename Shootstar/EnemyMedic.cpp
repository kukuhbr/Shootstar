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
	register_property((char*)"speed", &EnemyMedic::speed, 330);
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
	motion = Vector2(0, 0);
	if (!is_target_exist() || is_injured_exist()) {
		FindTarget();
	}
	if (is_target_exist()) {
		FollowTarget();
		HealTarget();
		CheckTargetHealer();
	}
	if (motion == Vector2(0, 0)) {
		Godot::print("{0} is idle", get_global_position());
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
	unset_target();
	Manager::manager_singleton->remove_child(this, 3);
	queue_free();
}

void EnemyMedic::set_target_injured(Node2D* _target) {
	target = _target;
	if (is_target_exist()) {
		Object::cast_to<Enemy>(_target)->modify_healer(1);
		target = _target;
	}
	else {
		target = nullptr;
	}
}

void EnemyMedic::set_target(Node2D* _target) {
	target = _target;
	if (is_target_exist()) {
		if (Object::cast_to<Enemy>(_target)->healer < 2) {
			Object::cast_to<Enemy>(_target)->modify_healer(1);
		}
		else {
			Godot::print("sub fail healer is {0}", Object::cast_to<Enemy>(target)->healer);
			target = nullptr;
		}
	}
	else {
		target = nullptr;
	}
}

void EnemyMedic::unset_target() {
	if (is_target_exist()) {
		Object::cast_to<Enemy>(target)->modify_healer(0);
		//Godot::print("unsubbed {0}", Object::cast_to<Enemy>(target)->healer);
	}
	target = nullptr;
}

Node2D* EnemyMedic::get_enemy(QuadTree* source) {
	QuadTree* quad = source->GetData(get_global_position());
	if (quad) {
		if (!quad->leaf.data.empty()) {
			real_t min;
			Node2D* temp_target = nullptr;
			vector<Node2D*> candidate = quad->leaf.data;
			for (auto i = candidate.begin(); i != candidate.end(); ++i) {
				if (i == candidate.begin()) {
					min = distance_to(*i);
					temp_target = *i;
				}
				else {
					if (distance_to(*i) < min) {
						min = distance_to(*i);
						temp_target = *i;
					}
				}
			}
			/*if (temp_target) {
				Godot::print("from {0} get {1}", get_global_position(), temp_target->get_global_position());
				Godot::print("pivot {0}", quad->pivot);
				Godot::print("Quadreant {0} to {1}", quad->FindQuadrant(get_global_position()), quad->FindQuadrant(temp_target->get_global_position()));
			}*/
			return temp_target;
		}
		else {
			return nullptr;
		}
	}
}

void EnemyMedic::FindTarget() {
	unset_target();
	// Find closest injured enemy
	QuadTree* tree = Manager::manager_singleton->injured_tree;
	if (tree) {
		//Godot::print("find target, get injured enemy");
		if (get_enemy(tree)) {
			set_target_injured(get_enemy(tree));
		} else { Godot::print("no injured enemy"); }
		if (!is_target_exist()) {
			// Fallback to closest healthy enemy
			tree = Manager::manager_singleton->healthy_tree;
			if (tree) {
				set_target(get_enemy(tree));
				if (is_target_exist()) {
					Godot::print("{0} find target, get healthy enemy", get_global_position());
				}
				else {
					Godot::print("set healthy enemy fail");
				}
				
			} else { Godot::print("healthy tree not found"); }
		}// else { Godot::print("target is available"); }
	}
}


bool EnemyMedic::is_injured_exist() {
	return Manager::manager_singleton->enemies.size() > 0;
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

void EnemyMedic::CheckTargetHealer() {
	if (Object::cast_to<Enemy>(target)->hp == 50) {
		if (Object::cast_to<Enemy>(target)->healer > 2) {
			//Godot::print("target hp full, unset target");
			unset_target();
		}
	}
}
