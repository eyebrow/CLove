![Alt text](CLoveLogo.png?raw=true "CLove")

CLove
=====
CLove is a simple and easy to learn and use game framework tested on
Mac/Linux/PC, made in C with OpenGLES 2 which uses Lua as scripting langauge.

How to build
============
On Windows: 
- Download and install mingw and let the setup configure the path for you.
After that check if you got gcc & g++ installed.
- Download and install CMake and let the setup configure the path for you.
- Download DX SDK 2010 (required by SDL2): http://www.microsoft.com/en-us/download/details.aspx?id=6812
- Download and install QT Community the Open Source Edition but only install the core of it since
you wont need all the stuff that QT comes with
- When building CLove be sure that QT uses your brand new installation of mingw
- Restart the PC

On Linux & OS X:
- Download and install cmake,gcc,g++(optional git)(on os x you can install these using brew)
- Download and install QT Community 

Features
========
- Lua scripting language
- Easy to learn and use api 
- Cross Platform 
- Possibility of hiding the code inside .zip files and load the data from there
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

Contributing
========
If you want to contribute to the project, feel free to work on what you please. I want to do a lot of the other stuff myself too.
Please try to replicate the code style of the existing code, I might refuse to pull your changes otherwise.

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


