#ifndef TEXT_H
#define TEXT_H

#include "filesystem.hh"

/**
 * Represents text from a file which can be changed and can be saved to etc.
 */
class Text {
    public:
        /**
         * Creates the text by loading it from a given file.
         * @param path is the path to the file.
         */
        Text(ghc::filesystem::path path);

        /**
         * Sets the content of the text thing for all it's users.
         * @param string is the value to set it to.
         */
        void set(char const *string);

        /**
         * Gets the value of the text for your enjoyment.
         * @return a c string of it.
         */
        char const *get() const;

        /**
         * Saves the text in it's latest form to the file it came from.
         * @return true if it all went find and false if there was a fuckup.
         */
        bool save() const;

    private:
        ghc::filesystem::path path;
        std::string content;
};

#endif
