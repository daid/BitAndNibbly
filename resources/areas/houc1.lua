sign.onUse(function(player)
    message(sign, "Welcome to the\nHall Of Unfair Challanges")
    message(sign, "The spikes are\nperfectly safe\nand won't hurt\nyou at all.")
    message(sign, "There won't be any\ncheap deaths here.")
    message(sign, "You can trust me.")
    message(sign, "Signed:\n-Daid")
end)

spikes4_trigger.onTouch(function(player) if spikes4.position.x > player.position.x then spikes4.position = spikes4.position + {-0.1, 0} end end)
spikes9_trigger.onTouch(function(player) if spikes9.position.y > player.position.y then spikes9.position = spikes9.position + {0, -0.1} end end)
