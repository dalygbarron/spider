#ifndef BEHAVIOUR_H
#define BEHAVIOUR_H

#include <string>
#include <unordered_map>
#include <vector>

/**
 * Used to make a lindel do what it should do.
 */
class Behaviour {
    public:
        /**
         * Represents styles of behaviour that lindel states can induce.
         */
        enum class Style {
            Static,
            Flap,
            Jump,
            Walk,
            Glide,
            Swim,
            Neander,
            TargetPlayer,
            TargetAverage,
            TargetSelf,
            TargetRandomAbove,
            TargetRandomGround,
            TargetRandomBelow
        };

        /**
         * Possible transition conditions between statess.
         */
        enum class Condition {
            Now,
            Ground,
            Above,
            Below,
            PlayerNear,
            PlayerFar,
            Stationary,
            Timer,
            Random
        };

        /**
         * Represents a transition from a lindel state to another for a given
         * entity.
         */
        class Transition {
            public:
                Behaviour::Condition condition;
                float param;
                std::string state;
        };

        /**
         * Represents a state that a lindel instance of an entity can be in.
         */
        class State {
            public:
                Behaviour::Style style;
                float param;
                std::vector<Behaviour::Transition> transitions;
        };

        std::unordered_map<std::string, Behaviour::State> states;
        Behaviour::State const *start;

        /**
         * Converts a string to a style enum.
         * @param string is the string to convert to a style.
         * @return the style which defaults to static if you give shit.
         */
        static Behaviour::Style stringToStyle(char const *string);

        /**
         * Converts a string to a condition enum.
         * @param string is the string to convert to a condition.
         * @return the behavior which defaults to now if you give shit.
         */
        static Behaviour::Condition stringToCondition(char const *string);
};

#endif
