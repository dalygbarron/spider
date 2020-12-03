#ifndef FILE_EDITOR_H
#define FILE_EDITOR_H

#include "Core.hh"
#include "Text.hh"
#include "TextEditor.h"

/**
 * Kind of like a subscreen type thing for editing a certain type of asset.
 */
class FileEditor {
    public:
        /**
         * Takes a path to a file and returns an editor for that specific file
         * which will be the correct type based on file extension. If there is
         * no correct editor for the given extension it will return null.
         * @param core is used to access repos and stuff at time of creation.
         * @param path is the path to the file to edit.
         * @return the created file editor if one was created ya know.
         */
        static FileEditor *createForFile(
            Core &core,
            ghc::filesystem::path path
        );

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

        /**
         * Updates this shiet.
         * @param delta  is the time since before.
         */
        virtual void update(float delta) = 0;

        /**
         * Draws the editor upon the given render target.
         * @param target is the screen to render onto.
         */
        virtual void draw(sf::RenderTarget &target) const = 0;
};

/**
 * Editor for entities.
 */
class EntityFileEditor: public FileEditor {
    public:
        /**
         * Creates the editor.
         * @param entity is a mutable reference to the entity we are editing.
         */
        EntityFileEditor(Entity &entity);

        virtual void update(float delta) override;

        virtual void draw(sf::RenderTarget &target) const override;

    private:
        Entity &entity;
};

/**
 * Editor for levels.
 */
class LevelFileEditor: public FileEditor {
    public:
        /**
         * Creates the editor.
         * @param level is a mutable reference to the level that we are
         *        editing.
         */
        LevelFileEditor(Level &level);

        virtual void update(float delta) override;

        virtual void draw(sf::RenderTarget &target) const override;

    private:
        Level &level;
};

/**
 * For editing text files, does syntax highlighting for xml files and lua files
 * also yeiiesiiise.
 */
class TextFileEditor: public FileEditor {
    public:
        enum class TextFileType {
            TXT,
            LUA,
            XML
        };

        /**
         * Creates the text file editor.
         * @param text is the text object it is editing.
         * @param type is the file type which determines how highlighting
         *             will work if there is any.
         */
        TextFileEditor(Text &text, TextFileType type);

        virtual void update(float delta) override;

        virtual void draw(sf::RenderTarget &target) const override;

    private:
        TextEditor textEditor;
        Text &text;
        TextFileType type;
        
};

#endif
