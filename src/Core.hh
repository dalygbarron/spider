#ifndef CORE_H
#define CORE_H

#include "Repository.hh"
#include "RatPack.hh"
#include "RatBatch.hh"

/**
 * Core engine parts that form the common dependencies of every screen.
 */
class Core {
    public:
        std::string name;
        ghc::filesystem::path filename;
        RatPack spritesheet;
        RatBatch batch;
        SoundBufferRepository soundBufferRepository;
        EntityRepository entityRepository;
        LevelRepository levelRepository;

        /**
         * Initialises the core.
         */
        Core();
};

#endif
