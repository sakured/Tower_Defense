#pragma once
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <simpletext.h>
#include "screen.hpp"
#include "graph\graph.hpp"

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

    // Add your variables here
    GLuint _texture {};
    float _angle {};

    std::vector<GLuint> _tower_sprites {};
    std::vector<GLuint> _enemy_sprites {};

    Graph::WeightedGraph _graph;

    std::vector<Case> _tile_list {};

    SimpleText TextRenderer {};

    int _money {100};

    int _life {150};

    bool _is_playing {false};
};