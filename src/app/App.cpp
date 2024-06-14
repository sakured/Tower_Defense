#include "App.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <img/img.hpp>

#include <iostream>
#include <sstream>

#include "simpletext.h"
#include "utils.hpp"
#include "draw/GLHelpers.hpp"

#include <draw/draw.hpp>

#include "screen.hpp"
#include "../graph/graph.hpp"
#include "../tower/tower.hpp"
#include "../enemy/enemy.hpp"

App::App() : _previousTime(0.0), _viewSize(2.0)
{
    // Télécharge l'image de la map
    img::Image map {img::load(make_absolute_path("images/map.png", true), 3, true)};

    // Création de la liste de case
    _tile_list = create_case_list(map.data(), map.data_size());

    // Recherche du des positions du début et de la fin
    for (auto & tile : _tile_list) {
        if (tile.type == CASE_TYPE::START) _in_pos = {tile.pos_x, tile.pos_y};
        if (tile.type == CASE_TYPE::END) _out_pos = {tile.pos_x, tile.pos_y};
    }

    // Lecture du fichier ITD
    auto itd = read_ITD(MAP_FILE_NAME, IN, OUT, PATH);
    auto graph = itd.first;
    auto node_positions = itd.second;
    int start {};
    int end {};

    for (auto it {node_positions.begin()}; it != node_positions.end(); it++) {
        if (it->second == _in_pos) start = it->first;
        if (it->second == _out_pos) end = it->first;
    }

    _path = find_path(graph, node_positions, start, end);

    // Tower sprites
    for (auto & tower_type : ALL_TOWER_TYPES) {
        img::Image tower {img::load(make_absolute_path(get_sprite_from_type(tower_type).c_str(), true), 4, true)};
        _tower_sprites.push_back(loadTexture(tower));
    }

    // Enemy sprites
    for (auto & enemy_type : ALL_ENEMY_TYPES) {
        img::Image enemy {img::load(make_absolute_path(get_sprite_from_type(enemy_type).c_str(), true), 4, true)};
        _enemy_sprites.push_back(loadTexture(enemy));
    }
}

void App::setup()
{
    // Set the clear color to a nice blue
    glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

    // Setup the text renderer with blending enabled and white text color
    TextRenderer.ResetFont();
    TextRenderer.SetColor(SimpleText::TEXT_COLOR, SimpleText::Color::WHITE);
    TextRenderer.SetColorf(SimpleText::BACKGROUND_COLOR, 0.f, 0.f, 0.f, 0.f);
    TextRenderer.EnableBlending(true);
}

void App::update()
{
    const double currentTime{glfwGetTime()};
    const double elapsedTime{currentTime - _previousTime};
    _previousTime = currentTime;

    _angle += 10.0f * elapsedTime;
    // _angle = std::fmod(_angle, 360.0f);



    render();
}

void App::render()
{
    // Clear the color and depth buffers of the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Dessin de la map et des infos
    draw_map(_tile_list);
    draw_level_informations(1, TextRenderer, _width, _height, _money, _tower_sprites, _enemy_sprites); 

    // render exemple quad
    glColor3f(1.0f, 0.0f, 0.0f);

    // Using stringstream to format the string with fixed precision
    std::string angle_label_text{};
    std::stringstream stream{};
    stream << std::fixed << "Angle: " << std::setprecision(2) << _angle;
    angle_label_text = stream.str();
    TextRenderer.Label(angle_label_text.c_str(), _width / 2, _height - 4, SimpleText::CENTER);
    
    // Mise à jour du texte
    TextRenderer.Render();
}

void App::key_callback(int /*key*/, int /*scancode*/, int /*action*/, int /*mods*/)
{
}

void App::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    // Récupération des coordonnées du curseur lors du clic
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
		double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        float vertical_margin = 0.2 * _height / 2;
        float map = _height - 2 * vertical_margin;
        xpos -= (_width / 2) - (map / 2);
        ypos -= vertical_margin;
        std::pair<int,int> case_coordinate { (int)(xpos/(map/WIDTH_OF_MAP)) , (int)(ypos/(map/WIDTH_OF_MAP))};
        std::cout << "Case(" << case_coordinate.first << "," << case_coordinate.second << ") " << std::endl;
        
        // Création d'une tour BOW à la case cliquée
        if (case_coordinate.first >= 0 && case_coordinate.first < WIDTH_OF_MAP && case_coordinate.second >= 0 && case_coordinate.second < WIDTH_OF_MAP
        && !get_case_from_coordinates(case_coordinate.first, case_coordinate.second, _tile_list).is_occupied) {
            std::cout << "liiibre " << WIDTH_OF_MAP << std::endl;
            tower tower { create_tower(case_coordinate.first, case_coordinate.second, TOWER_TYPE::BOW) };
            _tile_list[get_id_from_position(case_coordinate.first, case_coordinate.second)].is_occupied = true;
            draw_tower(get_case_from_coordinates(case_coordinate.first, case_coordinate.second, _tile_list));
            _money -= tower.cost;
        }
	}
}

void App::scroll_callback(double /*xoffset*/, double /*yoffset*/)
{
}

void App::cursor_position_callback(double /*xpos*/, double /*ypos*/)
{
}

void App::size_callback(int width, int height) {
    _width = width;
    _height = height;

    // make sure the viewport matches the new window dimensions
    glViewport(0, 0, _width, _height);

    const float aspectRatio {_width / (float)_height};

    // Change the projection matrix
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (aspectRatio > 1.0f)
    {
        glOrtho(-_viewSize / 2.0f * aspectRatio, _viewSize / 2.0f * aspectRatio, -_viewSize / 2.0f, _viewSize / 2.0f, -1.0f, 1.0f);
    }
    else
    {
        glOrtho(-_viewSize / 2.0f, _viewSize / 2.0f, -_viewSize / 2.0f / aspectRatio, _viewSize / 2.0f / aspectRatio, -1.0f, 1.0f);
    }
}
