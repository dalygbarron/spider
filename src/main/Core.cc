#include "Core.hh"
#include "Screen.hh"
#include "FileIO.hh"
#include "pugixml.hpp"
#include <SFML/Graphics.hpp>

Core::Core():
    renderer(this->spritesheet.getTexture()),
    entityRepository(this->spritesheet)
{
    // Does nothing else.
}

Item &Core::addItem(
    char const *name,
    char const *displayName,
    char const *description,
    char const *rat,
    sf::IntRect sprite
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

void Core::newGame(int id) {
    this->memory = Memory(id);
}

void Core::loadGame(int id) {
    this->memory = FileIO::loadMemory(id);
}

void Core::saveGame() {
    FileIO::saveMemory(this->memory);
}

Memory &Core::getMemory() {
    return this->memory;
}

std::unordered_map<std::string, Item> const &Core::getItems() {
    return this->items;
}

Level *Core::loadLevel(ghc::filesystem::path const &path) {
    if (ghc::filesystem::exists(path)) {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(path.c_str());
        if (!result) {
            spdlog::error("Level file '{}' cannnot be opened", path.c_str());
            return NULL;
        }
        pugi::xml_node node = doc.child("level");
        if (!node) {
            spdlog::error("File '{}' does not contain a level.", path.c_str());
            return NULL;
        }
        // Main bits.
        Level *level = new Level();
        level->file = path;
        level->script = node.attribute("script").value();
        level->setPic(node.attribute("pic").value());
        // Instances.
        for (pugi::xml_node child: node.children()) {
            char const *type = child.name();
            if (strcmp(type, "shapeInstance") == 0) {
                Instance &instance = level->addInstance();
                instance.name = child.attribute("name").value();
                for (pugi::xml_node point: child.children("point")) {
                    instance.mesh.addVertex(sf::Vector2f(
                        point.attribute("x").as_float(),
                        point.attribute("y").as_float()
                    ));
                }
            } else if (strcmp(type, "entityInstance") == 0) {
                Instance &instance = level->addInstance();
                instance.name = child.attribute("name").value();
                instance.pos.x = child.attribute("x").as_float();
                instance.pos.y = child.attribute("y").as_float();
                instance.size = child.attribute("size").as_float();
                instance.birthSwitch = FileIO::parseSwitchExpression(
                    child.attribute("birthSwitch").value()
                );
                instance.deathSwitch = FileIO::parseSwitchExpression(
                    child.attribute("deathSwitch").value()
                );
                instance.entity = this->entityRepository.get(
                    child.attribute("entity").value()
                );
            } else if (strcmp(type, "script") == 0) {
                level->script = child.child_value();
            } else {
                spdlog::warn(
                    "weird shit going on in level file '{}'. Wtf is '{}'?",
                    path.c_str(),
                    type
                );
            }
        }
        return level;
    } else {
        Level *level = new Level();
        level->file = path;
        return level;
    }
}

void Core::pushScreen(Screen *screen) {
    this->transitions.push(Transition {true, false, 0, screen});
}

void Core::popScreen(int response) {
    this->transitions.push(Transition {false, true, response, NULL});
}

void Core::replaceScreen(Screen *screen) {
    this->transitions.push(Transition {true, true, 0, screen});
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

void Core::drawScreens(sf::RenderTarget &target) {
    if (this->nScreens == 0) return;
    auto start = this->screens.begin() + this->firstVisible;
    auto end = this->screens.begin() + this->nScreens;
    auto almostEnd = this->screens.begin() + this->nScreens - 1;
    for (auto it = start; it != end; it++) {
        (*it)->draw(target, it == almostEnd);
    }
}
