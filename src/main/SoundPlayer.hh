#ifndef SOUND_PLAYER_H
#define SOUND_PLAYER_H

#include "filesystem.hh"
#include <SFML/Audio.hpp>

/**
 * Plays sound effects.
 */
class SoundPlayer {
    public:
        /**
         * Creates the sound player.
         * @param size       is the number of sounds it can have at once.
         * @param allowMusic is whether to actually play music. if not it just
         *                   pretends.
         */
        SoundPlayer(int size, int allowMusic);

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
        int allowMusic;
};

#endif
