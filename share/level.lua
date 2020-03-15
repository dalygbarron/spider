dofile("script/items.lua")

function _gameMenu()
    local gui = require("script.gui")
    local inventory = _inventory()
    local itemButtons = {}
    for k, v in ipairs(_itemInfo) do
        local count = inventory[v.name]
        if (count) then
            table.insert(itemButtons, gui.button(gui.frame(v.rat), k))
        end
    end
    local knob = gui.panel(
        1,
        gui.vsplit(
            0.1,
            gui.hsplit(
                0.9,
                gui.text("Inventory"),
                gui.button("X", -1)
            ),
            gui.panel(
                7,
                unpack(itemButtons)
            )
        )
    )
    gui.bake(knob, 256, 100, 512, 400)
    _xmlKnob(gui.xml(knob))
    local choice = coroutine.yield()
    if _itemInfo[choice] then
        local name = _itemInfo[choice].name
        if _G[name] then
            _G[name]()
        else 
            _useItem(name)
        end
    end

end

function _programMenu()
    local gui = require("script.gui")
    local answer = gui.ask(
        "Paused",
        "Menu",
        "Return",
        "Save",
        "Save and Quit"
    )
    if answer == 0 then
        gui.say("ferf", "returning")
    elseif answer == 1 then
        gui.say("saveer", "saving")
    else
        gui.say("saveer", "saving and quitting")
        _exit(3)
    end
end

function _itemMessage(name)
    local gui = require("script.gui")
    gui.say("Hell yeah", "You got "..name..".")
end
