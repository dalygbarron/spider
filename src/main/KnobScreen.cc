#include "Screen.hh"

KnobScreen::KnobScreen(Core &core, Control *control): Screen(core) {
    this->control = control;
}

KnobScreen::~KnobScreen() {
    delete this->control;
}

void KnobScreen::update(float delta, sf::RenderWindow &window) {
    if (!this->control) {
        spdlog::warn("Popping control screen because control is null");
        this->core.popScreen();
        return;
    }
    if (!this->control->isBaked()) {
        spdlog::warn("Removing control screen because control is unbaked");
        this->core.popScreen();
        return;
    }
    // TODO: let you click on stuff.
}

void KnobScreen::draw(sf::RenderTarget &target, int top) const {
    // TODO: draw the controls.
}
