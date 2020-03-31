function playerController(player)
    local input = require("script.input")
    while true do
        local x, y = _getActorPosition(player)
        local vX = 0
        local vY = 0
        if _input[input.LEFT] then vX = -3.5
        elseif _input[input.RIGHT] then vX = 3.5
        end
        if _input[input.UP] then vY = -3.5
        elseif _input[input.DOWN] then vY = 3.5
        end
        _setActorTransform(player, x, y, vX, vY, 0, 0)
        coroutine.yield()
    end
    return battle.FAILURE
end

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

function battle.main(title, actors)
    local player = _addActor(400, 500, "grease", true)
    -- untransition
    _setTitle(title)
    for i = 60, 0, -1 do
        _setTransitionStrength(i / 60)
        coroutine.yield()
    end
    _setTransitionStrength(0)
    _playMusic("music/battle.ogg")
    -- set up actor routines.
    local actorRoutines = {}
    for i, actor in ipairs(actors) do
        local routine = coroutine.create(actor.routine)
        coroutine.resume(routine, actor.actor, player)
        actorRoutines[i] = routine
    end
    local playerRoutine = coroutine.create(playerController)
    coroutine.resume(playerRoutine, player)
    table.insert(actorRoutines, playerRoutine)
    -- iterate the routines until they are done.
    while true do
        coroutine.yield()
        for i, routine in ipairs(actorRoutines) do
            local running, response = coroutine.resume(routine)
            if response == battle.SUCCESS then
                return battle.SUCCESS
            elseif response == battle.FAILURE then
                return battle.FAILURE
            end
        end
    end
end

return battle
