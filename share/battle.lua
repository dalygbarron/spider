local battle = {}

battle.FAILURE = 0
battle.SUCCESS = 1

function battle.wait(time)
    while time > 0 do
        time = time - coroutine.yield()
    end
end

function battle.distance(aX, aY, bX, bY)
    local x = math.abs(aX - bX)
    local y = math.abs(aY - bY)
    return math.sqrt(x * x + y * y)
end

function battle.angleTo(aX, aY, bX, bY)
    return math.atan((bX - aX) / (bY - aY))
end

function battle.walk(actor, speed, dX, dY)
    local x, y = _getActorPosition(actor)
    if x == -1 or y == -1 then
        return
    end
    local distance = battle.distance(x, y, dX, dY)
    local angle = battle.angleTo(x, y, dX, dY)
    print(x..","..y..","..dX..","..dY..","..angle)
    _setActorTransform(
        actor,
        x,
        y,
        math.sin(angle) * speed,
        math.cos(angle) * speed,
        0,
        0
    )
    battle.wait(distance / speed)
    _setActorTransform(actor, dX, dY, 0, 0, 0, 0)
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
