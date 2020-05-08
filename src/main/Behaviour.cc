#include "Behaviour.hh"
#include "Util.hh"

static char const *styleStrings[] = {
    "Static",
    "Flap",
    "Jump",
    "Walk",
    "Glide",
    "Swim",
    "Neander",
    "TargetPlayer",
    "TargetAverage",
    "TargetSelf",
    "TargetRandomAbove",
    "TargetRandomGround",
    "TargetRandomBelow"
};

static char const *conditionStrings[] = {
    "Now",
    "Ground",
    "Above",
    "Below",
    "PlayerNear",
    "PlayerFar",
    "Stationary",
    "Timer",
    "Random"
};

Behaviour::Style Behaviour::stringToStyle(char const *string) {
    int find = Util::findString(
        string,
        styleStrings,
        sizeof(styleStrings) / sizeof(char const *)
    );
    if (find >= 0) return static_cast<Behaviour::Style>(find);
    return Behaviour::Style::Static;
}

Behaviour::Condition Behaviour::stringToCondition(char const *string) {
    int find = Util::findString(
        string,
        conditionStrings,
        sizeof(conditionStrings) / sizeof(char const *)
    );
    if (find >= 0) return static_cast<Behaviour::Condition>(find);
    return Behaviour::Condition::Now;
}
