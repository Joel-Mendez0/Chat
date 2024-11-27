#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <array>
#include <sstream>
#include <unordered_map>

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
	void HandleInputBuffer();
	void Render();
	void DrawText(char* text_buffer);
	std::string WrapText(const std::string& text, const sf::Font& font, float maxWidth);
	void PrintMessageHistory();
	void ClearMessageHistory();
private:
	sf::RenderWindow* window;
	sf::Clock delta_clock;
	const uint16_t WINDOW_WIDTH = 600;
	const uint16_t WINDOW_HEIGHT = 800;
	sf::Event event;
	static const uint8_t MAX_MESSAGE_LENGTH = 25;
	char input_text[MAX_MESSAGE_LENGTH] = "";
	char temp_input_text[MAX_MESSAGE_LENGTH] = "";
	const uint16_t TEXT_BOX_WIDTH = 600;
	const uint16_t TEXT_BOX_HEIGHT = 75;
	const uint16_t CANVAS_STARTING_HEIGHT = TEXT_BOX_HEIGHT;
	const uint16_t TEXT_GAP = 20;
	const float BUBBLE_WIDTH = 300.0f;
	const float TOP_MARGIN = 10;
	static const uint8_t MESSAGE_HISTORY_LENGTH = 100;
	std::array<std::pair<std::string,uint8_t>, MESSAGE_HISTORY_LENGTH> message_history_seen;
	uint8_t message_index;
	std::vector<std::array<std::pair<std::string,uint8_t>, MESSAGE_HISTORY_LENGTH>> all_message_history;
	bool clear_pending = false;
	float scroll_offset;
	const float SCROLL_SPEED = 10.0f;
	std::unordered_map<uint8_t, std::string> id_map;
};