local gui = {}

local panelBorder, buttonBorder, fontWidth, fontHeight = _systemInfo()

-- TODO: shouldn't be like this
gui.BUTTON_BORDER = buttonBorder
gui.PANEL_BORDER = panelBorder
gui.FONT_WIDTH = fontWidth
gui.FONT_HEIGHT = fontHeight

--- Creates a representation of a text node for use within this module.
-- @param content is the text to write.
-- @return the representation.
function gui.text(content, font)
    if not font then
        font = _defaultFont
    end
    print(font)
    print("erg")
    return {
        type = "text",
        content = content,
        font = font,
        x = 0,
        y = 0,
        w = 0,
        h = 0
    }
end

--- Creates a representation of a button that can be used within this module.
-- @param content is what to put in the button, which should be either a string
--                or a table representation of a knob.
-- @param id      is the id to give the button so when it has been clicked you
--                will recognise it.
-- @return a representation of a button knob.
function gui.button(content, id)
    local kind = type(content)
    local knob = {
        type = "button",
        id = id or -1,
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

--- Creates a representation of a frame gui object. This is one that just draws
-- a static picture which is quite nice.
-- @param rat     is the name of the rat to draw.
function gui.frame(rat)
    return {
        type = "frame",
        rat = rat,
        x = 0,
        y = 0,
        w = 0,
        h = 0
    }
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

--- Creates a representation of a virtual horizontal split knob which is
-- something that only exists within this code, it simply affects the baking
-- process and is never sent to the engine.
-- @param share is the proportion of the horizontal space the first item gets.
-- @param a     is the first knob to share the inside space.
-- @param b     is the second knob to share the inside space.
-- @return the representation of a hsplit knob.
function gui.hsplit(share, a, b)
    return {
        type = "hsplit",
        share = share,
        a = a,
        b = b,
        x = 0,
        y = 0,
        w = 0,
        h = 0
    }
end

--- Creates a representation of a virtual vertical split knob which is
-- something that only exists within this code, it simply affects the baking
-- process and is never sent to the engine.
-- @param share is the proportion of the vertical space the first item gets.
-- @param a     is the first knob to share the inside space.
-- @param b     is the second knob to share the inside space.
-- @return the representation of a vsplit knob.
function gui.vsplit(share, a, b)
    return {
        type = "vsplit",
        share = share,
        a = a,
        b = b,
        x = 0,
        y = 0,
        w = 0,
        h = 0
    }
end

--- Sets the position of all of the knobs based on their familial
-- relationships.
-- @param knob is the parent knob to start baking at.
-- @param x    is the horizontal position to fit at.
-- @param y    is the vertical position to fit at.
-- @param w    is the width to fit the knobs in.
-- @param h    is the height to fit the knobs in.
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
        y = y + gui.PANEL_BORDER
        w = (w - gui.PANEL_BORDER * 2) / knob.parts
        h = (h - gui.PANEL_BORDER * 2) / rows
        for i, child in ipairs(knob.children) do
            gui.bake(
                child,
                x + ((i - 1) % knob.parts) * w,
                y + (math.ceil(i / knob.parts) - 1) * h,
                w,
                h
            )
        end
    elseif (knob.type == "hsplit") then
        gui.bake(knob.a, x, y, w * knob.share, h)
        gui.bake(knob.b, x + w * knob.share, y, w - w * knob.share, h)
    elseif (knob.type == "vsplit") then
        gui.bake(knob.a, x, y, w, h * knob.share)
        gui.bake(knob.b, x, y + h * knob.share, w, h - h * knob.share)
    end
end

--- Converts a knob in our representation we use in this module into an xml.
-- string, and recursively does it's contents.
-- @param knob is the knob to convert.
-- @return the xml string
function gui.xml(knob)
    if (knob.type == "button") then
        return string.format(
            "<button x=\"%d\" y=\"%d\" w=\"%d\" h=\"%d\" id=\"%d\">%s</button>",
            knob.x,
            knob.y,
            knob.w,
            knob.h,
            knob.id,
            gui.xml(knob.child)
        )
    elseif (knob.type == "panel") then
        local text = string.format(
            "<panel parts=\"%d\" x=\"%d\" y=\"%d\" w=\"%d\" h=\"%d\">",
            knob.parts,
            knob.x,
            knob.y,
            knob.w,
            knob.h
        )
        for i, child in ipairs(knob.children) do
            text = text..gui.xml(child)
        end
        return text.."</panel>"
    elseif (knob.type == "text") then
        return string.format(
            "<text x=\"%d\" y=\"%d\" w=\"%d\" h=\"%d\">%s</text>",
            knob.x,
            knob.y,
            knob.w,
            knob.h,
            knob.content
        )
    elseif (knob.type == "frame") then
        return string.format(
            "<frame x=\"%d\" y=\"%d\" w=\"%d\" h=\"%d\" rat=\"%s\" />",
            knob.x,
            knob.y,
            knob.w,
            knob.h,
            knob.rat
        )
    elseif (knob.type == "hsplit" or knob.type == "vsplit") then
        return gui.xml(knob.a)..gui.xml(knob.b)
    end
    return string.format("<%s />", knob.type)
end

--- Writes some text in a gui box with a heading.
-- @param speaker is the name of the thing speaker / the heading.
-- @param ...     is just a bunch of text that is concatenated with spaces.
function gui.say(speaker, ...)
    local speech = ""
    for i, bit in ipairs({...}) do
        speech = speech..bit.." "
    end
    local knob = gui.panel(
        1,
        gui.vsplit(
            0.20,
            gui.panel(1, gui.text(speaker)),
            gui.hsplit(
                0.9,
                gui.text(speech),
                gui.button("\x01")
            )
        )
    )
    gui.bake(knob, 192, 400, 640, 200)
    _xmlKnob(gui.xml(knob))
    coroutine.yield()
end

--- Writes some text in a gui box with a heading using a font of your choice.
-- @param speaker is the name of the thing speaker / the heading.
-- @param font    is the font to write the text in.
-- @param ...     is just a bunch of text that is concatenated with spaces.
function gui.sayAccent(speaker, font, ...)
    local speech = ""
    for i, bit in ipairs({...}) do
        speech = speech..bit.." "
    end
    local knob = gui.panel(
        1,
        gui.vsplit(
            0.20,
            gui.panel(1, gui.text(speaker)),
            gui.hsplit(
                0.9,
                gui.text(speech, font),
                gui.button("\x01")
            )
        )
    )
    gui.bake(knob, 192, 400, 640, 200)
    _xmlKnob(gui.xml(knob))
    coroutine.yield()
end

--- Writes some text and lets the user choose an option.
-- @param speaker  is the name of the asker / heading
-- @param question is the question to ask.
-- @param ...      is the answers to the question you can give.
-- @return the index of the chosen answer.
function gui.ask(speaker, question, ...)
    local answerButtons = {}
    for i, answer in ipairs({...}) do
        table.insert(answerButtons, gui.button(answer, i - 1))
    end
    local questionPanel = gui.panel(
        3,
        unpack(answerButtons)
    )
    local knob = gui.panel(
        1,
        gui.vsplit(
            0.2,
            gui.panel(1, gui.text(speaker)),
            gui.vsplit(
                0.25,
                gui.text(question),
                questionPanel
            )
        )
    )
    gui.bake(knob, 192, 400, 640, 200)
    _xmlKnob(gui.xml(knob))
    return coroutine.yield()
end

return gui
