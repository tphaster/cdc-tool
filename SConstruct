
env = Environment()
src = Split('#/src/main.cc')
libs = Split('boost_filesystem boost_regex boost_program_options')
include = Dir('include')

env.Append(CPPFLAGS = ['-Wall', '-Wextra', '-ansi', '-pedantic-errors'])

Program('cdc-tool', src, LIBS = libs, CPPPATH = include)

