#include "Screen.hh"
#include "Const.hh"
#include "Knob.hh"
#include "Util.hh"
#include "FileIO.hh"

AdventureScreen::AdventureScreen(Core &core, Level *level):
    ScriptedScreen(core, level->script),
    background(sf::IntRect(0, 0, Const::WIDTH, Const::HEIGHT))
{
    this->level = level;
    this->background.initFromString(Const::SKY_SHADER);
    this->background.setTexture(&level->getPic());
    // Add some new things to the script.
    this->script["_useItem"] = [this](std::string name) {
        std::unordered_map<std::string, Item> const &items =
            this->core.getItems();
        if (items.count(name)) {
            this->selected = &(this->core.getItems().at(name.c_str()));
        } else {
            spdlog::error("API: invalid item '{}'", name.c_str());
        }
    };
    this->script["_go"] = [this](std::string const &level) {
        this->core.replaceScreen(new AdventureScreen(
            this->core,
            this->core.loadLevel(level)
        ));
    };
    this->script["_battle"] = [this](std::string const &file) {
        this->core.pushScreen(new BattleScreen(this->core, file));
    };
    this->script["_getCamera"] = [this]() {
        return std::make_tuple(this->camera.x, this->camera.y);
    };
    this->script["_setCamera"] = [this](float x, float y) {
        this->camera.x = x;
        this->camera.y = y;
    };
    this->script["_world"] = [this](std::string const &xml) {
        spdlog::debug("Adding world xml: {}", xml.c_str());
        this->world = FileIO::readXml<World, EntityRepository &>(
            xml.c_str(),
            FileIO::parseWorld,
            this->core.entityRepository
        );
    };
    this->setScript("_start");
    // Check switches.
    this->refresh();
}

AdventureScreen::~AdventureScreen() {
    delete this->level;
}

void AdventureScreen::update(sf::RenderWindow &window) {
    this->background.update();
    if (this->runScript<float>(0)) return;
    Util::centreMouse(window);
    this->background.setUniform("angle", this->camera);
}

void AdventureScreen::onClick(
    sf::Mouse::Button button,
    sf::Vector2f pos
) {
    if (this->coroutine) return;
    sf::Vector3f floor = Util::sphereToScreen(
        sf::Vector2f(0, Const::HALF_PI * ((this->camera.y > 0) ? 1 : -1)),
        this->camera
    );
    sf::Vector2f floorScreen(floor.x, floor.y);
    for (Instance &instance: this->level->instances) {
        if (!instance.alive) continue;
        int hit = false;
        if (instance.entity) {
            sf::Vector3f screenPos = Util::sphereToScreen(
                instance.pos,
                this->camera
            );
            if (screenPos.z < 0) continue;
            float angle = Util::upAngle(
                this->camera,
                floorScreen,
                sf::Vector2f(screenPos.x, screenPos.y)
            );
            hit = instance.entity->mesh.in(
                Util::rotateAround(
                    sf::Vector2f(Const::HALF_WIDTH, Const::HALF_HEIGHT),
                    sf::Vector2f(screenPos.x, screenPos.y),
                    -angle,
                    1 / instance.size
                ) - sf::Vector2f(screenPos.x, screenPos.y) -
                instance.entity->offset
            );
        } else {
            hit = instance.mesh.inSphere(this->camera);
        }
        if (hit) {
            if (instance.entity && !instance.entity->item.empty()) {
                char const *itemName = instance.entity->item.c_str();
                this->core.getMemory().setItemCount(
                    itemName,
                    this->core.getMemory().getItemCount(itemName) + 1
                );
                instance.alive = false;
                this->core.getMemory().setLocalSwitch(
                    instance.entity->itemKey.c_str(),
                    true
                );
                this->setScript("_itemMessage");
                this->runScript<char const *>(itemName);
            } else {
                this->setScript(instance.name.c_str());
            }
            return;
        }
    }
}

void AdventureScreen::onStart() {
    this->core.getMemory().level = this->level->file;
}

void AdventureScreen::onReveal(int response) {
    this->core.getMemory().level = this->level->file;
    this->selected = NULL;
    this->runScript<int>(response);
}

void AdventureScreen::onDrag(sf::Vector2f prev, sf::Vector2f pos) {
    if (this->coroutine) return;
    if (prev == pos) return;
    //spdlog::info("{} {}", pos.x, pos.y);
    sf::Vector2f current = Util::screenToSphere(pos, this->camera);
    this->camera = current;
}

void AdventureScreen::onKey(sf::Keyboard::Key key) {
    if (this->coroutine) return;
    if (key == sf::Keyboard::Key::E) {
        this->setScript("_gameMenu");
    } else if (key == sf::Keyboard::Key::Escape) {
        this->setScript("_programMenu");
        if (!this->coroutine) {
            spdlog::warn("No _programMenu defined, quitting screen");
            this->core.popScreen(-1);
        }
    }
}

void AdventureScreen::draw(sf::RenderTarget &target, int top) const {
    this->core.renderer.batch.clear();
    // draw the behind world if applicable.
    if (this->world) {
        this->world->draw(target, this->core.renderer, this->camera);
    }
    // draw the level.
    this->background.draw(target);
    // drawing entity instances.
    sf::Vector3f floor = Util::sphereToScreen(
        sf::Vector2f(0, Const::HALF_PI * ((this->camera.y > 0) ? 1 : -1)),
        this->camera
    );
    sf::Vector2f floorScreen(floor.x, floor.y);
    for (Instance const &instance: this->level->instances) {
        if (!instance.alive) continue;
        if (instance.entity) {
            sf::Vector3f pos = Util::sphereToScreen(
                instance.pos,
                this->camera
            );
            if (pos.z < 0) continue;
            float angle = Util::upAngle(
                this->camera,
                floorScreen,
                sf::Vector2f(pos.x, pos.y)
            );
            this->core.renderer.batch.draw(
                instance.entity->sprite,
                sf::Vector2f(pos.x, pos.y),
                instance.entity->offset,
                angle,
                sf::Vector2f(instance.size, instance.size)
            );
        }
    }
    // Cursor and stuff.
    if (top) {
        if (this->selected) {
            this->core.renderer.batch.draw(
                this->selected->sprite,
                sf::Vector2f(Const::HALF_WIDTH + 32, Const::HALF_HEIGHT + 32)
            );
        }
        this->core.renderer.cursor(
            sf::Vector2f(Const::HALF_WIDTH, Const::HALF_HEIGHT),
            Renderer::CursorType::Pointer
        );

    }
    target.draw(this->core.renderer.batch);
}


void AdventureScreen::refresh() {
    Memory const &memory = this->core.getMemory();
    for (Instance &instance: this->level->instances) {
        if (instance.entity && !instance.entity->item.empty()) {
            instance.alive = !memory.getLocalSwitch(
                instance.entity->itemKey.c_str()
            );
        } else if (instance.lifeSwitch) {
            instance.alive = instance.lifeSwitch->evaluate(memory);
        }
    }
}
