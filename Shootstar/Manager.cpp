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
	delay = Timer::_new();
	delay->connect("timeout", this, "on_timeout");
	delay->set_wait_time(1.0f);
	delay->set_one_shot(false);
	add_child(delay);
	delay->start();
}

void Manager::on_timeout() {
	Godot::print("Lets delete");
	QuadTree::DestroyRecursive(injured_tree);
	Godot::print("delete success");
	is_make_tree = true;
}

Manager::Manager() {
}

Manager::~Manager() {

}

void Manager::CollectInjured() {
	//Clear healthy enemies
	/*auto iter = injured.begin();
	while (iter != injured.end()) {
		if (Object::cast_to<Enemy>(*iter)->hp == 50) {
			iter = injured.erase(iter);
		} else {
			++iter;
		}
	}*/
	injured.clear();
	//Push to Injured
	for (auto it = enemies.begin(); it != enemies.end(); ++it) {
		if (Object::cast_to<Enemy>(*it)->hp < 50) {
			injured.push_back(*it);
		}
	}
	//Create Injured QuadTree
	if (injured.size() > 0 && is_make_tree) {
		/*if (injured_tree) {
			delete injured_tree;
			injured_tree = nullptr;
		}*/
		is_make_tree = false;
		injured_tree = new QuadTree(x_dim, y_dim, 2, Vector2(0, 0), -1);
		for (auto it = injured.begin(); it != injured.end(); ++it) {
			injured_tree->FillTree(*it);
		}
		injured_tree->Print();
		Godot::print("Print is successful");
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