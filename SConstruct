import platform, os

env = Environment()
src = Split('#/src/main.cc #/src/FilesDep.cc #/src/CircularStrategy.cc')

if (platform.system() == "Linux"):
    env.Append(CPPPATH = 'include')
    env.Append(CPPFLAGS = '-Wall -Wextra -ansi -pedantic-errors -Wno-deprecated -O2')
    env.Append(LIBS = Split('boost_filesystem boost_regex boost_program_options'))
    env.Program('cdc-tool', src)
elif (platform.system() == "Windows"):
    env.Append(CPPPATH = [Dir('include'), Dir('/Program Files/boost/boost_1_46_1')])
    env.Append(CPPFLAGS = '/EHsc')
    env.Append(LIBPATH = Dir('/Program Files/boost/boost_1_46_1/lib'))
    env.Program('cdc-tool', src)

