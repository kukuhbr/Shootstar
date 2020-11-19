#include "Manager.h"
#include "Commons.h"

Manager* Manager::manager_singleton;

void Manager::_register_methods() {
	register_method((char*)"_ready", &Manager::_ready);
}

void Manager::_init() {}

void Manager::_ready() {
	Manager::manager_singleton = this;
}

Manager::Manager() {
}

Manager::~Manager() {

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
}

void Manager::remove_child(Node2D* child, int type) {
	if (type == 1) { // Bullet
		bullets.erase(std::remove(bullets.begin(), bullets.end(), child), bullets.end());
		//Godot::print("Bullets removed: {0}", String::num(bullets.size()));
	}
	else if (type == 2) { // Enemy
		enemies.erase(std::remove(enemies.begin(), enemies.end(), child), enemies.end());
		//Godot::print("Enemies removed: {0}", String::num(enemies.size()));

	}
}