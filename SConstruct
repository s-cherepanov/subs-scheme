
env = Environment(
	CCFLAGS = '-Wall -Werror -g',
	CPPPATH = '.',
	)

env.Program(
	["test_subs.cpp"]
	+ Glob( "test/*.cpp")
	+ Glob( "lib/*.cpp")
	)

test_cmd = env.Command( ".test.passed", "test_subs",
	"./test_subs && echo 'Passed' > .test.passed" )
env.Alias( "test", test_cmd )

testv_cmd = env.Command( ".testv.passed", "test_subs",
	"valgrind --quiet --leak-check=full ./test_subs && echo 'Passed' > .testv.passed" )
env.Alias( "testv", testv_cmd )

env.Program(
	["subs.cpp"]
	+ Glob( "lib/*.cpp")
	)

