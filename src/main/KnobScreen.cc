#include "Screen.hh"

KnobScreen::KnobScreen(Core &core, Knob *knob): Screen(core) {
    this->knob = knob;
}

KnobScreen::~KnobScreen() {
    if (this->knob) delete this->knob;
}

void KnobScreen::update(sf::RenderWindow &window) {
    if (!this->knob) {
        spdlog::warn("Popping control screen because control is null");
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
    if (this->knob) this->knob->draw(target, this->core.renderer);
    this->core.renderer.cursor(this->mouse, Renderer::CursorType::pointer);
    target.draw(this->core.renderer.batch);
}

int KnobScreen::isTransparent() const {
    return true;
}
