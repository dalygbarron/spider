#include "Repository.hh"

Text *TextRepository::load(char const *key) const {
    ghc::filesystem::path path = key;
    return new Text(path);
}
