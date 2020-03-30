local battle = {}

battle.FAILURE = 0
battle.SUCCESS = 1

function battle.wait(n)
    for i = 1, n, 1 do
        coroutine.yield()
    end
end

function battle.distance(aX, aY, bX, bY)
    local x = math.abs(aX - bX)
    local y = math.abs(aY - bY)
    return math.sqrt(x * x + y * y)
end

function battle.angleTo(aX, aY, bX, bY)
    return math.atan2(bY - aY, bX - aX)
end

function battle.walk(actor, speed, dX, dY)
    local x, y = _getActorPosition(actor)
    if x == -1 or y == -1 then
        return
    end
    local distance = battle.distance(x, y, dX, dY)
    local angle = battle.angleTo(x, y, dX, dY)
    _setActorTransform(
        actor,
        x,
        y,
        math.cos(angle) * speed,
        math.sin(angle) * speed,
        0,
        0
    )
    battle.wait(distance / speed)
    _setActorTransform(actor, dX, dY, 0, 0, 0, 0)
end

function battle.start(file)
    _stopMusic()
    _playSound("sound/transition.wav")
    for i = 1, 60, 1 do
        _setTransitionStrength(i / 60)
        coroutine.yield()
    end
    _battle(file)
    return coroutine.yield()
end

function battle.main(actors)
    -- untransition
    for i = 60, 0, -1 do
        _setTransitionStrength(i / 60)
        coroutine.yield()
    end
    _setTransitionStrength(0)
    _playMusic("music/battle.ogg")
    -- main loop
    local actorRoutines = {}
    for i, actor in ipairs(actors) do
        local routine = coroutine.create(actor.routine)
        coroutine.resume(routine, actor.actor)
        actorRoutines[i] = routine
    end
    while true do
        local delta = coroutine.yield()
        for i, routine in ipairs(actorRoutines) do
            local running, response = coroutine.resume(routine, delta)
            if response == battle.SUCCESS then
                return battle.SUCCESS
            elseif response == battle.FAILURE then
                return battle.FAILURE
            end
        end
    end
end

return battle
