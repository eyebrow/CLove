local a = "vlad"
 	local b = "vlad"
 	print("\n")
 	print("is " .. a .. " the same as " .. b .. "?")
 	print(love.filesystem.compare(a,b,#b))

 	local a = "Murii"
 	local b = "vlad"
 	print("\n")
 	print("is " .. a .. " the same as " .. b .. "?")
 	print(love.filesystem.compare(a,b,#b))

	v = {0,0,1,0,0,0,1}
	e = {1}
 	print("\n")
 	print("1)String test: ") 
	for r = 0, #v do
	print(love.filesystem.contain(tostring(v[r]),tostring(1)))
	end

	v = "0,0,0,0,0,0,1"
 	e = "12"
 	print("\n")
 	print("2)String test: ")
 	print(love.filesystem.contain(v,e)) -- e false 

	v = "0,0,0,0,0,0,1"
 	e = "1"
 	print("\n")
 	print("3)String test: ")
 	print(love.filesystem.contain(v,e)) -- e true

 	--create a file 
 	--love.filesystem.load works like in love2d
 	--love.filesystem.remove works like in love2d
 	--love.filesystem.exists works like in love2d
 	--love.filesystem.append works like in love2d
 	
 	print("\n")
 	print("Does TEST.txt exist?")
 	print(love.filesystem.exists("TEST.txt"))
 	love.filesystem.write("TEST.txt","Hello World!\n")
 	print("Now it should have been created succesfully")
 	print(love.filesystem.exists("TEST.txt"))
 	love.filesystem.append("TEST.txt", "Hey from append!")