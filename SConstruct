
env = Environment(
	CCFLAGS = '-Wall -Werror',
	CPPPATH = '.',
	TARFLAGS = '-c -z',
	)

env.Program(
	["test_subs.cpp"]
	+ Glob( "test/*.cpp")
	+ Glob( "lib/*.cpp")
	)

env.Program(
	["subs.cpp"]
	+ Glob( "lib/*.cpp")
	)

