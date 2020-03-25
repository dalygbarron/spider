local battle = {}

battle.FAILURE = 0
battle.SUCCESS = 1

function battle.start(file)
    _battle(file)
    return coroutine.yield()
end

return battle
