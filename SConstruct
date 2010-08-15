
env = Environment(
	CCFLAGS = '-Wall -Werror -g',
	CPPPATH = '.',
	)

env.Program(
	["test_subs.cpp"]
	+ Glob( "test/*.cpp")
	+ Glob( "lib/*.cpp")
	)

test_cmd = env.Command( ".test.passed", ["subs","test_subs"],
	"./test_subs && echo 'Passed' > .test.passed" )
env.Alias( "test", test_cmd )


testslow_cmd = env.Command( ".testslow.passed", ["subs","test_subs_slow"],
	"./test_subs_slow && echo 'Passed' > .testslow.passed" )
env.Alias( "testslow", testslow_cmd )


testv_cmd = env.Command( ".testv.passed", ["subs","test_subs"],
	"valgrind --quiet --leak-check=full ./test_subs && echo 'Passed' > .testv.passed" )
env.Alias( "testv", testv_cmd )

env.Program(
	["subs.cpp"]
	+ Glob( "lib/*.cpp")
	)

