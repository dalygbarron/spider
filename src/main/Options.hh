#ifndef OPTIONS_H
#define OPTIONS_H

/**
 * Represents the options that the program has been configured with like from
 * the commandline or elsewhere.
 */
class Options {
    public:
        ghc::filesystem::path game = "game.xml";
        ghc::filesystem::path file;
        int entityFlag = false;
        int levelFlag = false;
        int battleFlag = false;
        int ratFlag = false;
        int helpFlag = false;
        int versionFlag = false;
        int muteFlag = false;
};

#endif
