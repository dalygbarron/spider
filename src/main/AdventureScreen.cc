#include "Screen.hh"
#include "Const.hh"
#include "Knob.hh"
#include "Util.hh"
#include "FileIO.hh"
#include "Shaders.hh"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"

AdventureScreen::AdventureScreen(Core &core, Level *level):
    ScriptedScreen(core, level->script),
    background(Rectangle(glm::vec2(), core.size)),
    limiter(30)
{
    this->level = level;
    this->angle = glm::vec2(Const::PI, Const::PI);
    this->background.initFromString(Shaders::SKY_SHADER);
    this->background.setTexture(&level->getPic());
    this->background.setUniform("fov", this->core.fov);
    // Add some new things to the script.
    this->script["_input"] = this->script.create_table();
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
        return this->angle;
    };
    this->script["_setCamera"] = [this](float x, float y) {
        this->angle.x = x;
        this->angle.y = y;
    };
    this->script["_world"] = [this](std::string const &xml) {
        spdlog::info("Adding world xml: {}", xml.c_str());
        this->world = FileIO::readXml<World, Core &>(
            xml.c_str(),
            FileIO::parseWorld,
            this->core
        );
    };
    this->script["_getWorldPosition"] = [this]() -> glm::vec3 {
        if (this->world) return this->world->position;
        return glm::vec3(0);
    };
    this->script["_setWorldPosition"] = [this](glm::vec3 const &position) {
        if (this->world) this->world->position = position;
    };
    this->script["_getWorldVelocity"] = [this]() -> glm::vec3 {
        if (this->world) return this->world->velocity;
        return glm::vec3(0);
    };
    this->script["_setWorldVelocity"] = [this](glm::vec3 const &velocity) {
        if (this->world) this->world->velocity = velocity;
    };
    this->script["_getWorldGravity"] = [this]() -> glm::vec3 {
        if (this->world) return this->world->gravity;
        return glm::vec3(0);
    };
    this->script["_setWorldGravity"] = [this](glm::vec3 const &gravity) {
        if (this->world) this->world->gravity = gravity;
    };
    this->script["_getWorldRotation"] = [this]() -> glm::vec2 {
        if (this->world) return this->world->rotation;
        return glm::vec2(0);
    };
    this->script["_setWorldRotation"] = [this](glm::vec2 const &rotation) {
        if (this->world) this->world->rotation = rotation;
    };
    this->ticker = this->script["_update"];
    this->setScript("_start");
    // Check switches.
    this->refresh();
}

AdventureScreen::~AdventureScreen() {
    delete this->level;
}

void AdventureScreen::update(float delta, sf::RenderWindow &window) {
    int frames = this->limiter.update(delta);
    for (int i = 0; i < frames; i++) {
        glm::mat4 camera = Util::camera(this->angle);
        glm::mat4 const &projection = this->core.getProjection();
        this->sendInput();
        if (this->ticker) this->ticker();
        if (this->world) this->world->update(camera);
        this->background.setUniform("camera", glm::inverse(camera));
        this->background.update();
        if (this->runScript<float>(0)) return;
        glm::ivec2 mid = this->core.size / 2;
        sf::Mouse::setPosition(sf::Vector2i(mid.x, mid.y), window);
    }
}

void AdventureScreen::onClick(
    sf::Mouse::Button button,
    glm::ivec2 pos
) {
    if (this->coroutine) return;
    glm::mat4 camera = Util::camera(this->angle);
    for (Instance &instance: this->level->instances) {
        if (!instance.alive) continue;
        int hit = false;
        if (instance.entity) {
            glm::vec3 cartesian = Util::sphericalToCartesian(instance.pos);
            glm::vec4 screen = glm::vec4(cartesian, 1) * camera;
            if (screen.z > 0) continue;
            hit = instance.entity->mesh.in(
                Util::rotateAround(
                    glm::vec2(),
                    glm::vec2(screen.x, screen.y),
                    0,
                    1 / instance.size
                ) - instance.entity->offset
            );
        } else {
            hit = instance.mesh.inSphere(this->angle);
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

void AdventureScreen::onDrag(glm::ivec2 prev, glm::ivec2 pos) {
    if (this->coroutine || prev == pos) {
        return;
    }
    glm::vec2 mid = (glm::vec2)this->core.size * 0.5f;
    this->angle += ((glm::vec2)pos - mid) / mid * this->core.fov *
        glm::vec2(-1, 1);
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
    glm::mat4 camera = Util::camera(this->angle) * this->core.getProjection();
    // draw the level.
    this->background.draw(target);
    // drawing entity instances.
    glm::vec2 size = this->core.size;
    for (Instance const &instance: this->level->instances) {
        if (!instance.alive || !instance.entity) continue;
        glm::vec3 cartesian = Util::sphericalToCartesian(instance.pos);
        float scale = instance.entity->scale * instance.size;
        this->core.renderer.batch.draw(
            instance.entity->sprite,
            cartesian,
            instance.entity->offset,
            0,
            glm::vec2(scale, scale),
            camera
        );
    }
    // Cursor and stuff.
    if (top) {
        if (this->selected) {
            this->core.renderer.batch.draw(
                this->selected->sprite,
                this->core.size + glm::ivec2(32, 32)
            );
        }
        this->core.renderer.cursor(
            this->core.size + this->core.renderer.cursorRats[
                static_cast<int>(Renderer::CursorType::Pointer)
            ].size / 2,
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
