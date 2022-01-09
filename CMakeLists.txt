set( CMAKE_EXPORT_COMPILE_COMMANDS ON)

cmake_minimum_required(VERSION 3.5)

project(sssh VERSION 1.0.0)

add_executable(sssh)

target_sources(sssh
	PUBLIC
	main.c
	)
