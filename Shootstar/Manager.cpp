#include "Manager.h"
#include "Commons.h"

Manager* Manager::manager_singleton;

void Manager::_register_methods() {
	register_method((char*)"_ready", &Manager::_ready);
	register_method((char*)"_process", &Manager::_process);
	register_method((char*)"on_timeout", &Manager::on_timeout);
}

void Manager::_init() {}

void Manager::_process(float delta) {
	CollectInjured();
}

void Manager::_ready() {
	Manager::manager_singleton = this;
	injured_tree = new QuadTree(x_dim, y_dim, 2, Vector2(0, 0), -1);
	healthy_tree = new QuadTree(x_dim, y_dim, 2, Vector2(0, 0), -1);
	delay = Timer::_new();
	delay->connect("timeout", this, "on_timeout");
	delay->set_wait_time(1.0f);
	delay->set_one_shot(false);
	add_child(delay);
	delay->start();
}

void Manager::on_timeout() {
	//CollectInjured();
}

Manager::Manager() {
}

Manager::~Manager() {

}

void Manager::CollectInjured() {
	QuadTree::ClearTreeRecursive(injured_tree);
	QuadTree::ClearTreeRecursive(healthy_tree);
	int i = 0;
	for (auto it = enemies.begin(); it != enemies.end(); ++it) {
		i++;
		//Godot::print("add enemy {0}", i);
		if (Object::cast_to<Enemy>(*it)->hp < 50) {
			/*if (Object::cast_to<Enemy>(*it)->healer < 2) {
				injured_tree->FillTree(*it);
			}*/
			injured_tree->FillTree(*it);
		}
		else {
			/*if (Object::cast_to<Enemy>(*it)->healer < 2) {
				healthy_tree->FillTree(*it);
			}*/
			healthy_tree->FillTree(*it);
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