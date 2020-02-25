#include "Screen.hh"

KnobScreen::KnobScreen(Core &core, Knob *knob): Screen(core) {
    this->knob = knob;
}

KnobScreen::~KnobScreen() {
    if (this->knob) delete this->knob;
}

void KnobScreen::update(float delta, sf::RenderWindow &window) {
    if (!this->knob) {
        spdlog::warn("Popping control screen because control is null");
        this->core.popScreen(-1);
        return;
    }
    if (!this->knob->isBaked()) {
        spdlog::warn("Removing control screen because control is unbaked");
        this->core.popScreen(-1);
        return;
    }
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    this->mouse.x = mousePos.x;
    this->mouse.y = mousePos.y;
    Knob *result = this->knob->update(
        this->mouse,
        this->core.soundPlayer
    );
    if (result) this->core.popScreen(result->id);
}

void KnobScreen::draw(sf::RenderTarget &target, int top) const {
    this->core.renderer.batch.clear();
    if (this->knob) this->knob->draw(this->core.renderer);
    this->core.renderer.point(this->mouse, false);
    target.draw(this->core.renderer.batch);
}

int KnobScreen::isTransparent() const {
    return true;
}
