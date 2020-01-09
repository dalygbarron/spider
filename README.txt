Editor for designing adventure game scenes that are not flat but are in
a "photosphere" type of thingy. Does not deal with editing scripts or images,
but does let lay out levels, and also set click maps for things that appear
within levels.

Types of file that can be edited:
    - Level file:
        - Has a picture to use as the photosphere.
        - Has a script that controls the functionality of the level.
        - Has a collection of entities that exist within the level. These will
          be denoted at the top of the file and then referenced later in the
          list of entities with a specific offset and scale and name and birth
          key and death key and offset to draw ordering (normally ordered by
          how scaled they are), since this is for the specific instance.
        - Also has a collection of non generic named wire frames for clicking
          on. They are stored within the level because they are specifically
          shaped to stuff in the background of the level.

    - Entity File:
        - Has a type value which is generic or item.
        - Has a picture
        - Has a wireframe
        - Has a hotspot
        - If it is an item, the type of item.


Using the following:
    https://github.com/AirGuanZ/imgui-filebrowser
    https://github.com/ocornut/imgui
    https://github.com/eliasdaler/imgui-sfml
    https://github.com/SFML/SFML
