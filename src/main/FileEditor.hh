#ifndef FILE_EDITOR_H
#define FILE_EDITOR_H

/**
 * Kind of like a subscreen type thing for editing a certain type of asset.
 */
class FileEditor {
    public:
        /**
         * Takes a path to a file and returns an editor for that specific file
         * which will be the correct type based on file extension. If there is
         * no correct editor for the given extension it will return null.
         * @param path is the path to the file to edit.
         * @return the created file editor if one was created ya know.
         */
        static FileEditor *createForFile(ghc::filesystem::path path);

        /**
         * Creates it by giving access to the core.
         * @param core is the core thingy that gives access to shiet.
         */
        FileEditor(Core &core);

        /**
         * Deletes it. Might be needed for some subclass.
         */
        virtual ~FileEditor();

        /**
         * Called when there is a click.
         * @param button is the button that was clicked.
         * @param pos    is the logical screen location that was clicked.
         */
        virtual void onClick(sf::Mouse::Button button, glm::ivec2 pos);

        /**
         * Called when the user drags.
         * @param prev is the previous mouse ppsition.
         * @param pos  aiiiiiiidddddsssss.s.s.s..s.s.s.
         */
        virtual void onDrag(glm::ivec2 prev, glm::ivec2 pos);

        /**
         * Called when the user scrolls the mouse wheel.
         * @param delta is the amount of clicks that they scrolled by or
         *              something or nothing.
         */
        virtual void onScroll(int delta);
};

/**
 * For editing text files, does syntax highlighting for xml files and lua files
 * also yeiiesiiise.
 */
class TextFileEditor: public FileEditor {

};

#endif
