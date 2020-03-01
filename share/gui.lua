local gui = {}

--- Creates a representation of a button that can be used within this module.
-- @param content is what to put in the button, which should be either a string
--                or a table representation of a knob.
-- @return a representation of a button knob.
function gui.button(content)
    local kind = type(content)
    local knob = {
        type = "button"
    }
    if (kind == "string") then
        knob.child = {
            type = "text",
            content = content
        }
    elseif (kind == "table") then
        knob.child = content
    end
    return knob
end

--- Creates a representation of a panel that can be used within this module.
-- @param parts is the number of columns to seperate things into.
-- @param ...   all of these are meant to be knob representations to be made
--              children of this panel.
-- @return a representation of a penelle.
function gui.panel(parts, ...)
    local knob = {
        type = "panel",
        parts = parts,
        children = {}
    }
    for i, child in ipairs({...}) do
        table.insert(knob.children, child)
    end
    return knob
end

--- Converts a knob in our representation we use in this module into an xml.
-- string, and recursively does it's contents.
-- @param knob is the knob to convert.
-- @return the xml string
function gui.xml(knob)
    if (knob.type == "button") then
        return "<button>"..gui.xml(knob.child).."</button>"
    elseif (knob.type == "panel") then
        local xml = "<panel parts=\""..knob.parts.."\">"
        for i, child in ipairs(knob.children) do
            xml = xml..gui.xml(child)
        end
        return xml.."</panel>"
    elseif (knob.type == "text") then
        return "<text>"..knob.content.."</text>"
    end
    return "<shiiiiieet />"
end

--- Turns our nice representation in lua into xml which we can send into spider
-- and then sends it so we get a nice knob displayed.
-- @param knob is knob to display in this module's table representation.
-- @param x    is the distance from the left of the screen to fit it.
-- @param y    is the distance from the top of the screen to fit it.
-- @param w    is the width to give it all.
-- @param h    is the height to give it all.
function gui.display(knob, x, y, w, h)
    _xmlKnob(gui.xml(knob), x, y, w, h)
end

return gui
