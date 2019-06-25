sign.onUse(function(player)
    message(sign, "Room 2: I hope.")
    message(sign, "Signed:\n-Daid")
end)

spikes1_trigger.onTouch(function(player) if spikes1.position.y > player.position.y then spikes1.position = spikes1.position + {0, -0.05} end end)
spikes2_trigger.onTouch(function(player) if spikes2.position.y < player.position.y then spikes2.position = spikes2.position + {0, 0.04} end end)
spikes7_trigger.onTouch(function(player)
    spikes7.position = spikes7.position + {0.003, 0}
    spikes8.position = spikes8.position + {0.003, 0}
    spikes9.position = spikes9.position + {0.003, 0}
    spikes10.position = spikes10.position + {0.003, 0}
end)
