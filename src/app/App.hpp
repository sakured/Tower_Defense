#pragma once
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <simpletext.h>
#include "screen.hpp"
#include <unordered_map>
#include "../tower/tower.hpp"
#include "../enemy/enemy.hpp"

class App {
public:
    App();

    void setup();
    void update();
    
    // GLFW callbacks binding
    void key_callback(int key, int scancode, int action, int mods);
    void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    void scroll_callback(double xoffset, double yoffset);
    void cursor_position_callback(double xpos, double ypos);
    void size_callback(int width, int height);

private:
    void render();

    int _width {};
    int _height {};
    double _previousTime {};
    float _viewSize {};

    std::pair<float, float> _in_pos;
    std::pair<float, float> _out_pos;

    // Add your variables here
    GLuint _texture {};

    std::vector<GLuint> _tower_sprites {};
    std::vector<GLuint> _enemy_sprites {};

    std::vector<enemy> _enemy_list {};
    std::vector<tower> _tower_list {};

    std::vector<std::pair<float, float>> _path;

    std::vector<Case> _tile_list {};

    SimpleText _TextRenderer {};

    int _money {100};

    int _life {150};

    bool _is_playing {false};

    TOWER_TYPE _new_tower_type {TOWER_TYPE::NONE};
};