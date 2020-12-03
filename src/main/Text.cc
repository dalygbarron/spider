#include "Text.hh"
#include "FileIO.hh"

Text::Text(ghc::filesystem::path path) {
    char const *string = FileIO::readFile(path);
    this->content = string;
    delete string;
    this->path = path;
}

void Text::set(char const *string) {
    this->content = string;
}

char const *Text::get() const {
    return this->content.c_str();
}

bool Text::save() const {
    FILE *outFile = fopen(this->path.c_str(), "w");
    int amount = 0;
    if (outFile) {
        amount = fputs(this->content.c_str(), outFile);
        fclose(outFile);
        return (amount >= 0);
    }
    return false;
}
