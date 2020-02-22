#include "Core.hh"
#include "pugixml.hpp"

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

Level *Core::loadLevel(ghc::filesystem::path const &path) const {
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
            Instance *instance = new Instance();
            instance->name = child.attribute("name").value();
            char const *type = child.name();
            if (strcmp(type, "shapeInstance") == 0) {

            } else if (strcmp(type, "entityInstance") == 0) {

            } else {
                spdlog::warn(
                    "weird shit going on in level file '{}'. Wtf is '{}'?",
                    path.c_str(),
                    type
                );
            }
            level->addInstance(instance);
        }
        return level;
    } else {
        Level *level = new Level();
        level->file = path;
        return level;
    }
}
