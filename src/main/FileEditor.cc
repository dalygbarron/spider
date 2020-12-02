#include "FileEditor.hh"

static FileEditor *FileEditor::createForFile(
    Core &core,
    ghc::filesystem::path path
) {
    char const *ext = path.extension().c_str();
    if (strcmp(ext, "pex") == 0) {
        return new EntityFileEditor(core, path);
    } else if (strcmp(ext, "plx") == 0) {
        return new LevelFileEditor(core, path);
    } else if (strcmp(ext, "lua") == 0) {
        return new TextFileEditor(
            core,
            path,
            TextFileEditor::TextFileType::LUA
        );
    } else if (strcmp(ext, "xml") == 0) {
        return new TextFileEditor(
            core,
            path,
            TextFileEditor::TextFileType::XML
        );
    } else if (strcmp(ext, "txt") == 0) {
        return new TextFileEditor(
            core,
            path,
            TextFileEditor::TextFileType::PLAIN
        );
    }
    spdlog::warning("{} is not a supported file type", ext);
    return NULL;
}

FileEditor::~FileEditor() {}

void FileEditor::onClick(sf::Mouse::Button button, glm::ivec2 pos) {}

void FileEditor::onDrag(glm::ivec2 prev, glm::ivec2 pos) {}

void FileEditor::onScroll(int delta) {}
