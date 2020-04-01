local input = require("script.input")

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
    local player = _addActor(512, 500, "grease", true)
    _setPlayer(player)
    -- untransition
    _setTitle(title)
    for i = 60, 0, -1 do
        _setTransitionStrength(i / 60)
        coroutine.yield()
    end
    _setTransitionStrength(0)
    _playMusic("music/theme.ogg")
    -- set up actor routines.
    local actorRoutines = {}
    for i, actor in ipairs(actors) do
        local routine = coroutine.create(actor.routine)
        coroutine.resume(routine, actor.actor, player)
        actorRoutines[i] = routine
    end
    local playerRoutine = coroutine.create(playerController)
    local ok, response = coroutine.resume(playerRoutine, player)
    if not ok then error(response) end
    table.insert(actorRoutines, playerRoutine)
    -- iterate the routines until they are done.
    while true do
        coroutine.yield()
        for i, routine in ipairs(actorRoutines) do
            local ok, response = coroutine.resume(routine)
            if response == battle.SUCCESS then
                return battle.SUCCESS
            elseif response == battle.FAILURE then
                return battle.FAILURE
            elseif not ok then
                error(response)
            end
        end
    end
end

function playerShoot(player, interval, bullet)
    while true do
        if _input[input.SHOOT] then
            local spread = _input[input.STRAFE] and 0.5 or 1
            local angle = 0 - math.pi / 2 + math.random() * spread - spread / 2
            local x, y = _getActorPosition(player)
            _addBullet(
                player,
                bullet,
                x,
                y,
                math.cos(angle) * 9,
                math.sin(angle) * 9,
                0,
                0
            )
        end
        battle.wait(interval)
    end
end

function playerController(player)
    local shoot = coroutine.create(playerShoot)
    coroutine.resume(shoot, player, 2, 4)
    _setHp(player, 3)
    local currentHealth = 3
    while currentHealth > 0 do
        local x, y = _getActorPosition(player)
        local vX = 0
        local vY = 0
        local speed = _input[input.STRAFE] and 2 or 3.5
        if _input[input.LEFT] then vX = -speed
        elseif _input[input.RIGHT] then vX = speed
        end
        if _input[input.UP] then vY = -speed
        elseif _input[input.DOWN] then vY = speed
        end
        _setActorTransform(player, x, y, vX, vY, 0, 0)
        coroutine.resume(shoot)
        coroutine.yield()
        local newHealth = _getHp(player)
        if newHealth ~= currentHealth then
            _playSound("sound/ow.wav")
            local lifeText = ""
            for i = 1, newHealth, 1 do
                lifeText = lifeText.."\03"
            end
            currentHealth = newHealth
        end
    end
    return battle.FAILURE
end

return battle
