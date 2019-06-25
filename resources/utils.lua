
function message(target, msg)
    local bubble = showSpeechBubble(target, msg)
    yield()
    while not player.anyKeyPressed() do yield() end
    bubble.destroy()
end

n = 1 while _ENV["spikes"..n] ~= nil do
    _ENV["spikes"..n].setCollision({0.7, 0.5}, {0, -0.25}).onTouch(function(player) player.die() end)
    n = n + 1
end
