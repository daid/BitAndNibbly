sign.onUse(function(player)
    message(sign, "Watch out!")
    sign_platform.onUpdate(function()
        for n=1,60 do yield() end
        while true do
            sign_platform.position = sign_platform.position + {0, -0.1}
            if sign_platform.position.y < -1 then sign_platform.destroy() end
            yield()
        end
    end)
    message(sign, "Floor unstable.")
    message(sign, "Room unstable.")
    message(sign, "Signed:\n-Daid")
end)

sign_platform.setCollisionPlatform(1, {0, 0.5})
moving_platform1.setCollisionPlatform(1, {0, 0.5})
moving_platform2.setCollisionPlatform(1, {0, 0.5})
moving_platform3.setCollisionPlatform(1, {0, 0.5})
moving_block.setCollisionSolid({1, 1}, {0, 0})
moving_block2.setCollisionSolid({1, 1}, {0, 0})

switch.onUse(function(player)
    switch.onUse(nil)
    switch.setTile(324)
    moving_platform1.onUpdate(function()
        if moving_platform1.position.y < 1.5 then
            moving_platform1.position = moving_platform1.position + {0, 0.1}
        else
            moving_platform1.onUpdate(nil)
            
            switch.onUse(function(player)
                switch.onUse(nil)
                switch.setTile(323)
                moving_platform2.onUpdate(function()
                    if moving_platform2.position.y < 1.5 then
                        moving_platform2.position = moving_platform2.position + {0, 0.1}
                    else
                        moving_platform2.onUpdate(nil)
                    end
                end)
            end)
        end
    end)
end)

flag_trigger.onTouch(function(player)
    flag_trigger.onTouch(nil)
    moving_platform1.onUpdate(function()
        if moving_platform1.position.y < 2.5 then
            moving_platform1.position = moving_platform1.position + {0, 0.1}
        else
            moving_platform1.onUpdate(nil)
        end
    end)
    moving_platform2.onUpdate(function()
        if moving_platform2.position.y < 3.5 then
            moving_platform2.position = moving_platform2.position + {0, 0.1}
        else
            moving_platform2.onUpdate(nil)
        end
    end)

    center_trigger.onTouch(function(player)
        center_trigger.onTouch(nil)
        
        flag_trigger.onTouch(function(player)
            flag_trigger.onTouch(nil)
            moving_platform1.onUpdate(function()
                if moving_platform1.position.y < 4.5 then
                    moving_platform1.position = moving_platform1.position + {0, 0.1}
                else
                    moving_platform1.onUpdate(nil)
                end
            end)
            
            start_trigger.onTouch(function(player)
                start_trigger.onTouch(nil)
                moving_platform1.onUpdate(function()
                    if moving_platform1.position.x > 0.51 then
                        moving_platform1.position = moving_platform1.position + {-0.1, 0}
                    else
                        moving_platform1.onUpdate(nil)
                    end
                end)
                moving_platform2.onUpdate(function()
                    if moving_platform2.position.x > 0.51 then
                        moving_platform2.position = moving_platform2.position + {-0.1, 0}
                    else
                        moving_platform2.onUpdate(nil)
                    end
                end)
            end)
        end)
    end)
end)

block.setCollisionSolid({1, 1}, {0, 0})
block.count = 0
function updateBlock()
    block.onTouch(nil)
    --Tile: 857: death
    --Tile: 858-863: 1 to 6
    block.count = block.count + 1
    if block.count > 3 then setDeathBlock() return end
    block.setTile(857 + block.count)
    block.onUpdate(function()
        block.onUpdate(nil)
        for n=0,5 do yield() end
        block.onTouch(updateBlock)
    end)
end
function setDeathBlock()
    block.setTile(857)
    block.onUpdate(function()
        block.onUpdate(nil)
        for n=0,5 do yield() end
        block.onTouch(function(player)
            player.die()
        end)
        for n=0,120 do yield() end
        block.setTile(857 + block.count)
        if block.count == 6 then
            block.onTouch(nil)
            moving_platform3.onUpdate(function()
                if moving_platform3.position.y > 5.5 then
                    moving_platform3.position = moving_platform3.position + {0.0, -0.01}
                else
                    moving_platform3.onUpdate(nil)
                end
            end)
            return
        end
        block.onTouch(updateBlock)
    end)
end
block.onTouch(updateBlock)

portal_trigger.onTouch(function(player)
    portal_trigger.onTouch(nil)
    moving_block.onUpdate(function()
        if moving_block.position.x < 5.5 then
            moving_block.position = moving_block.position + {0.01, 0.0}
        else
            moving_block.onUpdate(nil)
        end
    end)
    moving_block2.onUpdate(function()
        if moving_block2.position.x < 0.5 then
            moving_block2.position = moving_block2.position + {0.01, 0.0}
        else
            moving_block2.onUpdate(nil)
        end
    end)
end)