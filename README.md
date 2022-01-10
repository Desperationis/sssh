# sssh
Super Simple Shell made with good old C for Unix systems. Features the most bare-bones functionality you can imagine including:

* Running commands
* `cd`, `help`, `exit` as built-ins

And that's it! It also displays the current working directory in the prompt, but other than that this is all you get.

## Compiling
All you need is gcc (or any other c compiler) and CMake greater than 3.5. Simply run `compile.bash` and `sssh` should be available in the `executable` folder. 

## Preview
```
  /home/diego/Desktop/sssh$ ls
  build  clean.bash  CMakeLists.txt  compile.bash  compile_commands.json	executable  LICENSE  src
  /home/diego/Desktop/sssh$ pwd
  /home/diego/Desktop/sssh
  /home/diego/Desktop/sssh$ cmake --version
  cmake version 3.16.3

  CMake suite maintained and supported by Kitware (kitware.com/cmake).
  /home/diego/Desktop/sssh$ 
```
