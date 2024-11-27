#include "Chat.h"

Chat::Chat() : window(nullptr)
{
    window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Chat Room");
    window->setFramerateLimit(120);

    ImGui::SFML::Init(*window);
    sf::Clock delta_clock;

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

void Chat::HandleEvents()
{
    while (window->pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
            window->close();
            break;
        case sf::Event::KeyPressed:
            if (event.key.code == sf::Keyboard::Escape)
            {
                window->close();
            }
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

    if (ImGui::IsKeyPressed(ImGuiKey_Enter) && temp_input_text[0] != '\0')
    {
        memcpy(input_text, temp_input_text, sizeof(temp_input_text));

        temp_input_text[0] = '\0';

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

void Chat::DrawText(char* text_buffer)
{
    sf::Font font;

    if (!font.loadFromFile("Fonts/Times New Roman.ttf")) {
        std::cout << "Could not load font" << std::endl;
    }

    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::White);

    const float bubble_width = BUBBLE_WIDTH;

    std::string wrapped_text = WrapText(text_buffer, font, bubble_width);

    text.setString(wrapped_text);

    float text_x = 20.0f;
    float text_y = CANVAS_STARTING_HEIGHT + TEXT_GAP;

    text.setPosition(text_x, text_y);

    float text_height = text.getLocalBounds().height;

    sf::RectangleShape bubble;
    bubble.setSize(sf::Vector2f(bubble_width, text_height + 20));
    bubble.setFillColor(sf::Color(30, 30, 30, 200));
    bubble.setPosition(text_x - 10, text_y - 10);

    window->draw(bubble);

    window->draw(text);
}
std::string Chat::WrapText(const std::string& text, const sf::Font& font, float maxWidth)
{
    std::string wrappedText;
    std::string word;

    std::istringstream stream(text);
    float currentLineWidth = 0.0f;

    while (stream >> word) {
        sf::Text testText;
        testText.setFont(font);
        testText.setCharacterSize(24);
        testText.setString(word);

        float wordWidth = testText.getLocalBounds().width;

        if (currentLineWidth + wordWidth > maxWidth) {
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
