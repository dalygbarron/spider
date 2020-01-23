#include "Repository.hh"

sf::Sound *SoundRepository::load(char const *key) const {
    sf::Sound *sound = new sf::Sound();
    if (!sound->loadFromFile(key)) {
        delete sound;
        return NULL;
    }
    return sound;
}
