#include "Screen.hh"
#include "Const.hh"
#include "Util.hh"

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
}

Screen *AdventureScreen::update(float delta, sf::RenderWindow &window) {
    window.setMouseCursorVisible(false);
    sf::Mouse::setPosition(Const::MOUSE_ORIGIN, window);
    this->shader.setUniform("angle", camera);
}

void AdventureScreen::onClick(
    sf::Mouse::Button button,
    sf::Vector2f pos
) {

}

void AdventureScreen::onDrag(sf::Vector2f delta, sf::Vector2f pos) {
    sf::Vector2f current = Util::screenToSphere(pos, this->camera);
    sf::Vector2f old = Util::screenToSphere(sf::Vector2f(Const::HALF_WIDTH, Const::HALF_HEIGHT), this->camera);
    this->camera += current - old;
}

void AdventureScreen::onKey(sf::Keyboard::Key key) {

}

void AdventureScreen::draw(
    sf::RenderTarget &target,
    sf::RenderStates states
) const {
    target.clear();
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
    target.draw(this->core.renderer.batch);
}
