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
		if (Object::cast_to<Enemy>(*it)->hp < 50) {
			injured_tree->FillTree(*it);
		}
		else {
			healthy_tree->FillTree(*it);
		}
	}
}

void Manager::append_child(Node2D* child, int type) {
	if (type == 1) { // Bullet
		bullets.push_back(Object::cast_to<Node2D>(child));
	}
	else if (type == 2) { // Enemy
		enemies.push_back(Object::cast_to<Node2D>(child));
	}
	else if (type == 3) { // Enemy
		medics.push_back(Object::cast_to<Node2D>(child));
	}
}

void Manager::remove_child(Node2D* child, int type) {
	if (type == 1) { // Bullet
		bullets.erase(std::remove(bullets.begin(), bullets.end(), child), bullets.end());
	}
	else if (type == 2) { // Enemy
		enemies.erase(std::remove(enemies.begin(), enemies.end(), child), enemies.end());
		injured.erase(std::remove(injured.begin(), injured.end(), child), injured.end());
	}
	else if (type == 3) { // Enemy
		medics.erase(std::remove(medics.begin(), medics.end(), child), medics.end());
	}
}