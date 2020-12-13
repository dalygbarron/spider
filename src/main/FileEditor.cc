#include "FileEditor.hh"

FileEditor *FileEditor::createForFile(
    Core &core,
    ghc::filesystem::path path
) {
    char const *ext = path.extension().c_str();
    TextFileEditor::TextFileType textType = TextFileEditor::TextFileType::TXT;
    if (strcmp(ext, ".psex") == 1) {
        Entity *entity = core.getEntity(path.c_str());
        if (entity) return new EntityFileEditor(*entity);
        return NULL;
    } else if (strcmp(ext, ".pslx") == 0) {
        return NULL;//return new LevelFileEditor(core, path);
    } else if (strcmp(ext, ".lua") == 0) {
        textType = TextFileEditor::TextFileType::LUA;
        goto textTime;
    } else if (strcmp(ext, ".xml") == 0) {
        textType = TextFileEditor::TextFileType::XML;
        goto textTime;
    } else if (strcmp(ext, ".txt") == 0) {
        goto textTime;
    }
    spdlog::warn("{} is not a supported file type", ext);
    return NULL;
    textTime:
        Text *text = core.textRepository.get(path.c_str());
        if (text) return new TextFileEditor(*text, textType);
        return NULL;
}

FileEditor::~FileEditor() {}

void FileEditor::onClick(sf::Mouse::Button button, glm::ivec2 pos) {}

void FileEditor::onDrag(glm::ivec2 prev, glm::ivec2 pos) {}

void FileEditor::onScroll(int delta) {}
