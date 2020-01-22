#include "Util.hh"
#include "filesystem.hh"
#include "pugixml.hpp"
#include <SFML/Graphics.hpp>
#include <stdio.h>

Level *Util::parseLevel(pugi::xml_node &node) {
    // Main bits.
    ghc::filesystem::path picture = node.attribute("picture").value();
    Level *level = new Level(pictureFromFile(picture));
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

Entity *Util::parseEntity(pugi::xml_node &node) {

}

Picture *Util::pictureFromFile(ghc::filesystem::path &path) {
    sf::Texture *texture = new sf::Texture();
    if (!texture->loadFromFile(path.c_str())) {
        delete texture;
        return NULL;
    }
    texture->setSmooth(true);
    return new Picture(texture, path);
}

Level *Util::levelFromFile(ghc::filesystem::path &path) {
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

Entity *Util::entityFromFile(ghc::filesystem::path &path) {
    // TODO: this.
    return NULL;
}

void Util::addRatsFromFile(RatPack &pack, ghc::filesystem::path &path) {
    // TODO: load the xml file and parse it and all that stuff.
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
