local boat = {}

local xmlConversions = {
    union = function(item, x, y, z)
        local accumulation = ""
        for i, content in ipairs(item.content) do
            accumulation = accumulation..boat.xml(content, x, y, z)
        end
        return accumulation
    end,
    distribution = function(item, x, y, z)
        local accumulation = ""
        for i = 1, item.n, 1 do
            -- TODO: proper distribution
            local nX = x + math.random() * item.dX - item.dX / 2 + item.oX
            local nY = y + math.random() * item.dY - item.dY / 2 + item.oY
            local nZ = z + math.random() * item.dZ - item.dZ / 2 + item.oZ
            accumulation = accumulation..boat.xml(item.content, nX, nY, nZ)
        end
        return accumulation
    end,
    lindel = function(item, x, y, z)
        return string.format(
            "<lindel name=\"%s\" x=\"%f\" y=\"%f\" z=\"%f\" />",
            item.name,
            x,
            y,
            z
        )
    end
}

function boat.union(...)
    return {
        type = "union",
        content = {...}
    }
end

function boat.distribution(n, dX, dY, dZ, content, oX, oY, oZ)
    if oX == nil then oX = 0 end
    if oY == nil then oY = 0 end
    if oZ == nil then oZ = 0 end
    return {
        type = "distribution",
        n = n,
        dX = dX,
        dY = dY,
        dZ = dZ,
        oX = oX,
        oY = oY,
        oZ = oZ,
        content = content
    }
end

function boat.lindel(name)
    return {
        type = "lindel",
        name = name
    }
end

function boat.xml(item, x, y, z)
    if x == nil then x = 0 end
    if y == nil then y = 0 end
    if z == nil then z = 0 end
    if xmlConversions[item.type] then
        return xmlConversions[item.type](item, x, y, z)
    else
        _logError(string.format("no boating type '%s'", item.type))
        return ""
    end
end

function boat.worldXml(ground, horizon, skyBottom, skyTop, waves, lindels)
    return string.format(
        "<world ground=\"%s\" horizon=\"%s\" bottomSky=\"%s\" waves=\"%f\" "..
            "topSky=\"%s\">%s</world>",
        ground,
        horizon,
        skyBottom,
        waves,
        skyTop,
        boat.xml(lindels)
    )
end

return boat
