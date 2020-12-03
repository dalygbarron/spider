#include "FileEditor.hh"

TextFileEditor::TextFileEditor(Text &text, TextFileType type):
    text(text)
{
    switch (type) {
        case TextFileType::LUA:
            this->textEditor.SetLanguageDefinition(
                TextEditor::LanguageDefinition::Lua()
            );
            break;
        case TextFileType::XML:
            break;
            //this->textEditor.SetLanguageDefinition(
            //    TextEditor::LanguageDefinition::Xml()
            //);
        default:
            break;
    }
    this->textEditor.SetText(text.get());
}

void TextFileEditor::update(float delta) {
    auto cpos = this->textEditor.GetCursorPosition();
    if (ImGui::Begin("Script Editor")) {
        this->textEditor.Render("brexit");
        ImGui::End();
    }
}

void TextFileEditor::draw(sf::RenderTarget &target) const {}
