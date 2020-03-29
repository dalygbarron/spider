#include "Screen.hh"
#include "Const.hh"

BattleScreen::BattleScreen(Core &core, ghc::filesystem::path const &path):
    ScriptedScreen(core, path),
    bullets(Const::MAX_BULLETS),
    actors(Const::MAX_ACTORS)
{
    this->script["_addBullet"] = [this](
        unsigned int parentId,
        int type,
        float x,
        float y,
        float vX,
        float vY,
        float gX,
        float gY
    ) -> int {
        Bullet::Prototype const *prototype = this->core.getBulletPrototype(
            type
        );
        if (!prototype) return 0;
        Bullet bullet(prototype, parentId);
        bullet.position.x = x;
        bullet.position.y = y;
        bullet.velocity.x = vX;
        bullet.velocity.y = vY;
        bullet.gravity.x = gX;
        bullet.gravity.y = gY;
        // TODO: can we get it's location in the pool before we set everything?
        Pool<Bullet>::Item *item = this->bullets.add(bullet);
        if (item) return item->id;
        return 0;
    };
    this->script["_addActor"] = [this](
        float x,
        float y,
        std::string const &ratName
    ) -> unsigned int {
        sf::IntRect rat = this->core.spritesheet.get(ratName.c_str());
        Actor actor(rat, fmin(rat.width, rat.height) / 2);
        actor.position.x = x;
        actor.position.y = y;
        Pool<Actor>::Item *item = this->actors.add(std::move(actor));
        return item->id;
    };
    this->script["_getActorPosition"] = [this](
        unsigned int id
    ) -> std::tuple<float, float> {
        Pool<Actor>::Item *actorItem = this->actors.get(id);
        if (!actorItem) return std::tuple(-1, -1);
        return std::tuple(
            actorItem->content.live.position.x,
            actorItem->content.live.position.y
        );
    };
    this->script["_setActorTransform"] = [this](
        unsigned int id,
        float x,
        float y,
        float vX,
        float vY,
        float gX,
        float gY
    ) {
        Pool<Actor>::Item *actorItem = this->actors.get(id);
        if (actorItem) {
            actorItem->content.live.position.x = x;
            actorItem->content.live.position.y = y;
            actorItem->content.live.velocity.x = vX;
            actorItem->content.live.velocity.y = vY;
            actorItem->content.live.gravity.x = gX;
            actorItem->content.live.gravity.y = gY;
        }
    };
    this->background.setPosition(sf::Vector2f(128, 0));
    this->background.setSize(sf::Vector2f(512, 600));
    this->background.setFillColor(sf::Color::Green);
    this->setScript("_main");
}

void BattleScreen::update(float delta, sf::RenderWindow &window) {
    if (this->coroutine) {
        // run the script.
        this->runScript<float>(delta);
        // update the bullets.
        for (Pool<Bullet>::Item &item: this->bullets.getItemsMutable()) {
            if (!item.alive) continue;
            item.content.live.update(delta);
            if (!this->bounds.contains(item.content.live.position)) {
                this->bullets.remove(item.id);
            }
        }
        // update the actors.
        for (Pool<Actor>::Item &item: this->actors.getItemsMutable()) {
            if (!item.alive) continue;
            item.content.live.update(delta);
            item.content.live.position = Util::clampInRect(
                item.content.live.position,
                this->bounds
            );
        }
    } else {
        this->core.popScreen(this->getLastResponse());
    }
}

void BattleScreen::draw(sf::RenderTarget &target, int top) const {
    // Draw the background.
    target.draw(this->background);
    this->core.renderer.batch.clear();
    // Draw the actors.
    for (Pool<Actor>::Item const &item: this->actors.getItems()) {
        if (!item.alive) continue;
        this->core.renderer.batch.draw(
            item.content.live.rat,
            item.content.live.position
        );
    }
    // Draw the bullets.
    for (Pool<Bullet>::Item const &item: this->bullets.getItems()) {
        if (!item.alive) continue;
        this->core.renderer.batch.draw(
            item.content.live.prototype->rat,
            item.content.live.position
        );
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
