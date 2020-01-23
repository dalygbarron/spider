#include "Repository.hh"

sf::SoundBuffer *SoundBufferRepository::load(char const *key) const {
    sf::SoundBuffer *soundBuffer = new sf::SoundBuffer();
    if (!soundBuffer->loadFromFile(key)) {
        delete soundBuffer;
        return NULL;
    }
    return soundBuffer;
}
