#ifndef CORE_H
#define CORE_H

#include "Repository.hh"
#include "RatPack.hh"

/**
 * Core engine parts that form the common dependencies of every screen.
 */
class Core {
    public:
        std::string name;
        std::filename;
        SoundBufferRepository soundBufferRepository;
        EntityRepository entityRepository;
        LevelRepository levelRepository;

        /**
         * Gives you a constant reference to the core's spritesheet.
         * @return the spritesheet.
         */
        RatPack const &getSpritesheet();

    private:
        RatPack spritesheet;
};


#endif
