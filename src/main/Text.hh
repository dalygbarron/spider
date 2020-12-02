#ifndef TEXT_H
#define TEXT_H

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



    private:
        ghc::filesystem::path path;
        std::string content;
};

#endif
