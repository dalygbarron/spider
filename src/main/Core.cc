#include "Core.hh"
#include "Screen.hh"
#include "pugixml.hpp"
#include <SFML/Graphics.hpp>

Core::Core(sf::Font *font):
    font(font),
    renderer(this->spritesheet.getTexture()),
    entityRepository(this->spritesheet)
{
    // Does nothing else.
}

Core::~Core() {
    delete this->font;
}

sf::Font &Core::getFont() {
    return *this->font;
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
    printf("push %d\n", this->nScreens);
    this->screens[this->nScreens] = screen;
    this->nScreens++;
    this->firstVisible = this->nScreens - 1;
    printf("pushed\n");
}

void Core::popScreen(int response) {
    delete this->screens[this->nScreens];
    this->nScreens--;
    if (this->nScreens > 0) this->screens[this->nScreens]->onReveal(response);
    this->firstVisible = this->nScreens - 1;
}

void Core::replaceScreen(Screen *screen) {
    delete this->screens[this->nScreens];
    this->screens[this->nScreens] = screen;
    this->firstVisible = this->nScreens - 1;
}

Screen *Core::getTopScreen() {
    if (this->nScreens > 0) return this->screens[this->nScreens - 1];
    return NULL;
}

void Core::drawScreens(sf::RenderTarget &target) {
    printf("drawing %d %d\n", this->firstVisible, this->nScreens);
    for (auto it = this->screens.begin() + this->firstVisible;
        it != this->screens.end(); it++
    ) {
        (*it)->draw(target, it == this->screens.end());
    }
}
