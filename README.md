![Alt text](CLoveLogo.png?raw=true "CLove")

CLove
=====
CLove is a simple and easy to learn and use game framework tested on
Mac/Linux/PC, made in C with OpenGLES 2 which uses Lua as scripting language.

How to build
============
Download or clone CLove.
- On Linux based systems : download cmake and make (sudo apt-get install cmake && make). Call "cmake ../ && make" from a new folder called "build".
- On Os X systems : download cmake and make (sudo brew install cmake && make). Call "cmake ../ && make" from a new folder called "build".
- On Windows : download MinGW,add gcc and g++ to system path.After that download QT Community and build CLove using it.


Features
========
- Lua scripting language
- Easy to learn and use api
- 3D support - WIP (everything has been put on hold till this feature is usable)
- Cross Platform
- Powerful Batch system
- Image loader & drawing
- Image creation from scratch or from a template & save(png,bmp,tga)
- Sound loader & playing (Vorbis and Wav)
- Primitives drawing
- Filesystem functions
- OpenGL ES 2.0
- Font loading & drawing
- Keyboard & Mouse functions
- Physics system thanks to ChipMunk

Examples (see examples folder)
--------
```lua
local image = love.graphics.newImage("image.png")

function love.draw()
	--draw the loaded image named "image" at the postion X:200, Y:200
	love.graphics.draw(image, 200, 200)
	--draw a circle at X:100 Y:100 with the radius of 32 and number of
	--segments(points) of 24
	love.graphics.circle("fill", 100, 100, 32, 24)
end
```

Images of running CLove demos
-----------------------------
![Image 1:](data/1.png?raw=true "Web")
![Image 2:](data/2.png?raw=true "Linux")
![Image 2:](data/3.png?raw=true "Os X")


