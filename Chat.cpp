#include "Chat.h"

Chat::Chat() : window(nullptr), scroll_offset(0.0f)
{
    window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Chat Room");
    window->setFramerateLimit(30);

    ImGui::SFML::Init(*window);
    sf::Clock delta_clock;

    std::array<std::string, MESSAGE_HISTORY_LENGTH> message_history_seen;
    uint8_t message_index = 0; // Everytime message adjust by message_index = (message_index + 1) % MESSAGE_HISTORY_LENGTH;

    std::vector<std::array<std::string, MESSAGE_HISTORY_LENGTH>> all_message_history;

    id_map.insert(std::make_pair(0, "Joel"));
    id_map.insert(std::make_pair(1, "Simulated Bot"));
}

Chat::~Chat()
{
    ImGui::SFML::Shutdown();
    delete window;
}

void Chat::Run()
{
    while (window->isOpen())
    {
        HandleEvents();
        Render();
    }
}

void Chat::HandleEvents() {
    while (window->pollEvent(event)) {
        switch (event.type) {
        case sf::Event::Closed:
            window->close();
            break;
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Escape) {
                window->close();
            }
            if (event.key.code == sf::Keyboard::SemiColon) {
                //PrintMessageHistory();
            }
            break;
        case sf::Event::MouseWheelScrolled:
            if (event.mouseWheelScroll.delta > 0) {
                scroll_offset -= SCROLL_SPEED; // Scroll up
            }
            else {
                scroll_offset += SCROLL_SPEED; // Scroll down
            }
            break;
        }

        ImGui::SFML::ProcessEvent(event);
        HandleInputBuffer();
    }
}


void Chat::HandleInputBuffer()
{
    sf::Time deltaTime = delta_clock.restart();

    ImGui::SFML::Update(*window, deltaTime);

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(TEXT_BOX_WIDTH, TEXT_BOX_HEIGHT));

    ImGui::Begin("Chat Input", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    ImGui::InputText("Message", temp_input_text, sizeof(temp_input_text));

    ImGui::SameLine();
    if (ImGui::Button("Back to Top")) {
        scroll_offset = 0.0f;
    }

    if (ImGui::Button("Clear Message History")) {
        ClearMessageHistory();
    }

    if (ImGui::IsKeyPressed(ImGuiKey_Enter) && temp_input_text[0] != '\0')
    {
        memcpy(input_text, temp_input_text, sizeof(temp_input_text));
        temp_input_text[0] = '\0';

        if (clear_pending) {
            for (auto& message : message_history_seen) {
                message.first.clear();
                message.second = 0;
            }
            clear_pending = false;
        }

        message_history_seen[message_index].first = input_text;
        message_history_seen[message_index].second = 0;

        message_index = (message_index + 1);

        if (message_index == MESSAGE_HISTORY_LENGTH) {
            all_message_history.push_back(message_history_seen);
            clear_pending = true;
            message_index = 0;
        }

        // Simulate response
  
        message_history_seen[message_index].first = "Hi";
        message_history_seen[message_index].second = 1;
        message_index = (message_index + 1);

        if (message_index == MESSAGE_HISTORY_LENGTH) {
            all_message_history.push_back(message_history_seen);
            clear_pending = true;
            message_index = 0;
         }
    }

    ImGui::End();
    window->clear(sf::Color::Black);
    ImGui::SFML::Render(*window);
}

void Chat::Render()
{
    DrawText(input_text);
    window->display();
}

