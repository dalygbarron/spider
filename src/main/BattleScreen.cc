#include "Screen.hh"

BattleScreen::BattleScreen(Core &core, ghc::filesystem::path const &path):
    ScriptedScreen(core, path)
{
    this->setScript("_main");
}

void BattleScreen::update(float delta, sf::RenderWindow &window) {
    if (this->coroutine) {
        this->runScript<float>(delta);
    } else {
        this->core.popScreen(this->getLastResponse());
    }
}

void BattleScreen::draw(sf::RenderTarget &target, int top) const {

}

void BattleScreen::onStart() {

}

void BattleScreen::onReveal(int response) {
    this->runScript<int>(response);
}

void BattleScreen::onKey(sf::Keyboard::Key key) {

}
