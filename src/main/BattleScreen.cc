#include "Screen.hh"

BattleScreen::BattleScreen(Core &core, ghc::filesystem::path const &path):
    ScriptedScreen(core, path),
    bullets(1024)
{
    this->script["_addBullet"] = [this](
        int type,
        float x,
        float y,
        float vX,
        float vY,
        float gX,
        float gY
    ) -> int {
        Bullet bullet;
        bullet.position.x = x;
        bullet.position.y = y;
        bullet.velocity.x = vX;
        bullet.velocity.y = vY;
        bullet.gravity.x = gX;
        bullet.gravity.y = gY;
        Pool<Bullet>::Item *item = this->bullets.add(bullet);
        if (item) return item->id;
        return 0;
    };
    this->background.setPosition(sf::Vector2f(128, 0));
    this->background.setSize(sf::Vector2f(512, 600));
    this->background.setFillColor(sf::Color::Green);
    this->setScript("_main");
}

void BattleScreen::update(float delta, sf::RenderWindow &window) {
    if (this->coroutine) {
        this->runScript<float>(delta);
        for (Pool<Bullet>::Item &item: this->bullets.getItemsMutable()) {
            if (!item.alive) continue;
            item.content.live.update(delta);
            if (!this->bounds.contains(item.content.live.position)) {
                this->bullets.remove(item.id);
            }
        }
    } else {
        this->core.popScreen(this->getLastResponse());
    }
}

void BattleScreen::draw(sf::RenderTarget &target, int top) const {
    // Draw the background.
    target.draw(this->background);
    // Draw the bullets.
    this->core.renderer.batch.clear();
    for (Pool<Bullet>::Item const &item: this->bullets.getItems()) {
        if (!item.alive) continue;
        this->core.renderer.node(item.content.live.position, false);
    }
    // Draw the GUI.
    this->core.renderer.panel(sf::FloatRect(0, 0, 128, 600));
    this->core.renderer.panel(sf::FloatRect(640, 0, 384, 600));
    target.draw(this->core.renderer.batch);
}

void BattleScreen::onStart() {

}

void BattleScreen::onReveal(int response) {
    this->runScript<int>(response);
}

void BattleScreen::onKey(sf::Keyboard::Key key) {

}