void Chat::DrawText(char* text_buffer) {
    sf::Font font;
    if (!font.loadFromFile("Fonts/Times New Roman.ttf")) {
        std::cout << "Could not load font" << std::endl;
        return;
    }

    float y_offset = CANVAS_STARTING_HEIGHT - scroll_offset + TOP_MARGIN;

    bool draw_flag = true;
    for (size_t i = 0; i < MESSAGE_HISTORY_LENGTH; ++i) {
        if (message_history_seen[i].first.empty()) continue;

        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);

        std::string wrapped_text = WrapText(message_history_seen[i].first, font, BUBBLE_WIDTH);
        text.setString(wrapped_text);

        sf::FloatRect bounds = text.getLocalBounds();
        float text_height = bounds.height + bounds.top;

        int lineCount = std::count(wrapped_text.begin(), wrapped_text.end(), '\n') + 1;
        float bubble_height = (text_height * lineCount) + 20;
        sf::Vector2f bubble_size(BUBBLE_WIDTH, bubble_height);

        float bubble_x;
        float bubble_y = y_offset;

        // Determine bubble position
        if (message_history_seen[i].second == 0) {
            bubble_x = 20.0f;
        }
        else {
            bubble_x = float(WINDOW_WIDTH - bubble_size.x - 20.0f);
        }

        // Check if the bubble is within the drawable area
        if (bubble_y + bubble_height < TEXT_BOX_HEIGHT + bubble_height) {
            draw_flag = false;
        }

        if (bubble_y + bubble_height < 0 || bubble_y > WINDOW_HEIGHT) {
            y_offset += bubble_height + TEXT_GAP;
            draw_flag = false;
            continue;
        }

        // Create the bubble
        sf::RectangleShape bubble(bubble_size);
        bubble.setFillColor(sf::Color(30, 30, 30, 200));
        bubble.setPosition(bubble_x, bubble_y);
        bubble.setOutlineThickness(2.0f);
        bubble.setOutlineColor(sf::Color::White);

        // Position the text inside the bubble
        text.setPosition(bubble_x + 10, bubble_y + 10);

        // Add sender's name
        sf::Text name_text;
        name_text.setFont(font);
        name_text.setCharacterSize(24);
        name_text.setFillColor(sf::Color::Cyan);

        std::string sender_name = id_map[message_history_seen[i].second];
        name_text.setString(sender_name);

        sf::FloatRect name_bounds = name_text.getLocalBounds();

        float name_x;
        float name_y = bubble_y - name_bounds.height + (bubble_height/2); // Slightly above the bubble

        if (message_history_seen[i].second == 0) { // Left-aligned bubble
            name_x = bubble_x + bubble_size.x + (BUBBLE_WIDTH/4); // To the right of the bubble
        }
        else { // Right-aligned bubble
            name_x = bubble_x - name_bounds.width - (BUBBLE_WIDTH/4); // To the left of the bubble
        }

        name_text.setPosition(name_x, name_y);

        // Draw the bubble, text, and sender's name if drawable
        if (draw_flag) {
            window->draw(bubble);
            window->draw(text);
            window->draw(name_text);
        }

        y_offset += bubble_height + TEXT_GAP;
        draw_flag = true;
    }
}


std::string Chat::WrapText(const std::string& text, const sf::Font& font, float maxWidth)
{
    std::string wrappedText;
    std::string word;

    std::istringstream stream(text);
    float currentLineWidth = 0.0f;

    const float padding = 10.0f;
    float effectiveWidth = maxWidth - 2 * padding;

    while (stream >> word) {
        sf::Text testText;
        testText.setFont(font);
        testText.setCharacterSize(24);
        testText.setString(word);

        float wordWidth = testText.getLocalBounds().width;

        if (currentLineWidth + wordWidth > effectiveWidth) {
            wrappedText += "\n";
            currentLineWidth = 0.0f;
        }

        if (!wrappedText.empty() && wrappedText.back() != '\n') {
            wrappedText += " ";
        }

        wrappedText += word;
        currentLineWidth += wordWidth + 5;
    }

    return wrappedText;
}
void Chat::PrintMessageHistory() 
{
    std::cout << "VISIBLE MESSAGE HISTORY:" << std::endl;

    for (uint8_t i = 0; i < MESSAGE_HISTORY_LENGTH; i++) {
        std::cout << message_history_seen[i].first << std::endl;
    }

    std::cout << "ALL MESSAGES HISTORY: " << std::endl;

    for (uint8_t i = 0; i < all_message_history.size(); i++) {
        for (uint8_t j = 0; j < MESSAGE_HISTORY_LENGTH; j++) {
            std::cout << all_message_history[i][j].first << std::endl;
        }
    }

}

void Chat::ClearMessageHistory() {
    message_history_seen.fill(std::make_pair("", 0));
    all_message_history.clear();
}