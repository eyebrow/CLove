
ModelViewer =
{
    model = false,
    src,
    file,
    image,
    quads,
    quadWidth=16,
    quadHeight=16,
    updateTimer=0,
    widthInput  = {text="16"},
    heightInput = {text="16"},
    spaceInput  = {text="1"},
    --View
    rotation = 0,
    layer_spacing = 1,
    autoRotate = true,
    rotateDir = 1,
    flip = false,
    rotate_speed = 30,
    zoom = 1,
    nearest=true,

}

function ModelViewer:new(file)
    self.file = file
    self.model = true
    self.zoom = 1

    self:loadImage()

    self:createQuads(true)

end

function ModelViewer:loadImage()
    self.image = love.graphics.newImage(self.file)
    self.image:setFilter(self.nearest and "nearest" or "linear", self.nearest and "nearest" or "linear")

    self.quadWidth=self.image:getHeight()
    self.quadHeight=self.quadWidth

    self.heightInput.text   = self.quadWidth..""
    self.widthInput.text    = self.quadHeight..""

    return true
end

function ModelViewer:wheelmoved(y)
    if y>0 then
        self.zoom = math.min(5, self.zoom*1.25)
    else
        self.zoom = math.max(.25, self.zoom*0.75)
    end
end

function ModelViewer:keypressed(key)
    if key=="f" then
        self.flip = not self.flip
    end
    if key=="r" then
        self.autoRotate = not self.autoRotate
    end
end

function ModelViewer:update(dt)

    if love.keyboard.isDown("a") or love.keyboard.isDown("left") then
        self.rotation = self.rotation - math.max(100, self.rotate_speed*1.5)*dt
        self.rotateDir = -1
    elseif love.keyboard.isDown("d") or love.keyboard.isDown("right") then
        self.rotation = self.rotation + math.max(100, self.rotate_speed*1.5)*dt
        self.rotateDir = 1
    elseif self.autoRotate then
       self.rotation = self.rotation + self.rotate_speed*dt*self.rotateDir
    end
    
    if self.rotation > 360 then
        self.rotation = 360
        self.rotation = -360
    elseif self.rotation <= -360 then
        self.rotation = -360
        self.rotation = 360
    end
    
    if tonumber(self.spaceInput.text)==nil then
        self.layer_spacing = 1
    else
        self.layer_spacing = tonumber(self.spaceInput.text) or 1
    end 
end

function ModelViewer:createQuads(first)
    self.quads = {}

    local steps = self.image:getWidth()/self.quadWidth
    for i=1,steps do
        self.quads[i] = love.graphics.newQuad((i-1)*self.quadWidth, 0, self.quadWidth, self.quadHeight, self.image:getWidth(), self.image:getHeight())
    end

    if first then
        self.layer_spacing = math.floor(math.max(((self.quadWidth+self.quadHeight)/3)/#self.quads, 1)+.5)
        self.spaceInput.text = self.layer_spacing..""
    end

end

function ModelViewer:drawModel(x,y)
    local scale = 100/math.max(self.quadWidth, #self.quads) * self.zoom
    local sy = #self.quads/2 * self.layer_spacing*scale

    for i=1,#self.quads do
        local index = self.flip and #self.quads-i+1 or i
        for j=1,math.max(1,self.layer_spacing)*scale do
            love.graphics.draw(self.image, self.quads[index], x, y+sy - i*scale*self.layer_spacing - j, math.rad(self.rotation), scale,scale,self.quadWidth/2, self.quadHeight/2)
            if j>1 then break end
        end
    end

    scale = WINDOW_W/self.image:getWidth()
    love.graphics.draw(self.image, 0, WINDOW_H-self.image:getHeight()*scale, 0, scale, scale)
end

return ModelViewer
