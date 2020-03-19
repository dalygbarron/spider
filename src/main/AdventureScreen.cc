#include "Screen.hh"
#include "Const.hh"
#include "Knob.hh"
#include "Util.hh"
#include "FileIO.hh"

AdventureScreen::AdventureScreen(Core &core, Level *level): Screen(core) {
    this->level = level;
    this->back.setSize(sf::Vector2f(Const::WIDTH, Const::HEIGHT));
    this->shader.setUniform("angle", this->camera);
    this->shader.setUniform("picture", sf::Shader::CurrentTexture);
    if (!this->shader.loadFromMemory(
        Const::SKY_SHADER, sf::Shader::Fragment
    )) {
        spdlog::error("Couldn't start the sky shader");
    }
    this->back.setTexture(&this->level->getPic(), true);
    // TODO: handle birth and death switches again.
    this->script.open_libraries(
        sol::lib::base,
        sol::lib::coroutine,
        sol::lib::package,
        sol::lib::math,
        sol::lib::table,
        sol::lib::string
    );
    sol::usertype<Item> itemType = this->script.new_usertype<Item>(
        "Item",
        sol::constructors<Item()>()
    );
    itemType["name"] = &Item::name;
    itemType["displayName"] = &Item::displayName;
    itemType["description"] = &Item::description;
    itemType["rat"] = &Item::rat;
    this->script["_itemInfo"] = this->script.create_table();
    int i = 1;
    for (std::pair<std::string, Item> const &item: this->core.getItems()) {
        this->script["_itemInfo"][i] = item.second;
        i++;
    }
    this->script["_inventory"] = [this]() {
        sol::table inventoryTable = this->script.create_table();
        for (std::pair<std::string, int> const &item:
            this->core.getMemory().getItems()
        ) {
            inventoryTable[item.first.c_str()] = item.second;
        }
        return inventoryTable;
    };
    this->script["_addItem"] = [this](std::string name) {
        Memory &memory = this->core.getMemory();
        char const *nameString = name.c_str();
        memory.setItemCount(nameString, memory.getItemCount(nameString) + 1);
        return this->core.getMemory().getItemCount(name.c_str());
    };
    this->script["_useItem"] = [this](std::string name) {
        std::unordered_map<std::string, Item> const &items =
            this->core.getItems();
        if (items.count(name)) {
            this->selected = &(this->core.getItems().at(name.c_str()));
        } else {
            spdlog::error("API: invalid item '{}'", name.c_str());
        }
    };
    this->script["_saveGame"] = [this]() {
        this->core.saveGame();
    };
    this->script["_loadGame"] = [this](int id) {
        this->core.loadGame(id);
        spdlog::info("idiota: {}", this->core.getMemory().level.c_str());
        this->core.replaceScreen(new AdventureScreen(
            this->core,
            this->core.loadLevel(this->core.getMemory().level.c_str())
        ));
    };
    this->script["_newGame"] = [this](int id) {
        this->core.newGame(id);
    };
    this->script["_systemInfo"] = []() {
        return std::make_tuple(8, 8);
    };
    this->script["_getCamera"] = [this]() {
        return std::make_tuple(this->camera.x, this->camera.y);
    };
    this->script["_setCamera"] = [this](float x, float y) {
        this->camera.x = x;
        this->camera.y = y;
    };
    this->script["_xmlKnob"] = [this](std::string const &xml) {
        spdlog::debug("Adding knob xml: '{}'", xml.c_str());
        Knob *knob = FileIO::readXml<Knob, FileIO::KnobInfo>(
            xml.c_str(),
            FileIO::parseKnob,
            FileIO::KnobInfo {
                this->core.renderer.getMeasurements(),
                this->core.spritesheet
            }
        );
        if (knob) {
            this->core.pushScreen(new KnobScreen(this->core, knob));
        } else {
            spdlog::error("API: Invalid argument to _xmlKnob");
        }
    };
    this->script["_exit"] = [this](int response) {
        this->core.popScreen(response);
    };
    this->script.script(this->level->script);
    this->setScript("_start");
}

AdventureScreen::~AdventureScreen() {
    delete this->level;
}

void AdventureScreen::update(float delta, sf::RenderWindow &window) {
    if (this->runScript<float>(delta)) return;
    Util::centreMouse(window);
    this->shader.setUniform("angle", camera);
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
            hit = instance.entity->mesh.in(Util::rotateAround(
                sf::Vector2f(Const::HALF_WIDTH, Const::HALF_HEIGHT),
                sf::Vector2f(screenPos.x, screenPos.y),
                -angle,
                1 / instance.size
            ) - sf::Vector2f(screenPos.x, screenPos.y) - instance.entity->offset);
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
    target.clear();
    sf::RenderStates states;
    states.shader = &(this->shader);
    target.draw(back, states);
    this->core.renderer.batch.clear();
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
    if (top) {
        if (this->selected) {
            this->core.renderer.batch.draw(
                this->selected->sprite,
                sf::Vector2f(Const::HALF_WIDTH + 32, Const::HALF_HEIGHT + 32)
            );
        }
        this->core.renderer.cursor(
            sf::Vector2f(Const::HALF_WIDTH, Const::HALF_HEIGHT),
            Renderer::CursorType::pointer
        );

    }
    target.draw(this->core.renderer.batch);
}

void AdventureScreen::setScript(char const *name) {
    this->coroutine = this->script[name];
}
