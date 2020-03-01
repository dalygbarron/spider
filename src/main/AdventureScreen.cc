#include "Screen.hh"
#include "Const.hh"
#include "Knob.hh"
#include "Util.hh"
#include "FileIO.hh"

AdventureScreen::AdventureScreen(Core &core, Level const &level):
    Screen(core),
    level(level)
{
    this->back.setSize(sf::Vector2f(Const::WIDTH, Const::HEIGHT));
    this->shader.setUniform("angle", this->camera);
    this->shader.setUniform("picture", sf::Shader::CurrentTexture);
    if (!this->shader.loadFromMemory(
        Const::SKY_SHADER, sf::Shader::Fragment
    )) {
        spdlog::error("Couldn't start the sky shader");
    }
    this->back.setTexture(&this->level.getPic(), true);
    this->script.open_libraries(
        sol::lib::base,
        sol::lib::coroutine,
        sol::lib::package,
        sol::lib::math,
        sol::lib::table,
        sol::lib::string
    );
    this->script["_error"] = [](std::string message) {
        spdlog::error("Script: {}", message.c_str());
    };
    this->script["_getCamera"] = [this]() {
        return std::make_tuple(this->camera.x, this->camera.y);
    };
    this->script["_setCamera"] = [this](float x, float y) {
        this->camera.x = x;
        this->camera.y = y;
    };
    this->script["_xmlKnob"] = [this](std::string const &xml) {
        spdlog::info(xml.c_str());
        Knob *knob = FileIO::readXml(
            xml.c_str(),
            FileIO::parseKnob
        );
        if (knob) {
            this->core.pushScreen(new KnobScreen(this->core, knob));
        } else {
            spdlog::error("API: Invalid argument to _xmlKnob");
        }
    };
    this->script.script(this->level.script);
    this->coroutine = this->script["_start"];
    this->coroutine.error_handler = this->script["_error"];
    if (this->coroutine) this->coroutine();
}

void AdventureScreen::update(float delta, sf::RenderWindow &window) {
    if (this->coroutine) {
        this->coroutine(delta);
    }
    sf::Mouse::setPosition(Const::MOUSE_ORIGIN, window);
    this->shader.setUniform("angle", camera);
}

void AdventureScreen::onClick(
    sf::Mouse::Button button,
    sf::Vector2f pos
) {
    if (this->coroutine) return;
}

void AdventureScreen::onReveal(int response) {
    if (this->coroutine) this->coroutine(response);
}

void AdventureScreen::onDrag(sf::Vector2f delta, sf::Vector2f pos) {
    if (this->coroutine) return;
    sf::Vector2f current = Util::screenToSphere(pos, this->camera);
    sf::Vector2f old = Util::screenToSphere(
        sf::Vector2f(Const::HALF_WIDTH, Const::HALF_HEIGHT),
        this->camera
    );
    this->camera += current - old;
}

void AdventureScreen::onKey(sf::Keyboard::Key key) {
    if (this->coroutine) return;
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
    for (Instance const &instance: this->level.instances) {
        if (instance.entity) {
            sf::Vector3f pos = Util::sphereToScreen(
                instance.pos,
                this->camera
            );
            if (pos.z < 0) continue;
            float angle;
            if (this->camera.y > 0) {
                angle = atan2(floor.x - pos.x, floor.y - pos.y) *
                    sin(this->camera.y);
            } else {
                angle = atan2(pos.x - floor.x, pos.y - floor.y) *
                    sin(this->camera.y);
            }
            this->core.renderer.batch.draw(
                instance.entity->sprite,
                sf::Vector2f(pos.x, pos.y),
                instance.entity->offset,
                angle * ((this->camera.y > 0) ? -1 : 1),
                sf::Vector2f(instance.size, instance.size)
            );
        }
    }
    if (top) {
        this->core.renderer.cursor(
            sf::Vector2f(Const::HALF_WIDTH, Const::HALF_HEIGHT),
            Renderer::CursorType::pointer
        );
    }
    target.draw(this->core.renderer.batch);
}