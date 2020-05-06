local boat = {}

local xmlConversions = {
    union = function(item, x, y, z)
        local accumulation = ""
        for i, content in ipairs(item.content) do
            accumulation = accumulation..boat.xml(content, x, y, z)
        end
        return accumulation
    end,
    dist = function(item, x, y, z)
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
    offset = function(item, x, y, z)
        x = x + item.oX
        y = y + item.oY
        z = z + item.oZ
        return boat.xml(item.content, x, y, z)
    end,
    choose = function(item, x, y, z)
        local choice = item.choices[math.random(#item.choices)]
        return boat.xml(choice, x, y, z)
    end,
    lindel = function(item, x, y, z)
        return string.format(
            "<l name=\"%s\" x=\"%.3f\" y=\"%.3f\" z=\"%.3f\" />",
            item.name,
            x,
            y,
            z
        )
    end
}

--- Just creates a prototype that holds a bunch of children so you can treat
-- things as one thing.
-- @return the prototype.
function boat.union(...)
    return {
        type = "union",
        content = {...}
    }
end

--- Creates a prototype for a 3D distribution of the given thing and with an
-- offset in space for convenience.
-- @param n       is the number of samples to create.
-- @param dX      is the deviance on the x axis.
-- @param dY      is the deviance on the y axis.
-- @param dZ      is the deviance on the z axis.
-- @param content is the thing to create at each sample.
-- @param oX      is the offset to give samples on the x axis.
-- @param oY      is the offset to give samples on the y axis.
-- @param oZ      is the offset to give samples on the z axis.
-- @return the created prototype. Samples are actually not taken until later.
function boat.dist(n, dX, dY, dZ, content, oX, oY, oZ)
    if oX == nil then oX = 0 end
    if oY == nil then oY = 0 end
    if oZ == nil then oZ = 0 end
    return {
        type = "dist",
        n = n,
        dX = dX,
        dY = dY,
        dZ = dZ,
        content = content,
        oX = oX,
        oY = oY,
        oZ = oZ
    }
end

--- Gives it's content an offset
-- @param oX is the offset on the x axis.
-- @param oY is the offset on the y axis.
-- @param oZ is the offset on the z axis.
-- @return the prototype.
function boat.offset(content, oX, oY, oZ)
    return {
        type = "offset",
        content = content,
        oX = oX,
        oY = oY,
        oZ = oZ
    }
end

--- You put in a bunch of choices and it just uses one of them
-- @return whatever it chose
function boat.choose(...)
    return {
        type = "choose",
        choices = {...}
    }
end

--- Gives a prototype for placing a single lindel in the world
-- @param name is the name of the entity it is an instance of.
-- @return the prototype.
function boat.lindel(name)
    return {
        type = "lindel",
        name = name
    }
end

--- Converts a lindel prototype into an xml representation that the spider
-- engine can ingest as part of a world.
-- @param item is the prototype to convert.
-- @param x is the x offset to add.
-- @param y is the y offset to add.
-- @param z is the z offset to add.
-- @return the xml form in a string.
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

--- Creates an xml representation of an entire world which the spider engine
-- can ingest.
-- @param ground is the name of the ground texture to use.
-- @param horizon is the colour to give the horizon as a hex string.
-- @param skyBottom is nothing tbh.
-- @param skyTop    is the colour to give the top of the sky.
-- @param waves     is a multiplier to control how much / at all the ground
--                  should ripple around like it has waves. So if you want no
--                  waves, pass 0, if you want normal waves pass 1.
-- @param lindels   all of the lindels in prototype form that will be put in
--                  the level.
-- @return the full xml as a string.
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
