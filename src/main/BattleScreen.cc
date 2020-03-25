#include "Screen.hh"

BattleScreen::BattleScreen(Core &core, ghc::filesystem::path const &path):
    ScriptedScreen(core, path),
    bullets(1024)
{
    this->setScript("_main");
    int noice = true;
    while (noice) {
        Bullet bullet;
        bullet.position.x = rand() % 1024;
        bullet.position.y = rand() % 600;
        bullet.velocity.x = rand() % 50 - 25;
        bullet.velocity.y = rand() % 50 - 25;
        bullet.gravity.x = rand() % 50 - 25;
        bullet.gravity.y = rand() % 50 - 25;
        noice = this->bullets.add(std::move(bullet));
    }
}

void BattleScreen::update(float delta, sf::RenderWindow &window) {
    if (this->coroutine) {
        this->runScript<float>(delta);
        for (Pool<Bullet>::Item &item: this->bullets.getItemsMutable()) {
            if (!item.alive) continue;
            item.content.update(delta);
        }
    } else {
        this->core.popScreen(this->getLastResponse());
    }
}

void BattleScreen::draw(sf::RenderTarget &target, int top) const {
    this->core.renderer.batch.clear();
    for (Pool<Bullet>::Item const &item: this->bullets.getItems()) {
        if (!item.alive) continue;
        this->core.renderer.node(item.content.position, false);
    }
    target.draw(this->core.renderer.batch);
}

void BattleScreen::onStart() {

}

void BattleScreen::onReveal(int response) {
    this->runScript<int>(response);
}

void BattleScreen::onKey(sf::Keyboard::Key key) {

}
