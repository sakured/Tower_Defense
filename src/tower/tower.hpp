#include <iostream>
#pragma once

enum class TOWER_TYPE {
    BOW,
    CROSSBOW,
    NONE
};

static const int NUMBER_OF_TOWER_TYPE = 2;

static const TOWER_TYPE ALL_TOWER_TYPES[NUMBER_OF_TOWER_TYPE] = {TOWER_TYPE::BOW, TOWER_TYPE::CROSSBOW};

static const std::string ALL_TOWER_SPRITES[NUMBER_OF_TOWER_TYPE] = {"images/Towers/pirate1.png", "images/Towers/pirate2.png"};

struct tower {
    TOWER_TYPE type;

    int pos_x;
    int pos_y;

    unsigned int cost;
    unsigned int damage;
    unsigned int range;
    unsigned int pace;

    bool in_range(std::pair<int, int> pos_enemy);
};

tower create_tower(int pos_x, int pos_y, TOWER_TYPE type);

std::string get_sprite_from_type(TOWER_TYPE type);

std::string tower_type_to_string(TOWER_TYPE type);

int get_cost_from_type(TOWER_TYPE type);