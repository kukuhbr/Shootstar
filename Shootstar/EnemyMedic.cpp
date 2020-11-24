#include "EnemyMedic.h"
#include "Commons.h"
#include "Profiler.h"

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
	//Profiler::profiler_singleton->CreateNewProfile(0, "EnemyMedic:FindTarget");
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
	queue_free();
}

void EnemyMedic::FindTarget() {
	//time_point start = Profiler::profiler_singleton->Record(0);
	std::vector<Node2D*> all_enemy = Manager::manager_singleton->enemies;
	std::vector<Node2D*> injured;
	real_t min;
	Node2D *temp_target = nullptr;
	if (all_enemy.size() > 0) {
		//Find injured
		for (auto it = all_enemy.begin(); it != all_enemy.end(); ++it) {
			if (Object::cast_to<Enemy>(*it)->hp < 50) {
				injured.push_back(*it);
			}
		}
		if (injured.empty()) {
			injured = all_enemy;
		}
		//Find closest
		for (auto it = injured.begin(); it != injured.end(); ++it) {
			if (it == injured.begin()) {
				temp_target = (*it);
				min = distance_to(*it);
			}
			if (distance_to(*it) < min) {
				min = distance_to(*it);
				temp_target = (*it);
			}
		}
		target = temp_target;
		//time_point end = Profiler::profiler_singleton->Record(0);
		//Profiler::profiler_singleton->RecordTime(0, end, start);
	}
}

bool EnemyMedic::is_target_exist() {
	if (target == nullptr) {
		return false;
	}
	std::vector<Node2D*> temp = Manager::manager_singleton->enemies;
	if (std::find(temp.begin(), temp.end(), target) != temp.end()) {
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
