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
        ghc::filesystem::path filename;
        RatPack spritesheet;
        SoundBufferRepository soundBufferRepository;
        EntityRepository entityRepository;
        LevelRepository levelRepository;
};

#endif
