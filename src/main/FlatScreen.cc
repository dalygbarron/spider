#include "Screen.hh"

FlatScreen::FlatScreen(Core &core, char const *code):
    ScriptedScreen(core, code)
{

}

void FlatScreen::update(float delta, sf::RenderWindow &window) {
    if (this->coroutine) {
        // TODO: run script and save what it yields as it should yield
        // something on it's last run.
    } else {
        this->core.popScreen(false);
    }
}

void FlatScreen::draw(sf::RenderTarget &target, int top) const {

}

void FlatScreen::onStart() {

}

void FlatScreen::onReveal(int response) {

}

void FlatScreen::onKey(sf::Keyboard::Key key) {

}
