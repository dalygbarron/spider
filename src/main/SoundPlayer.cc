#include "SoundPlayer.hh"
#include "spdlog/spdlog.h"

SoundPlayer::SoundPlayer(int size) {
    this->sounds.resize(size);
}

void SoundPlayer::playSound(sf::SoundBuffer *sound) {
    for (sf::Sound &player: this->sounds) {
        if (player.getStatus() == sf::Sound::Status::Stopped) {
            player.setBuffer(*sound);
            player.play();
            return;
        }
    }
}

void SoundPlayer::playMusic(ghc::filesystem::path const &path) {
    char const *filename = path.c_str();
    if (strcmp(filename, musicName.c_str()) == 0) return;
    if (!this->music.openFromFile(filename)) {
        spdlog::error("Could not open music '{}'", filename);
    }
    this->music.setLoop(true);
    this->music.play();
}

void SoundPlayer::stopMusic() {
    this->music.stop();
}
