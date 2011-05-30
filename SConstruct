
env = Environment()
src = Split('#/src/main.cc')
libs = Split('boost_filesystem boost_regex boost_program_options')
include = Dir('include')
cppflags = "-Wall -Wextra -ansi -pedantic-errors -Wno-deprecated -O2"

Program('cdc-tool', src, LIBS = libs, CPPPATH = include, CPPFLAGS = cppflags)

