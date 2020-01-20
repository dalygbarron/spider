#include "Screen.hh"
#include "Const.hh"
#include "Util.hh"

Screen::Transition::Transition(Screen *screen, int kill):
screen(screen),
kill(kill) {
    // Does not very much.
}

Screen::Screen():
window(sf::VideoMode(Const::WIDTH, Const::HEIGHT), "screne") {
    this->mouse = sf::Mouse::getPosition();
    for (int i = 0; i < sf::Mouse::Button::ButtonCount; i++) {
        this->buttons[i] = 0;
    }
    this->view.setSize(sf::Vector2f(Const::WIDTH, Const::HEIGHT));
    this->view.setCenter(sf::Vector2f(Const::WIDTH / 2, Const::HEIGHT / 2));
    this->window.resetGLStates();
}

Screen::~Screen() {
    // Does nothing.
}

sf::RenderWindow &Screen::getWindow() {
    return this->window;
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
            this->view = Util::getLetterboxView(
                this->view,
                sf::Vector2i(event.size.width, event.size.height)
            );
        } else if (!ImGui::GetIO().WantCaptureMouse) {
            if (event.type == sf::Event::MouseButtonPressed) {
                this->onClick(event.mouseButton.button);
                buttons[event.mouseButton.button] = true;
            } else if (event.type == sf::Event::MouseButtonReleased) {
                buttons[event.mouseButton.button] = false;
            } else if (event.type == sf::Event::MouseMoved) {
                for (int i = 0; i < sf::Mouse::Button::ButtonCount; i++) {
                    if (buttons[i]) {
                        this->onDrag((sf::Mouse::Button)i, sf::Vector2f(
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
    Screen::Transition transition = this->logic(delta.asSeconds());
    // Render.
    window.setView(this->view);
    window.clear();
    window.draw(*this);
    ImGui::SFML::Render(window);
    window.display();
    return transition;
}

void Screen::onClick(sf::Mouse::Button button) {
    // Do nothing by default.
}

void Screen::onDrag(sf::Mouse::Button button, sf::Vector2f delta) {
    // Do nothing by default.
}
