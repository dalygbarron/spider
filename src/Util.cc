#include "Util.hh"
#include "Const.hh"
#include "filesystem.hh"
#include "pugixml.hpp"
#include <SFML/Graphics.hpp>
#include <stdio.h>

void Util::parsePatch(Core &core, pugi::xml_node const &node) {
    Patch patch;
    patch.fill(
        core.spritesheet.get(node.attribute("rat").value()),
        node.attribute("left").as_int(),
        node.attribute("top").as_int(),
        node.attribute("right").as_int(),
        node.attribute("bottom").as_int()
    );
    char const *role = node.attribute("role").value();
    if (strcmp(role, "box") == 0) {
        core.renderer.setBoxPatch(patch);
    } else if (strcmp(role, "boxHighlight") == 0) {
        core.renderer.setBoxHighlightPatch(patch);
    } else {
        spdlog::error("No such role in gui for patch as '{}'", role);
    }
}

Level *Util::parseLevel(pugi::xml_node const &node) {
    // Main bits.
    Level *level = new Level();
    level->script = node.attribute("script").value();
    // Defines.
    pugi::xml_node defines = node.child("defines");
    for (pugi::xml_node define = defines.child("define"); define;
        define = define.next_sibling("define")
    ) {
    }
    // Entities.
    // TODO: things.
    return level;
}

Entity *Util::parseEntity(pugi::xml_node const &node) {

}

Level *Util::levelFromFile(ghc::filesystem::path const &path) {
    if (ghc::filesystem::exists(path)) {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(path.c_str());
        if (!result) {
            fprintf(
                stderr,
                "File '%s' simply can NOT be opened.\n",
                path.c_str()
            );
            return NULL;
        }
        pugi::xml_node node = doc.child("level");
        if (!node) {
            fprintf(
                stderr,
                "File '%s' sadly does not contain a level.\n",
                path.c_str()
            );
        }
        Level *level = Util::parseLevel(node);
        level->file = path;
        return level;
    } else {
        printf("%s not exist\n", path.c_str());
        return NULL;
    }
}

Entity *Util::entityFromFile(ghc::filesystem::path const &path) {
    // TODO: this.
    return NULL;
}

void Util::initRatPackFromFile(
    RatPack &pack,
    ghc::filesystem::path const &path
) {
    // TODO: I am thinking just now that the path to the image in the rat pack
    //       is going to be relative to the rat pack. Therefore, we are
    //       possibly going to want to change the current working directory to
    //       the directory of the pack or something like that. I will figure
    //       out a cohesive way of dealing with this issue later.
    pack.clear();
    if (!ghc::filesystem::exists(path)) {
        spdlog::error("No Rat Pack at '{}'", path.c_str());
        return;
    }
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path.c_str());
    if (!result) {
        spdlog::error(
            "'{}' does not work as an xml file: {}",
            path.c_str(),
            result.description()
        );
        return;
    }
    pugi::xml_node node = doc.child("pack");
    if (!node) {
        spdlog::error("File '{}' has no ratpack", path.c_str());
        return;
    }
    pugi::xml_attribute image = node.attribute("image");
    if (image) {
        sf::Texture &texture = pack.getTextureMutable();
        texture.loadFromFile(image.value());
    }
    for (pugi::xml_node rat = node.child("rat"); rat;
        rat = rat.next_sibling("rat")
    ) {
        pack.add(rat.attribute("name").value(), sf::IntRect(
            rat.attribute("x").as_int(),
            rat.attribute("y").as_int(),
            rat.attribute("w").as_int(),
            rat.attribute("h").as_int()
        ));
    }
}

Core *Util::loadCoreFromFile(ghc::filesystem::path const &path) {
    sf::IntRect pointRat(0, 0, 0, 0);
    sf::IntRect nodeRat(0, 0, 0, 0);
    sf::IntRect nodeHighlightRat(0, 0, 0, 0);
    sf::IntRect boxRat(0, 0, 0, 0);
    sf::IntRect boxHighlightRat(0, 0, 0, 0);
    if (!ghc::filesystem::exists(path)) {
        spdlog::info("Creating new game core at '{}'", path.c_str());
        // TODO: is this still ok?
        Core *core = new Core(NULL);
        core->filename = path;
        return core;
    }
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file(path.c_str());
    if (!result) {
        spdlog::error(
            "'{}' does not work as an xml file: {}",
            path.c_str(),
            result.description()
        );
        return NULL;
    }
    pugi::xml_node node = doc.child("game");
    if (!node) {
        spdlog::error("File '{}' has no game in it", path.c_str());
        return NULL;
    }
    // Load names of sprites.
    sf::Font *font = new sf::Font();
    font->loadFromFile(node.attribute("font").value());
    Core *core = new Core(font);
    core->filename = path;
    core->name = node.attribute("name").value();
    // Check some optional things.
    pugi::xml_attribute ratPack = node.attribute("rat");
    if (ratPack) Util::initRatPackFromFile(core->spritesheet, ratPack.value());
    // now set the gui bits
    core->renderer.setPointRat(core->spritesheet.get(
        node.attribute("point").value())
    );
    core->renderer.setNodeRat(core->spritesheet.get(
        node.attribute("node").value()
    ));
    core->renderer.setNodeHighlightRat(core->spritesheet.get(
        node.attribute("nodeHighlight").value()
    ));
    // Now look for child nodes to deal with.
    for (pugi::xml_node child: node.children()) {
        char const *type = child.name();
        if (strcmp(type, "patch") == 0) {
            Util::parsePatch(*core, child);
        }
        // TODO: other types of child node.
    }
    return core;
}

sf::View Util::getLetterboxView(sf::View view, sf::Vector2i dimensions) {
    float windowRatio = dimensions.x / (float)dimensions.y;
    float viewRatio = view.getSize().x / (float)view.getSize().y;
    float sizeX = 1;
    float sizeY = 1;
    float posX = 0;
    float posY = 0;
    int horizontalSpace = true;
    if (windowRatio < viewRatio) horizontalSpace = false;
    if (horizontalSpace) {
        sizeX = viewRatio / windowRatio;
        posX = (1 - sizeX) / 2.f;
    } else {
        sizeY = windowRatio / viewRatio;
        posY = (1 - sizeY) / 2.f;
    }
    view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
    return view;
}

float Util::degrees(float radians) {
    return fmod(radians, Const::DOUBLE_PI) * Const::RADIAN_CONVERT;
}
