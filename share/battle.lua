local battle = {}

battle.FAILURE = 0
battle.SUCCESS = 1

function battle.wait(time)
    while time > 0 do
        time = time - coroutine.yield()
    end
end

function battle.start(file)
    _stopMusic()
    _playSound("sound/transition.wav")
    local strength = 0
    while strength < 1 do
        strength = strength + coroutine.yield()
        _setTransitionStrength(strength)
    end
    _battle(file)
    return coroutine.yield()
end

return battle
