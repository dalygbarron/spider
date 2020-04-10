#include "Screen.hh"
#include "Const.hh"

BattleScreen::BattleScreen(Core &core, ghc::filesystem::path const &path):
    ScriptedScreen(core, path),
    bullets(Const::MAX_BULLETS),
    actors(Const::MAX_ACTORS)
{
    // do the shade loading
    if (!this->backgroundShader.loadFromFile(
        "shader/dots.frag",
        sf::Shader::Fragment
    )) {
        spdlog::error("shader can't load");
    }
    // Battle script api.
    this->script["_input"] = this->script.create_table();
    this->script["_addBullet"] = [this](
        unsigned int parentId,
        int type,
        float x,
        float y,
        float vX,
        float vY,
        float gX,
        float gY
    ) -> unsigned int {
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
        Pool<Bullet>::Item *item = this->bullets.add(bullet);
        if (item) return item->id;
        return 0;
    };
    this->script["_shoot"] = [this](
        unsigned int parentId,
        int type,
        float vX,
        float vY
    ) -> unsigned int {
        Bullet::Prototype const *prototype = this->core.getBulletPrototype(
            type
        );
        if (!prototype) return 0;
        Pool<Actor>::Item *parent = this->actors.get(parentId);
        if (!parent) return 0;
        Bullet bullet(prototype, parentId);
        bullet.position.x = parent->content.live.position.x;
        bullet.position.y = parent->content.live.position.y;
        bullet.velocity.x = vX;
        bullet.velocity.y = vY;
        bullet.gravity.x = 0;
        bullet.gravity.y = 0;
        Pool<Bullet>::Item *item = this->bullets.add(bullet);
        if (item) return item->id;
        return 0;
    };
    this->script["_addAnimation"] = [this](
        bool loop,
        unsigned int frameTime,
        sol::table const &frames
    ) -> Rat::Animation const * {
        Rat::Animation animation;
        animation.loop = loop;
        animation.frameTime = frameTime;
        for (std::pair<sol::object, sol::object> frame: frames) {
            animation.frames.push_back(frame.second.as<unsigned int>());
        }
        this->animations.push_back(animation);
        return &this->animations.back();
    };
    this->script["_setWalkAnimation"] = [this](
        unsigned int actorId,
        const Rat::Animation *animation
    ) {
        Pool<Actor>::Item *actorItem = this->actors.get(actorId);
        if (!actorItem) return;
        actorItem->content.live.walkAnimation = animation;
        actorItem->content.live.rat.stop();
    };
    this->script["_playAnimation"] = [this](
        unsigned int actorId,
        Rat::Animation const *animation,
        int priority
    ) {
        Pool<Actor>::Item *actorItem = this->actors.get(actorId);
        if (!actorItem) return;
        actorItem->content.live.rat.play(animation, priority);
    };
    this->script["_addActor"] = [this](
        float x,
        float y,
        std::string const &ratName,
        bool dainty,
        unsigned int ratX,
        unsigned int ratY
    ) -> unsigned int {
        Rat rat(
            this->core.spritesheet.get(ratName.c_str()),
            sf::Vector2u(ratX, ratY)
        );
        rat.stop();
        sf::Vector2u dimensions = rat.getSize();
        float radius = fmin(dimensions.x, dimensions.y) / 2;
        if (dainty) radius = Const::DAINTY_RADIUS;
        Actor actor(radius, std::move(rat));
        actor.position.x = x;
        actor.position.y = y;
        actor.dainty = dainty;
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
    this->script["_getHp"] = [this](unsigned int id) -> int {
        Pool<Actor>::Item *actorItem = this->actors.get(id);
        if (actorItem) return actorItem->content.live.hp;
        return 0;
    };
    this->script["_setHp"] = [this](unsigned int id, int hp) {
        Pool<Actor>::Item *actorItem = this->actors.get(id);
        if (actorItem) {
            actorItem->content.live.hp = hp;
            actorItem->content.live.maxHp = hp;
        }
    };
    this->script["_setPlayer"] = [this](unsigned int id) {
        this->player = id;
    };
    this->script["_setTitle"] = [this](std::string const &title) {
        this->title = title;
    };
    this->script["_setSubtitle"] = [this](std::string const &subtitle) {
        this->subtitle = subtitle;
    };
    this->background.setPosition(
        sf::Vector2f(this->bounds.left, this->bounds.top)
    );
    this->background.setSize(
        sf::Vector2f(this->bounds.width, this->bounds.height)
    );
    this->background.setFillColor(sf::Color::Green);
    this->setScript("_main");
}

void BattleScreen::update(sf::RenderWindow &window) {
    this->backgroundShader.setUniform("time", this->frame * Const::FRAME_TIME);
    if (this->coroutine) {
        // Update the input records.
        this->script["_input"][0] = sf::Keyboard::isKeyPressed(
            sf::Keyboard::Left
        );
        this->script["_input"][1] = sf::Keyboard::isKeyPressed(
            sf::Keyboard::Right
        );
        this->script["_input"][2] = sf::Keyboard::isKeyPressed(
            sf::Keyboard::Up
        );
        this->script["_input"][3] = sf::Keyboard::isKeyPressed(
            sf::Keyboard::Down
        );
        this->script["_input"][4] = sf::Keyboard::isKeyPressed(
            sf::Keyboard::LShift
        );
        this->script["_input"][5] = sf::Keyboard::isKeyPressed(
            sf::Keyboard::Z
        );
        // run the script.
        this->runScript<float>(0);
        // update the bullets.
        for (Pool<Bullet>::Item &item: this->bullets.getItemsMutable()) {
            if (!item.alive) continue;
            item.content.live.update();
            if (!this->bounds.contains(item.content.live.position)) {
                this->bullets.remove(item.id);
            }
        }
        // update the actors and cololide them with bullets.
        for (Pool<Actor>::Item &item: this->actors.getItemsMutable()) {
            if (!item.alive) continue;
            item.content.live.update();
            item.content.live.actorUpdate();
            // TODO: just make it all mobs have a rat or something.
            item.content.live.rat.update();
            item.content.live.position = Util::clampInRect(
                item.content.live.position,
                this->bounds
            );
            for (Pool<Bullet>::Item &bulletItem:
                this->bullets.getItemsMutable()
            ) {
                if (!bulletItem.alive) continue;
                if (bulletItem.content.live.parent == item.id) continue;
                if (Mob::collide(item.content.live, bulletItem.content.live)) {
                    item.content.live.hp--;
                    this->bullets.remove(bulletItem.id);
                    // TODO: special effects.
                }
            }
        }
    } else {
        this->core.popScreen(this->getLastResponse());
    }
    this->frame++;
}

void BattleScreen::draw(sf::RenderTarget &target, int top) const {
    // Draw the background.
    sf::RenderStates states;
    states.shader = &(this->backgroundShader);
    target.draw(this->background, states);
    this->core.renderer.batch.clear();
    // Draw the actors.
    for (Pool<Actor>::Item const &item: this->actors.getItems()) {
        if (!item.alive) continue;
        if (!item.content.live.dainty && item.content.live.maxHp > 0) {
            this->core.renderer.arc(
                item.content.live.position,
                item.content.live.radius * 1.2,
                -Const::HALF_PI,
                fmax((float)item.content.live.hp / item.content.live.maxHp, 0)
                    * Const::DOUBLE_PI - Const::HALF_PI
            );
        }
        this->core.renderer.rat(
            item.content.live.rat,
            item.content.live.position,
            0,
            sf::Vector2f(1, 1),
            item.content.live.flip
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
    // Draw hit boxes on dainty actors.
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
        for (Pool<Actor>::Item const &item: this->actors.getItems()) {
            if (item.alive && item.content.live.dainty) {
                this->core.renderer.arc(
                    item.content.live.position,
                    item.content.live.radius + 1,
                    0,
                    Const::DOUBLE_PI
                );
            }
        }
    }
    // Draw the GUI.
    this->core.renderer.panel(sf::FloatRect(0, 0, 256, 600));
    this->core.renderer.panel(sf::FloatRect(768, 0, 256, 600));
    this->core.renderer.text(
        this->title.c_str(),
        sf::Vector2f(this->bounds.left, this->bounds.top),
        this->core.renderer.battleFont
    );
    this->core.renderer.text(
        this->subtitle.c_str(),
        sf::Vector2f(
            this->bounds.left,
            this->bounds.top + this->core.renderer.font.height / 16
        ),
        this->core.renderer.battleFont
    );
    this->core.renderer.batch.draw(
        this->core.renderer.battleRat,
        sf::FloatRect(
            this->bounds.left + this->bounds.width + this->core.renderer.panelPatch.left.width,
            this->core.renderer.panelPatch.top.height,
            256 - this->core.renderer.panelPatch.left.width * 2,
            600 - this->core.renderer.panelPatch.top.height * 2
        )
    );
    this->core.renderer.batch.draw(
        this->core.renderer.battleRat,
        sf::FloatRect(
            this->core.renderer.panelPatch.left.width,
            this->core.renderer.panelPatch.top.height,
            256 - this->core.renderer.panelPatch.left.width * 2,
            600 - this->core.renderer.panelPatch.top.height * 2
        )
    );
    Pool<Actor>::Item const *playerItem = this->actors.getConst(this->player);
    if (playerItem && playerItem->alive) {
        for (int i = 0; i < playerItem->content.live.hp; i++) {
            this->core.renderer.node(
                sf::Vector2f(
                    this->bounds.left + this->bounds.width -
                        this->core.renderer.nodeHighlightRat.width / 2 -
                        this->core.renderer.nodeHighlightRat.width * i,
                    Const::HEIGHT - this->core.renderer.nodeHighlightRat.height / 2
                ),
                true
            );
        }
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
