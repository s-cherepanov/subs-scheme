
env = Environment(
	CCFLAGS = '-Wall -Werror -g',
	CPPPATH = '.',
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

test_cmd = env.Command( ".test.passed", ["subs","test_subs"],
	"./test_subs && echo 'Passed' > .test.passed" )
env.Alias( "test", test_cmd )

testslow_cmd = env.Command( ".testslow.passed",
	["subs","test_subs_slow"] + Glob( "testslow/*.scm*" ),
	"./test_subs_slow && echo 'Passed' > .testslow.passed" )
env.Alias( "testslow", testslow_cmd )

testsicp_cmd = env.Command( ".testsicp.passed",
	["subs","test_subs_sicp"] + Glob( "testsicp/*.scmsession" ),
	"./test_subs_sicp && echo 'Passed' > .testsicp.passed" )
env.Alias( "testsicp", testsicp_cmd )


testvalgrind_cmd = env.Command( ".testvalgrind.passed", ["subs","test_subs"],
	"valgrind --quiet --leak-check=full ./test_subs && echo 'Passed' > .testvalgrind.passed" )
env.Alias( "testvalgrind", testvalgrind_cmd )


env.Alias( "testall", ["testvalgrind", "testslow", "testsicp" ] )

