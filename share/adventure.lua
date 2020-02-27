local adventure = {}

function adventure.longitudeDistance(a, b)
    a = a % (math.pi * 2)
    b = b % (math.pi * 2)
    if math.abs(a - b) > math.pi then
        return math.abs(((a + math.pi) % math.pi) - b)
    else 
        return math.abs(a - b)
    end
end

function adventure.look(longitude, latitude, time)
    local x, y = _getCamera()
    local dY = latitude - y
    local dX = adventure.longitudeDistance(longitude, x)
    local timer = 0
    while timer <= time do
        local delta = coroutine.yield()
        timer = timer + delta
        _setCamera(x + dX * timer / time, y + dY * timer / time)
    end
end

return adventure
