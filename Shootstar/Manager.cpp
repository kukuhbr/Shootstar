#include "Manager.h"
#include "Commons.h"

Manager* Manager::manager_singleton;

void Manager::_register_methods() {
	register_method((char*)"_ready", &Manager::_ready);
	register_method((char*)"_process", &Manager::_process);
}

void Manager::_init() {}

void Manager::_process(float delta) {
	CollectInjured();
}

void Manager::_ready() {
	Manager::manager_singleton = this;
}

Manager::Manager() {
}

Manager::~Manager() {

}

void Manager::CollectInjured() {
	//Clear healthy enemies
	auto iter = injured.begin();
	while (iter != injured.end()) {
		if (Object::cast_to<Enemy>(*iter)->hp == 50) {
			iter = injured.erase(iter);
		} else {
			++iter;
		}
	}
	//Push to Injured
	for (auto it = enemies.begin(); it != enemies.end(); ++it) {
		if (Object::cast_to<Enemy>(*it)->hp < 50) {
			injured.push_back(*it);
		}
	}
}

void Manager::append_child(Node2D* child, int type) {
	if (type == 1) { // Bullet
		bullets.push_back(Object::cast_to<Node2D>(child));
		//Godot::print("Bullets added: {0}", String::num(bullets.size()));
	}
	else if (type == 2) { // Enemy
		enemies.push_back(Object::cast_to<Node2D>(child));
		//Godot::print("Enemis added: {0}", String::num(enemies.size()));
	}
	else if (type == 3) { // Enemy
		medics.push_back(Object::cast_to<Node2D>(child));
		//Godot::print("Medics added: {0}", String::num(medics.size()));
	}
}

void Manager::remove_child(Node2D* child, int type) {
	if (type == 1) { // Bullet
		bullets.erase(std::remove(bullets.begin(), bullets.end(), child), bullets.end());
		//Godot::print("Bullets removed: {0}", String::num(bullets.size()));
	}
	else if (type == 2) { // Enemy
		enemies.erase(std::remove(enemies.begin(), enemies.end(), child), enemies.end());
		injured.erase(std::remove(injured.begin(), injured.end(), child), injured.end());
		//Godot::print("Enemies removed: {0}", String::num(enemies.size()));
	}
	else if (type == 3) { // Enemy
		medics.erase(std::remove(medics.begin(), medics.end(), child), medics.end());
		//Godot::print("Meidcs removed: {0}", String::num(medics.size()));
	}
}