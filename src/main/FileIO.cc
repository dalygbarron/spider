#include "FileIO.hh"
#include "Const.hh"
#include <iostream>
#include <cstdlib>

Memory::SwitchExpression *FileIO::parseSwitchExpression(char const *string) {
    std::stringstream stream(string);
    std::string token;
    std::deque<Memory::SwitchExpression *> stack;
    int i = 1;
    while (getline(stream, token, ' ')) {
        int n = stack.size();
        char const *cString = token.c_str();
        if (cString[0] == '&') {
            if (n < 2) goto abort;
            Memory::SwitchExpression *a = stack.back();
            stack.pop_back();
            Memory::SwitchExpression *b = stack.back();
            stack.back() = new Memory::SwitchExpression(
                Memory::SwitchExpression::Type::And,
                NULL,
                a,
                b
            );
        } else if (cString[0] == '|') {
            if (n < 2) goto abort;
            Memory::SwitchExpression *a = stack.back();
            stack.pop_back();
            Memory::SwitchExpression *b = stack.back();
            stack.back() = new Memory::SwitchExpression(
                Memory::SwitchExpression::Type::Or,
                NULL,
                a,
                b
            );
        } else if (cString[0] == '!') {
            if (n == 0) goto abort;
            stack.back() = new Memory::SwitchExpression(
                Memory::SwitchExpression::Type::Not,
                NULL,
                stack.back(),
                NULL
            );
        } else if (cString[0] == '.') {
            stack.push_back(new Memory::SwitchExpression(
                Memory::SwitchExpression::Type::LocalSwitch,
                cString + 1,
                NULL,
                NULL
            ));
        } else {
            stack.push_back(new Memory::SwitchExpression(
                Memory::SwitchExpression::Type::Switch,
                cString,
                NULL,
                NULL
            ));
        }
        i++;
    }
    while (stack.size() > 1) {
        Memory::SwitchExpression *a = stack.back();
        stack.pop_back();
        Memory::SwitchExpression *b = stack.back();
        stack.back() = new Memory::SwitchExpression(
            Memory::SwitchExpression::Type::And,
            NULL,
            a,
            b
        );
    }
    if (stack.size() == 1) return stack.back();
    return NULL;
    abort:
        spdlog::error("Switch expression error at token {}", i);
        while (!stack.empty()) {
            delete stack.back();
            stack.pop_back();
        }
        return NULL;
}

Memory FileIO::loadMemory(int id) {
    char *text;
    asprintf(&text, ".save%d.xml", id);
    ghc::filesystem::path path = text;
    delete text;
    Memory memory(id);
    if (ghc::filesystem::exists(path)) {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(path.c_str());
        if (!result) {
            spdlog::error("Cannot load save file from '{}'", path.c_str());
            return memory;
        }
        pugi::xml_node node = doc.child("save");
        if (!node) {
            spdlog::error("'{}' is not a valid save file", path.c_str());
            return memory;
        }
        memory.level = node.attribute("level").value();
        for (pugi::xml_node child: node.children()) {
            char const *type = child.name();
            if (strcmp(type, "switch") == 0) {
                memory.setSwitch(
                    child.attribute("name").value(),
                    child.attribute("value").as_bool()
                );
            } else if (strcmp(type, "item") == 0) {
                memory.setItemCount(
                    child.attribute("name").value(),
                    child.attribute("count").as_int()
                );
            } else if (strcmp(type, "local") == 0) {
                for (pugi::xml_node localChild: child.children()) {
                    memory.setLocalSwitchStatic(
                        child.attribute("level").value(),
                        localChild.attribute("name").value(),
                        localChild.attribute("value").as_bool()
                    );
                }
            } else if (strcmp(type, "string") == 0) {
                memory.setString(
                    child.attribute("name").value(),
                    child.attribute("value").value()
                );
            } else {
                spdlog::error("Unknown entry type '{}' in memory file", type);
            }
        }
    }
    return memory;
}

