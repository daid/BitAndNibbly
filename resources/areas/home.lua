
addParticleEmitter(mailbox).frequency(3.0).area(0.8, 0.6).color(0.6, 0.6, 0.9, 1.0)
mailbox.onUse(function(player)
    player.die()
end)