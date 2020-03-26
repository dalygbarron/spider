#ifndef SOUND_PLAYER_H
#define SOUND_PLAYER_H

#include "filesystem.hh"
#include <SFML/Audio.hpp>

/**
 * Plays sound effects.
 */
class SoundPlayer {
    public:
        SoundPlayer(int size);

        /**
         * Finds a free sound player and plays this souind in it.
         * @param sound is the sound to play.
         */
        void playSound(sf::SoundBuffer *sound);

        /**
         * Plays some music from the given file. If the file is invalid it
         * kills the current music and does nothing, and if this file is
         * already playing it just does nothing.
         * @param path is the path to the music file to play.
         */
        void playMusic(ghc::filesystem::path const &path);

        /**
         * Stops the currently playing music.
         */
        void stopMusic();

    private:
        std::vector<sf::Sound> sounds;
        sf::Music music;
        std::string musicName;

};

#endif
