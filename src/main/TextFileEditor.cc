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
            this->textEditor.SetLanguageDefinition(
                TextEditor::LanguageDefinition::Xml()
            );
    }
    this->textEditor.setText(text.get());
}

void TextFileEditor::draw(sf::RenderTarget &target) {
    auto cpos = this->textEditor.GetCursorPosition();
    if (ImGui::Begin("Script Editor")) {
        this->textEditor.Render("brexit");
    }
    ImGui::End();
}
