local battle = {}

battle.FAILURE = 0
battle.SUCCESS = 1

function battle.start(file)
    _battle(file)
    return coroutine.yield()
end

function battle.wait(time)
    while time > 0 do
        time = time - coroutine.yield()
    end
end

return battle
