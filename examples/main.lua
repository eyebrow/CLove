viewer = require("model_viewer")

function love.load(arg)
    WINDOW_W = love.graphics.getWidth()
    WINDOW_H = love.graphics.getHeight()

    love.graphics.setBackgroundColor(30, 30, 30, 255)

    font = love.graphics.newFont(14)
    love.graphics.setFont(font)
    
    viewer:new("example.png")

end

function love.textinput(t)

end

function love.wheelmoved(y)
    viewer:wheelmoved(y)
end

function love.keypressed(key)
    viewer:keypressed(key)
end

function love.update(dt)
    viewer:update(dt)
    if love.keyboard.isDown("esc") then
        love.event.quit()
    end
end

function love.draw()
    love.graphics.print("FPS:"..love.timer.getFPS(),25,15)
    --love.graphics.print("It's only a matter of perspective", WINDOW_W/2 - 100, WINDOW_H / 2 - 150)
    viewer:drawModel(240,240)
end

