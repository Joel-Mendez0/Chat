#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include<sstream>;

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>

#include <imgui.h>
#include <imgui-SFML.h>

class Chat {
public:
	Chat();
	~Chat();
	void Run();
	void HandleEvents();
	void HandleInputBuffer(); //Sets the input buffer based on input text
	void Render();
	void DrawText(char* text_buffer);
	std::string WrapText(const std::string& text, const sf::Font& font, float maxWidth);
private:
	sf::RenderWindow* window;
	sf::Clock delta_clock;
	const uint16_t WINDOW_WIDTH = 600;
	const uint16_t WINDOW_HEIGHT = 800;
	sf::Event event;
	char input_text[128] = "";
	char temp_input_text[128] = "";
	const uint16_t TEXT_BOX_WIDTH = 600;
	const uint16_t TEXT_BOX_HEIGHT = 100;
	const uint16_t CANVAS_STARTING_HEIGHT = TEXT_BOX_HEIGHT;
	const uint16_t TEXT_GAP = 20;
	const float BUBBLE_WIDTH = 500.0f;
};