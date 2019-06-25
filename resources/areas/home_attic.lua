if getGlobal("GOT_BLUE_DIAMOND") == 1 then
    diamond.destroy()
else
    addParticleEmitter(diamond).frequency(3.0).area(0.5, 0.5).color(0.235, 0.675, 0.843, 0.8)

    diamond.onTouch(function(player)
        message(player, "Picked up diamond.")
        message(player, "And some point there\nwill be a cool\nanimation now")
        diamond.destroy()
        setGlobal("GOT_BLUE_DIAMOND", 1)
    end)
end

bookcase.onUse(function(player)
    message(player, "A long time ago in\na galaxy far,\nfar away....")
    message(player, "Well, will read the\nrest some other day.")
end)
