#ifndef OPTIONS_H
#define OPTIONS_H

/**
 * Represents the options that the program has been configured with like from
 * the commandline or elsewhere.
 */
class Options {
    public:
        std::string command;
        ghc::filesystem::path file;
        int levelFlag = true;
        int helpFlag = false;
};

#endif
