#include "Core.hh"
#include "Screen.hh"
#include "Const.hh"
#include "Shaders.hh"
#include "pugixml.hpp"
#include <SFML/Graphics.hpp>

Core::Core(int allowMusic, glm::ivec2 size, float fov):
    renderer(this->spritesheet.getTexture(), size),
    entityRepository(this->spritesheet),
    soundPlayer(16, allowMusic),
    size(size),
    fov(fov * (float)size.x / (float)size.y, fov)
{
    float aspect = (float)size.x / (float)size.y;
    this->projection = glm::perspective(
        fov,
        aspect,
        Const::MIN_CLIP,
        Const::MAX_CLIP
    );
    this->transition.setSize(sf::Vector2f(size.x, size.y));
    this->transitionShader.setUniform("power", this->transitionStrength);
    this->transitionShader.setUniform("picture", sf::Shader::CurrentTexture);
    if (!this->transitionShader.loadFromMemory(
        Shaders::TRANSITION_SHADER,
        sf::Shader::Fragment
    )) {
        spdlog::error("Couldn't start the transition shader");
    }
}

Item &Core::addItem(
    char const *name,
    char const *displayName,
    char const *description,
    char const *rat,
    Rectangle sprite
) {
    Item item;
    item.name = name;
    item.displayName = displayName;
    item.description = description;
    item.rat = rat;
    item.sprite = sprite;
    this->items[name] = std::move(item);
    return this->items[name];
}

std::unordered_map<std::string, Item> const &Core::getItems() {
    return this->items;
}

Entity &Core::createEntity(char const *name) {
    this->entities.emplace(name);
    return this->entities[name];
}

std::unordered_map<std::string, Entity> const &core::getEntities() const {
    return this->entities;
}

Level &Core::createLevel(char const *name) {
    this->levels.emplace(name);
    return this->levels[name];
}

std::unordered_map<std::string, Level> const &core::getLevels() const {
    return this->level;
}

void Core::addBulletPrototype(int id, Rectangle rat) {
    Bullet::Prototype prototype;
    prototype.rat = rat;
    prototype.radius = (float)rat.size.x / 2;
    this->bulletPrototypes[id] = std::move(prototype);
}

Bullet::Prototype const *Core::getBulletPrototype(int id) {
    if (this->bulletPrototypes.count(id)) {
        return &this->bulletPrototypes.at(id);
    }
    return NULL;
}

void Core::newGame(unsigned int id) {
    this->memory = Memory(id);
}

void Core::loadGame(unsigned int id) {
    this->memory = FileIO::loadMemory(id);
}

void Core::saveGame() {
    FileIO::saveMemory(this->memory);
}

Memory &Core::getMemory() {
    return this->memory;
}

void Core::setTransitionStrength(float strength) {
    this->transitionStrength = strength;
    this->transitionShader.setUniform("power", this->transitionStrength);
}

void Core::setTransitionColour(sf::Color colour) {
    // TODO: stuff.
}

void Core::setTransitionTexture(ghc::filesystem::path const &path) {
    if (!this->transitionTexture.loadFromFile(path.c_str())) {
        spdlog::error(
            "Could not load transition texture from '{}'",
            path.c_str()
        );
    }
    this->transition.setTexture(&this->transitionTexture, true);
}

glm::mat4 const &Core::getProjection() const {
    return this->projection;
}

void Core::pushScreen(Screen *screen) {
    this->transitions.push(Transition {true, false, 0, screen});
}

void Core::popScreen(int response) {
    this->transitions.push(Transition {false, true, response, NULL});
}

void Core::replaceScreen(Screen *screen) {
    this->transitions.push(Transition {true, true, 0, screen});
    spdlog::info("yeah");
}

Screen *Core::getTopScreen() {
    if (this->nScreens > 0) return this->screens[this->nScreens - 1];
    return NULL;
}

void Core::performTransitions() {
    int action = !this->transitions.empty();
    while (!this->transitions.empty()) {
        Transition transition = this->transitions.front();
        this->transitions.pop();
        if (transition.pop && this->nScreens > 0) {
            delete this->screens[this->nScreens - 1];
            this->nScreens--;
            if (this->nScreens > 0 && !transition.push) {
                this->screens[this->nScreens - 1]->onReveal(
                    transition.response
                );
            }
        }
        if (transition.push) {
            if (this->nScreens >= this->screens.size()) {
                this->screens.resize(this->nScreens + 4);
            }
            this->screens[this->nScreens] = transition.screen;
            this->screens[this->nScreens]->onStart();
            this->nScreens++;
        }
    }
    if (action) {
        for (
            this->firstVisible = this->nScreens - 1;
            this->firstVisible >= 0 &&
                this->screens[this->firstVisible]->isTransparent();
            this->firstVisible--
        );
    }
}

void Core::drawScreens(sf::RenderTarget &target) const {
    // Draw the screens.
    if (this->nScreens > 0) {
        auto start = this->screens.begin() + this->firstVisible;
        auto end = this->screens.begin() + this->nScreens;
        auto almostEnd = this->screens.begin() + this->nScreens - 1;
        for (auto it = start; it != end; it++) {
            (*it)->draw(target, it == almostEnd);
        }
    }
    // draw the transition thingy.
    if (this->transitionStrength > 0) {
        sf::RenderStates states;
        states.shader = &(this->transitionShader);
        target.draw(this->transition, states);
    }
}
