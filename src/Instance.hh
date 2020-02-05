#ifndef INSTANCE_H
#define INSTANCE_H

/**
 * An instance of a thing that can appear within a level.
 */
class Instance {
    public:
        enum {
            MESH,
            ENTITY
        } type;
        union {
            Mesh mesh;
            Entity const *entity;
        } content;
        std::string name;
};

#endif
