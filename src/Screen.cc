#include "Screen.hh"

Screen::Transition::Transition(Screen *screen, int kill):
screen(screen),
kill(kill) {
    // Does nothing else.
}

Screen::Screen():
window(sf::VideoMode(Const::WIDTH, Const::HEIGHT), "screne") {
    this->mouse = sf::Mouse::getPosition();
    for (int i = 0; i < sf::Mouse::Button::ButtonCount; i++) {
        this->buttons[i] = 0;
    }
}

Screen::~Screen() {
    // Does nothing.
}

Screen::Transition Screen::update() {
    // Handle Events.
    sf::Event event;
    while (this->window.pollEvent(event)) {
        ImGui::SFML::ProcessEvent(event);
        if (event.type == sf::Event::Closed) {
            this->window.close();
            return Screen::Transition(NULL, true);
        } else if (event.type == sf::Event::Resized) {
            view = Util::getLetterboxView(
                view,
                sf::Vector2i(event.size.width, event.size.height)
            );
        } else if (!ImGui::GetIO().WantCaptureMouse) {
            if (event.type == sf::Event::MouseButtonPressed) {
                screen->onClick(event.mouseButton.button);
                buttons[event.mouseButton.button] = true;
            } else if (event.type == sf::Event::MouseButtonReleased) {
                buttons[event.mouseButton.button] = false;
            } else if (event.type == sf::Event::MouseMoved) {
                for (int i = 0; i < sf::Mouse::Button::ButtonCount; i++) {
                    if (buttons[i]) {
                        screen->onDrag((sf::Mouse::Button)i, sf::Vector2f(
                            event.mouseMove.x - mouse.x,
                            event.mouseMove.y - mouse.y
                        ));
                    }
                }
                mouse.x = event.mouseMove.x;
                mouse.y = event.mouseMove.y;
            }
        }
    }
    // Update Screen.
    sf::Time delta = deltaClock.restart();
    ImGui::SFML::Update(window, delta);
    screen->update(delta.asSeconds(), window);
    // Render.
    window.setView(view);
    window.clear();
    window.draw(*screen);
    ImGui::SFML::Render(window);
    window.display();
    return Transition(NULL, false);
}

void Screen::onClick(sf::Mouse::Button button) {
    // Do nothing by default.
}

void Screen::onDrag(sf::Mouse::Button button, sf::Vector2f delta) {
    // Do nothing by default.
}
