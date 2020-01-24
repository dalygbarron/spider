#ifndef CORE_H
#define CORE_H

/**
 * Core engine parts that form the common dependencies of every screen.
 */
class Core {
    public:
        SoundBufferRepository soundBufferRepository;
        EntityRepository entityRepository;
        LevelRepository levelRepository;

        RatPack const &getRatPack();

    private:
        RatPack spritesheet;
};


#endif
