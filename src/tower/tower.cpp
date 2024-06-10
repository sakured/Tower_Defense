#include "tower.hpp"

/**
 * Crée une tour selon un type donné
*/
tower create_tower(int pos_x, int pos_y, TOWER_TYPE type) {
    tower tower {};

    switch (type) {
        case TOWER_TYPE::BOW:
            tower = {type, pos_x, pos_y, 50, 4, 10, 10};
            break;
            
        case TOWER_TYPE::CROSSBOW:
            tower = {type, pos_x, pos_y, 75, 8, 12, 5};
            break;

        default:
            break;
    }

    return tower;
}

std::string get_sprite_from_type(TOWER_TYPE type) {
    return ALL_TOWER_SPRITES[(int)type];
}
