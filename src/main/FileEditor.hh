#ifndef FILE_EDITOR_H
#define FILE_EDITOR_H

/**
 * Kind of like a subscreen type thing for editing a certain type of asset.
 */
class FileEditor {
    public:
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
         * @param pos  is the current mouse position.
         */
        virtual void onDrag(glm::ivec2 prev, glm::ivec2 pos);

        /**
         * Called when the user scrolls the mouse wheel.
         * @param delta is the amount of clicks that they scrolled by or
         *              something or nothing.
         */
        virtual void onScroll(int delta);
};

#endif
