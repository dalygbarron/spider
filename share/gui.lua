local gui = {}

gui.BUTTON_BORDER = 8
gui.PANEL_BORDER = 8

--- Creates a representation of a button that can be used within this module.
-- @param content is what to put in the button, which should be either a string
--                or a table representation of a knob.
-- @return a representation of a button knob.
function gui.button(content)
    local kind = type(content)
    local knob = {
        type = "button",
        x = 0,
        y = 0,
        w = 0,
        h = 0
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
        children = {},
        x = 0,
        y = 0,
        w = 0,
        h = 0
    }
    for i, child in ipairs({...}) do
        table.insert(knob.children, child)
    end
    return knob
end

function gui.bake(knob, x, y, w, h)
    knob.x = x
    knob.y = y
    knob.w = w
    knob.h = h
    if (knob.type == "button") then
        gui.bake(
            knob.child,
            x + gui.BUTTON_BORDER,
            y + gui.BUTTON_BORDER,
            w - gui.BUTTON_BORDER * 2,
            h - gui.BUTTON_BORDER * 2
        )
    elseif (knob.type == "panel") then
        local rows = math.ceil(table.getn(knob.children) / knob.parts)
        x = x + gui.PANEL_BORDER
        y = x + gui.PANEL_BORDER
        w = (w - gui.PANEL_BORDER * 2) / knob.parts
        h = (h - gui.PANEL_BORDER * 2) / rows
        for i, child in ipairs(knob.children) do
            gui.bake(
                child,
                x + (i % knob.parts) * w,
                y + math.ceil(i / knob.parts) * h,
                w,
                h
            )
        end
    end
    -- TODO: this return is kinda weird imo
    return knob
end

--- Converts a knob in our representation we use in this module into an xml.
-- string, and recursively does it's contents.
-- @param knob is the knob to convert.
-- @return the xml string
function gui.xml(knob)
    print(knob.type)
    if (knob.type == "button") then
        return string.format(
            "<button x=\"%d\" y=\"%d\" w=\"%d\" h=\"%d\">%s</button",
            gui.xml(knob.child)
        )
    elseif (knob.type == "panel") then
        local text = string.format("<panel parts=\"%d\">", knob.parts)
        for i, child in ipairs(knob.children) do
            text = text..gui.xml(child)
        end
        return text.."</panel>"
    elseif (knob.type == "text") then
        return "<text>"..knob.content.."</text>"
    end
    return "<shiiiiieet />"
end

return gui