void FileIO::saveMemory(Memory const &memory) {
    char *text;
    asprintf(&text, ".save%d.xml", memory.getId());
    ghc::filesystem::path path = text;
    delete text;
    pugi::xml_document doc;
    pugi::xml_node node = doc.append_child("save");
    node.append_attribute("level") = memory.level.c_str();
    for (std::pair<std::string const, int> item: memory.getSwitches()) {
        pugi::xml_node child = node.append_child("switch");
        child.append_attribute("name") = item.first.c_str();
        child.append_attribute("value") = item.second;
    }
    for (std::pair<std::string const, int> item: memory.getItems()) {
        pugi::xml_node child = node.append_child("item");
        child.append_attribute("name") = item.first.c_str();
        child.append_attribute("count") = item.second;
    }
    for (std::pair<std::string, std::unordered_map<std::string, int>> level:
        memory.getLocalSwitches()
    ) {
        pugi::xml_node levelNode = node.append_child("local");
        levelNode.append_attribute("level") = level.first.c_str();
        for (std::pair<std::string, int> item: level.second) {
            pugi::xml_node child = levelNode.append_child("switch");
            child.append_attribute("name") = item.first.c_str();
            child.append_attribute("value") = item.second;
        }
    }
    int success = doc.save_file(path.c_str());
    if (!success) spdlog::error("could not save to '{}'", path.c_str());
}

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
        core.renderer.boxPatch = patch;
    } else if (strcmp(role, "boxHighlight") == 0) {
        core.renderer.boxHighlightPatch = patch;
    } else if (strcmp(role, "panel") == 0) {
        core.renderer.panelPatch = patch;
    } else if (strcmp(role, "button") == 0) {
        core.renderer.buttonPatch = patch;
    } else if (strcmp(role, "buttonDepressed") == 0) {
        core.renderer.buttonDepressedPatch = patch;
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
    std::vector<glm::vec2> const &vertices = entity.mesh.getVertices();
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
            for (glm::vec2 const &point: instance.mesh.getVertices()) {
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
        pack.add(rat.attribute("name").value(), Rectangle(
            rat.attribute("x").as_int(),
            rat.attribute("y").as_int(),
            rat.attribute("w").as_int(),
            rat.attribute("h").as_int()
        ));
    }
}

