if ! [[ -d build ]]
then
	mkdir build
fi

if ! [[ -d executable ]]
then
	mkdir executable
fi

cd build
cmake ..

if ! [[ -e compile_commands.json ]]
then
	# Usually you need to cmake twice to get compile_commands.json
	cmake .. 
fi

if make
then
	mv compile_commands.json ../
	mv sssh ../executable
	echo "Compilation done."
fi
