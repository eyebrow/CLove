
--[[
--
-- Note: Threads in CLove are totally different then in Love
--
--]]

local a = 2
function put_data_test()
    while (a < love.math.random(3600,31500)) do
        a = a + 1
        print(a)
    end
    return 1
end

function love.load()
    -- Return: returns 1 or -1 for succeded or fail
    -- Param 1: Put the data that you want to have it processed (function that has return
    -- type of int or a lua file
    -- (Optional) Param 2: Give this thread a name
    local t = love.thread.newThread(put_data_test(), "test")
   --local thread = love.thread.newThread("testThread.lua","test") 
    
end

function love.update(dt)

end

function love.draw()
    
end
