#include "FileIO.hh"
#include "Const.hh"
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
    } else if (strcmp(role, "panel") == 0) {
        core.renderer.setPanelPatch(patch);
    } else if (strcmp(role, "button") == 0) {
        core.renderer.setButtonPatch(patch);
    } else if (strcmp(role, "buttonDepressed") == 0) {
        core.renderer.setButtonDepressedPatch(patch);
    } else {
        spdlog::error("No such role in gui for patch as '{}'", role);
    }
}

void FileIO::saveEntity(Entity const &entity) {
    spdlog::info("Saving entity to {}", entity.file.c_str());
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("entity");
    node.append_attribute("name") = entity.name.c_str();
    node.append_attribute("rat") = entity.spriteName.c_str();
    node.append_attribute("offset-x") = entity.offset.x;
    node.append_attribute("offset-y") = entity.offset.y;
    std::vector<sf::Vector2f> const &vertices = entity.mesh.getVertices();
    for (int i = 0; i < vertices.size(); i++) {
        pugi::xml_node point = node.append_child("point");
        point.append_attribute("x") = vertices[i].x;
        point.append_attribute("y") = vertices[i].y;
    }
    int success = doc.save_file(entity.file.c_str());
    if (!success) {
        spdlog::error(
            "Could not save entity to file {}. Does folder exist?",
            entity.file.c_str()
        );
    }
}

void FileIO::saveLevel(Level const &level) {
    spdlog::info("Saving level to {}", level.file.c_str());
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("level");
    node.append_attribute("pic") = level.getPicFile().c_str();
    pugi::xml_node scriptNode = node.append_child("script");
    scriptNode.text() = level.script.c_str();
    for (Instance const &instance: level.instances) {
        if (instance.entity) {
            pugi::xml_node entityNode = node.append_child("entityInstance");
            entityNode.append_attribute("name") = instance.name.c_str();
            entityNode.append_attribute("entity") =
                instance.entity->file.c_str();
            entityNode.append_attribute("x") = instance.pos.x;
            entityNode.append_attribute("y") = instance.pos.y;
            entityNode.append_attribute("size") = instance.size;
        } else {
            pugi::xml_node shapeNode = node.append_child("shapeInstance");
            shapeNode.append_attribute("name") = instance.name.c_str();
            for (sf::Vector2f const &point: instance.mesh.getVertices()) {
                pugi::xml_node pointNode = shapeNode.append_child("point");
                pointNode.append_attribute("x") = point.x;
                pointNode.append_attribute("y") = point.y;
            }
        }
    }
    int success = doc.save_file(level.file.c_str());
    if (!success) {
        spdlog::error(
            "Could not save level to file {}. Does folder exist?",
            level.file.c_str()
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
    sf::IntRect lineRat(0, 0, 0, 0);
    sf::IntRect lineHighlightRat(0, 0, 0, 0);
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
    core->start = node.attribute("start").value();
    // Check some optional things.
    pugi::xml_attribute ratPack = node.attribute("rat");
    if (ratPack) FileIO::initRatPackFromFile(core->spritesheet, ratPack.value());
    // now set the gui bits
    core->renderer.setPointRat(core->spritesheet.get(
        node.attribute("point").value()
    ));
    core->renderer.setCursorRat(core->spritesheet.get(
        node.attribute("pointer").value()
    ), Renderer::CursorType::pointer);
    core->renderer.setCursorRat(core->spritesheet.get(
        node.attribute("talk").value()
    ), Renderer::CursorType::talk);
    core->renderer.setCursorRat(core->spritesheet.get(
        node.attribute("move").value()
    ), Renderer::CursorType::move);
    core->renderer.setCursorRat(core->spritesheet.get(
        node.attribute("use").value()
    ), Renderer::CursorType::use);
    core->renderer.setPointHighlightRat(core->spritesheet.get(
        node.attribute("pointHighlight").value()
    ));
    core->renderer.setLineRat(core->spritesheet.get(
        node.attribute("line").value()
    ));
    core->renderer.setLineHighlightRat(core->spritesheet.get(
        node.attribute("lineHighlight").value()
    ));
    core->renderer.setNodeRat(core->spritesheet.get(
        node.attribute("node").value()
    ));
    core->renderer.setNodeHighlightRat(core->spritesheet.get(
        node.attribute("nodeHighlight").value()
    ));
    core->renderer.setFont(core->spritesheet.get(
        node.attribute("font").value()
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

Knob *FileIO::parseKnob(pugi::xml_node node) {
    int x = node.attribute("x").as_int();
    int y = node.attribute("y").as_int();
    int w = node.attribute("w").as_int();
    int h = node.attribute("h").as_int();
    int id = node.attribute("id").as_int();
    char const *type = node.name();
    if (strcmp(type, "panel") == 0) {
        PanelKnob *panel = new PanelKnob(
            x,
            y,
            w,
            h,
            node.attribute("parts").as_int()
        );
        for (pugi::xml_node child: node.children()) {
            panel->addChild(FileIO::parseKnob(child));
        }
        return panel;
    } else if (strcmp(type, "button") == 0) {
        pugi::xml_node child = node.first_child();
        return new ButtonKnob(x, y, w, h, id, FileIO::parseKnob(child));
    } else if (strcmp(type, "text") == 0) {
        TextKnob *text = new TextKnob(x, y, w, h, node.child_value());
        return text;
    }
}
