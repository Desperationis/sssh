option=$1

cmakeBuild() {
	if [[ $option == *-d* ]]
	then
		echo "Compiling in debug.."
		cmake -DCMAKE_BUILD_TYPE=Debug ..
	else
		echo "Compiling in release.."
		cmake -DCMAKE_BUILD_TYPE=Release ..
	fi
}



if ! [[ -d build ]]
then
	mkdir build
fi

if ! [[ -d executable ]]
then
	mkdir executable
fi

cd build
cmakeBuild

if ! [[ -e compile_commands.json ]]
then
	# Usually you need to cmake twice to get compile_commands.json
	cmakeBuild

fi

if make
then
	mv compile_commands.json ../
	mv sssh ../executable
	echo "Compilation done."
fi