Core *FileIO::loadCoreFromFile(
    ghc::filesystem::path const &path,
    int allowMusic
) {
    sf::IntRect pointRat(0, 0, 0, 0);
    sf::IntRect lineRat(0, 0, 0, 0);
    sf::IntRect lineHighlightRat(0, 0, 0, 0);
    sf::IntRect nodeRat(0, 0, 0, 0);
    sf::IntRect nodeHighlightRat(0, 0, 0, 0);
    sf::IntRect arcRat(0, 0, 0, 0);
    sf::IntRect boxRat(0, 0, 0, 0);
    sf::IntRect boxHighlightRat(0, 0, 0, 0);
    if (!ghc::filesystem::exists(path)) {
        spdlog::info("Creating new game core at '{}'", path.c_str());
        // TODO: is this still ok?
        Core *core = new Core(allowMusic);
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
    Core *core = new Core(allowMusic);
    core->filename = path;
    core->name = node.attribute("name").value();
    core->start = node.attribute("start").value();
    // load spritesheet.
    pugi::xml_attribute ratPack = node.attribute("rat");
    if (ratPack) FileIO::initRatPackFromFile(core->spritesheet, ratPack.value());
    // load display params.
    core->setDisplay(
        glm::ivec2(
            node.attribute("width").as_int(),
            node.attribute("height").as_int()
        ),
        node.attribute("fov").as_float()
    );
    // load sprite related stuff.
    core->defaultFont = node.attribute("default-font").value();
    core->setTransitionTexture(node.attribute("transition").value());
    // now set the gui bits
    core->renderer.setCursorRat(
        core->spritesheet.get(node.attribute("pointer").value()),
        Renderer::CursorType::Pointer
    );
    core->renderer.setCursorRat(
        core->spritesheet.get(node.attribute("talk").value()),
        Renderer::CursorType::Talk
    );
    core->renderer.setCursorRat(
        core->spritesheet.get(node.attribute("move").value()),
        Renderer::CursorType::Move
    );
    core->renderer.setCursorRat(
        core->spritesheet.get(node.attribute("use").value()),
        Renderer::CursorType::Use
    );
    core->renderer.font = core->spritesheet.get(core->defaultFont.c_str());
    core->renderer.battleFont =
        core->spritesheet.get(node.attribute("battle-font").value());
    core->renderer.battleRat =
        core->spritesheet.get(node.attribute("battle").value());
    core->renderer.pointRat =
        core->spritesheet.get(node.attribute("point").value());
    core->renderer.pointHighlightRat =
        core->spritesheet.get(node.attribute("pointHighlight").value());
    core->renderer.lineRat =
        core->spritesheet.get(node.attribute("line").value());
    core->renderer.lineHighlightRat =
        core->spritesheet.get(node.attribute("lineHighlight").value());
    core->renderer.nodeRat =
        core->spritesheet.get(node.attribute("node").value());
    core->renderer.nodeHighlightRat =
        core->spritesheet.get(node.attribute("nodeHighlight").value());
    // Now look for child nodes to deal with.
    for (pugi::xml_node child: node.children()) {
        char const *type = child.name();
        if (strcmp(type, "patch") == 0) {
            FileIO::parsePatch(*core, child);
        } else if (strcmp(type, "item") == 0) {
            core->addItem(
                child.attribute("name").value(),
                child.attribute("display-name").value(),
                child.attribute("description").value(),
                child.attribute("rat").value(),
                core->spritesheet.get(child.attribute("rat").value())
            );
        } else if (strcmp(type, "bullet") == 0) {
            core->addBulletPrototype(
                child.attribute("id").as_int(),
                core->spritesheet.get(child.attribute("rat").value())
            );
        } else {
            spdlog::warn("Unknown child type '{}' in game file", type);
        }
    }
    return core;
}

Knob *FileIO::parseKnob(pugi::xml_node node, RatPack const &spritesheet) {
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
            panel->addChild(FileIO::parseKnob(child, spritesheet));
        }
        return panel;
    } else if (strcmp(type, "button") == 0) {
        pugi::xml_node child = node.first_child();
        return new ButtonKnob(
            x,
            y,
            w,
            h,
            id,
            FileIO::parseKnob(child, spritesheet)
        );
    } else if (strcmp(type, "text") == 0) {
        return new TextKnob(
            x,
            y,
            w,
            h,
            spritesheet.get(node.attribute("font").value()),
            node.child_value()
        );
    } else if (strcmp(type, "frame") == 0) {
        return new FrameKnob(x, y, w, h, spritesheet.get(
            node.attribute("rat").value()
        ));
    } else {
        spdlog::error("No knob type '{}'", type);
        return NULL;
    }
}

World *FileIO::parseWorld(
    pugi::xml_node node,
    Core &core
) {
    sf::Texture *ground = new sf::Texture();
    if (!ground->loadFromFile(node.attribute("ground").value())) {
        spdlog::error(
            "Couldn't load file '{}' as ground texture for world",
            node.attribute("ground").value()
        );
    }
    ground->setRepeated(true);
    ground->setSmooth(true);
    ground->generateMipmap();
    sf::Color col(strtoul(node.attribute("horizon").value(), NULL, 16));
    World *world = new World(
        ground,
        sf::Color(strtoul(node.attribute("horizon").value(), NULL, 16)),
        sf::Color(strtoul(node.attribute("bottomSky").value(), NULL, 16)),
        sf::Color(strtoul(node.attribute("topSky").value(), NULL, 16)),
        node.attribute("waves").as_float(),
        core.getSize(),
        core.getFov()
    );
    for (pugi::xml_node child: node.children()) {
        Entity const *entity = core.entityRepository.get(
            child.attribute("name").value()
        );
        if (entity) {
            world->addLindel(*entity, glm::vec3(
                child.attribute("x").as_float(),
                child.attribute("y").as_float(),
                child.attribute("z").as_float()
            ));
        } else {
            spdlog::error(
                "'{}' does not name an entity",
                child.attribute("name").value()
            );
        }
    }
    return world;
}
