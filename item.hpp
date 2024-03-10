#pragma once

#include "include.hpp"

class Vector2;

struct Ammo {
	int capacity;
	int contents;
};

class Item {
public:
	int get_amount ( CSharedMemComms* drv );
	int get_uid ( CSharedMemComms* drv );
	int get_iid ( CSharedMemComms* drv );
	bool is_weapon( CSharedMemComms* drv );
	uintptr_t get_base_projectile ( CSharedMemComms* drv );
	std::string get_name ( CSharedMemComms* drv );
	std::string get_nice_name ( CSharedMemComms* drv );
	std::string get_bullet_name ( CSharedMemComms* drv );
	Ammo get_bullet_amount ( CSharedMemComms* drv );
};

class ItemContainer {
public:
	Item* get_item ( CSharedMemComms* drv, int id );
};

class Inventory {
public:
	ItemContainer* get_belt ( CSharedMemComms* drv );
};