#include "Screen.hh"
#include "FileIO.hh"
#include "Knob.hh"

ScriptedScreen::ScriptedScreen(Core &core, std::string const &code):
    Screen(core)
{
    this->initScript();
    this->script.script(code);
}

ScriptedScreen::ScriptedScreen(Core &core, ghc::filesystem::path const &path):
    Screen(core)
{
    this->initScript();
    this->script.script_file(path.c_str());
}

int ScriptedScreen::getLastResponse() const {
    return this->response;
}

void ScriptedScreen::setScript(char const *name) {
    this->coroutine = this->script[name];
}

void ScriptedScreen::refresh() {
    // By default this does nothing.
}

void ScriptedScreen::initScript() {
    this->script.open_libraries(
        sol::lib::base,
        sol::lib::coroutine,
        sol::lib::package,
        sol::lib::math,
        sol::lib::table,
        sol::lib::string
    );
    sol::usertype<sf::Vector2f> vectorType = this->script.new_usertype<sf::Vector2f>(
        "Vector2f",
        sol::constructors<sf::Vector2f(), sf::Vector2f(float, float)>()
    );
    vectorType["x"] = &sf::Vector2f::x;
    vectorType["y"] = &sf::Vector2f::y;
    sol::usertype<Item> itemType = this->script.new_usertype<Item>(
        "Item",
        sol::constructors<Item()>()
    );
    itemType["name"] = &Item::name;
    itemType["displayName"] = &Item::displayName;
    itemType["description"] = &Item::description;
    itemType["rat"] = &Item::rat;
    this->script["_itemInfo"] = this->script.create_table();
    int i = 1;
    for (std::pair<std::string, Item> const &item: this->core.getItems()) {
        this->script["_itemInfo"][i] = item.second;
        i++;
    }
    this->script["_defaultFont"] = this->core.defaultFont.c_str();
    this->script["_inventory"] = [this]() {
        sol::table inventoryTable = this->script.create_table();
        for (std::pair<std::string, int> const &item:
            this->core.getMemory().getItems()
        ) {
            inventoryTable[item.first.c_str()] = item.second;
        }
        return inventoryTable;
    };
    this->script["_addItem"] = [this](std::string name) {
        Memory &memory = this->core.getMemory();
        char const *nameString = name.c_str();
        memory.setItemCount(nameString, memory.getItemCount(nameString) + 1);
        return this->core.getMemory().getItemCount(name.c_str());
    };
    this->script["_getString"] = [this](
        std::string const &name
    ) -> char const * {
        return this->core.getMemory().getString(name.c_str());
    };
    this->script["_setString"] = [this](
        std::string const &name,
        std::string const &value
    ) {
        this->core.getMemory().setString(name.c_str(), value.c_str());
    };
    this->script["_getSwitch"] = [this](std::string const &name) -> bool {
        return this->core.getMemory().getSwitch(name.c_str());
    };
    this->script["_getLocalSwitch"] = [this](std::string const &name) -> bool {
        return this->core.getMemory().getLocalSwitch(name.c_str());
    };
    this->script["_setSwitch"] = [this](std::string const &name, bool value) {
        return this->core.getMemory().setSwitch(name.c_str(), value);
    };
    this->script["_setLocalSwitch"] = [this](
        std::string const &name,
        bool value
    ) {
        return this->core.getMemory().setLocalSwitch(name.c_str(), value);
    };
    this->script["_setTransitionStrength"] = [this](float strength) {
        this->core.setTransitionStrength(strength);
    };
    this->script["_playSound"] = [this](std::string const &sound) {
        sf::SoundBuffer *buffer = this->core.soundBufferRepository.get(
            sound.c_str()
        );
        this->core.soundPlayer.playSound(buffer);
    };
    this->script["_playMusic"] = [this](std::string const &music) {
        this->core.soundPlayer.playMusic(music.c_str());
    };
    this->script["_stopMusic"] = [this]() {
        this->core.soundPlayer.stopMusic();
    };
    this->script["_saveGame"] = [this]() {
        this->core.saveGame();
    };
    this->script["_loadGame"] = [this](int id) {
        this->core.loadGame(id);
        return this->core.getMemory().level;
    };
    this->script["_newGame"] = [this](int id) {
        this->core.newGame(id);
    };
    this->script["_systemInfo"] = []() {
        return std::make_tuple(8, 8);
    };
    this->script["_xmlKnob"] = [this](std::string const &xml) {
        spdlog::debug("Adding knob xml: '{}'", xml.c_str());
        Knob *knob = FileIO::readXml<Knob, RatPack const &>(
            xml.c_str(),
            FileIO::parseKnob,
            this->core.spritesheet
        );
        if (knob) {
            this->core.pushScreen(new KnobScreen(this->core, knob));
        } else {
            spdlog::error("API: Invalid argument to _xmlKnob");
        }
    };
    this->script["_logInfo"] = [this](std::string const &value) {
        spdlog::info(value);
    };
    this->script["_logWarning"] = [this](std::string const &value) {
        spdlog::warn(value);
    };
    this->script["_logError"] = [this](std::string const &value) {
        spdlog::error(value);
    };
    this->script["_exit"] = [this](int response) {
        this->core.popScreen(response);
    };
}
