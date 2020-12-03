#include "Text.hh"

Text::Text(ghc::filesystem::path path) {
    this->content = FileIO::readFile(path);
    this->path = path;
}

void Text::set(char const *string) {
    this->content = string;
}

char const *Text::get() {
    return this->content.c_str();
}

void Text::save() {
    FILE *outFile = fopen(this->path.c_str(), "w");
    int amount = 0;
    if (outFile) {
        amount = fputs(this->content.c_str(), outFile);
        fclose(outFile);
        return (amount >= 0);
    }
    return false;
}
