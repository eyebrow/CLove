	v = {0,0,1,0,0,0,1}
	e = {1}
	for r = 0, #v do
 	print("1)String test: ") 
	print(love.filesystem.contain(tostring(v[r]),tostring(1)))
	end

	v = "0,0,0,0,0,0,1"
 	e = "12"
 	print("2)String test: ")
 	print(love.filesystem.contain(v,e)) -- e false 

	v = "0,0,0,0,0,0,1"
 	e = "1"
 	print("3)String test: ")
 	print(love.filesystem.contain(v,e)) -- e true

 	--create a file 
 	--love.filesystem.load works like in love2d
 	--love.filesystem.remove works like in love2d
 	--love.filesystem.exists works like in love2d
 	--love.filesystem.append works like in love2d
 	
 	print("Does TEST.txt exist?")
 	print(love.filesystem.exists("TEST.txt"))
 	love.filesystem.write("TEST.txt","Hello World!\n")
 	print("Now it should have been created succesfully")
 	print(love.filesystem.exists("TEST.txt"))
 	love.filesystem.append("TEST.txt", "Hey from append!")
