local boat = {}

local xmlConversions = {
    union = function(item, x, y)
        local accumulation = ""
        for i, content in ipairs(item.content) do
            accumulation = accumulation..boat.xml(content, x, y)
        end
        return accumulation
    end,
    distribution = function(item, x, y)
        local accumulation = ""
        for i = 1, item.n, 1 do
            -- TODO: proper distribution
            local nX = x + math.random() * item.deviance
            local nY = y + math.random() * item.deviance
            accumulation = accumulation..boat.xml(item.content, nX, nY)
        end
        return accumulation
    end,
    mob = function(item, x, y)
        return string.format(
            "<mob name=\"%s\" x=\"%f\" y=\"%f\" />",
            item.name,
            x,
            y
        )
    end
}

function boat.union(...)
    return {
        type = "union",
        content = {...}
    }
end

function boat.distribution(n, deviance, content)
    return {
        type = "distribution",
        n = n,
        deviance = deviance,
        content = content
    }
end

function boat.mob(name)
    return {
        type = "mob",
        name = name
    }
end

function boat.xml(item, x, y)
    if x == nil then x = 0 end
    if y == nil then y = 0 end
    if xmlConversions[item.type] then
        return xmlConversions[item.type](item, x, y)
    else
        _logError(string.format("no boating type '%s'", item.type))
    end
end

function boat.worldXml(sky, ground, mobs)
    return string.format(
        "<world sky=\"%s\" ground=\"%s\"><mobs>%s</mobs></world>",
        sky,
        ground,
        boat.xml(mobs)
    )
end

return boat
