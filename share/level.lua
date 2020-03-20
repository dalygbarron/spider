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
                gui.panel(1, gui.text("Inventory")),
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
    local knob = gui.panel(
        1,
        gui.vsplit(
            0.1,
            gui.panel(1, gui.text("Paused")),
            gui.panel(
                1,
                gui.button("Resume", 1),
                gui.button("Save", 2),
                gui.button("Save and Quit", 3)
            )
        )
    )
    gui.bake(knob, 384, 100, 256, 400)
    _xmlKnob(gui.xml(knob))
    local choice = coroutine.yield()
    if choice == 1 then
        -- nothing lol
    elseif choice == 2 then
        _saveGame()
        gui.say("saveer", "saved")
    else
        gui.say("saveer", "saving and quitting")
        _saveGame()
        _exit(3)
    end
end

function _itemMessage(name)
    local gui = require("script.gui")
    gui.say("Hell yeah", "You got "..name..".")
end
