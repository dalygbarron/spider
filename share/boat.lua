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
            local nX = x + math.random() * item.deviance - item.deviance / 2
            local nY = y + math.random() * item.deviance - item.deviance / 2
            accumulation = accumulation..boat.xml(item.content, nX, nY)
        end
        return accumulation
    end,
    lindel = function(item, x, y)
        return string.format(
            "<lindel name=\"%s\" x=\"%f\" y=\"%f\" />",
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

function boat.lindel(name)
    return {
        type = "lindel",
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
