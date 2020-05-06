#include "Entity.hh"

Entity::Behaviour stringToBehaviour(char const *string) {
    static std::unordered_map<std::string, Entity::Behaviour> map {
       ("Static", Entity::Behaviour::Static),
       ("Flap", Entity::Behaviour::Flap),
       ("Jump", Entity::Behaviour::Jump),
       ("Walk", Entity::Behaviour::Walk),
       ("Glide", Entity::Behaviour::Glide),
       ("Swim", Entity::Behaviour::Swim),
       ("Neander", Entity::Behaviour::Neander),
       ("TargetPlayer", Entity::Behaviour::TargetPlayer),
       ("TargetAverage", Entity::Behaviour::TargetAverage),
       ("TargetSelf", Entity::Behaviour::TargetSelf),
       ("TargetRandomAbove", Entity::Behaviour::TargetRandomAbove),
       ("TargetRandomGround", Entity::Behaviour::TargetRandomGround),
       ("TargetRandomBelow", Entity::Behaviour::TargetRandomBelow)
    };
    if (map.count(string) == 0) return Behaviour::Static;
    return map.at(string);
}

Entity::Condition stringToCondition(char const *string) {
    static std::unordered_map<std::string, Entity::Condition> {
        ("Now", Entity::Condition::Now),
        ("Ground", Entity::Condition::Ground),
        ("Above", Entity::Condition::Above),
        ("Below", Entity::Condition::Below),
        ("PlayerNear", Entity::Condition::PlayerNear),
        ("PlayerFar", Entity::Condition::PlayerFar),
        ("Stationary", Entity::Condition::Stationary),
        ("Timer", Entity::Condition::Timer),
        ("Random", Entity::Condition::Random)
    };
    if (map.count(string) == 0) return Condition::Now;
    return map.at(string);
}
