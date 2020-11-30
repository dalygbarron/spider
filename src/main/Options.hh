#ifndef OPTIONS_H
#define OPTIONS_H

/**
 * Represents the options that the program has been configured with like from
 * the commandline or elsewhere.
 */
class Options {
    public:
        ghc::filesystem::path game = "game.xml";
        int editorFlag = false;
        int muteFlag = false;
};

#endif
