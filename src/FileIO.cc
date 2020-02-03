#include "FileIO.hh"
#include <iostream>

void FileIO::parsePatch(Core &core, pugi::xml_node const &node) {
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

Level *FileIO::parseLevel(pugi::xml_node const &node) {
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

Entity *FileIO::parseEntity(pugi::xml_node const &node) {

}

Level *FileIO::levelFromFile(ghc::filesystem::path const &path) {
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
            return NULL;
        }
        Level *level = FileIO::parseLevel(node);
        level->file = path;
        return level;
    } else {
        printf("%s not exist\n", path.c_str());
        return NULL;
    }
}

Entity *FileIO::entityFromFile(ghc::filesystem::path const &path) {
    if (ghc::filesystem::exists(path)) {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(path.c_str());
        if (!result) {
            spdlog::error("File '{}' cannot be opened", path.c_str());
            return NULL;
        }
        pugi::xml_node node = doc.child("entity");
        if (!node) {
            spdlog::error("file '{}' contains no entity", path.c_str());
            return NULL;
        }
        Entity *entity = new Entity();
        entity->file = path;
        entity->name = node.attribute("name").value();
        entity->item = node.attribute("item").value();
        entity->sprite = node.attribute("rat").value();
        entity->origin.x = node.attribute("origin-x").as_float();
        entity->origin.y = node.attribute("origin-y").as_float();
        return entity;
    }
    Entity *entity = new Entity();
    entity->file = path;
    return entity;
}

void FileIO::saveEntity(Entity const &entity) {
    spdlog::info("Saving entity to {}", entity.file.c_str());
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("entity");
    node.append_attribute("name") = entity.name.c_str();
    node.append_attribute("rat") = entity.sprite.c_str();
    node.append_attribute("origin-x") = entity.origin.x;
    node.append_attribute("origin-y") = entity.origin.y;
    int success = doc.save_file(entity.file.c_str());
    if (!success) {
        spdlog::error(
            "Could not save entity to file {}. Does folder exist?",
            entity.file.c_str()
        );
    }
}

void FileIO::initRatPackFromFile(
    RatPack &pack,
    ghc::filesystem::path const &path
) {
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

Core *FileIO::loadCoreFromFile(ghc::filesystem::path const &path) {
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
    if (ratPack) FileIO::initRatPackFromFile(core->spritesheet, ratPack.value());
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
            FileIO::parsePatch(*core, child);
        }
        // TODO: other types of child node.
    }
    return core;
}
