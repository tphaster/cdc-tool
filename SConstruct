env = Environment()
src = Split('#/src/main.cc #/src/FilesDep.cc #/src/CircularStrategy.cc')
cc = env['CC']
if cc == 'gcc': 
	#LINUX
	libs = Split('boost_filesystem boost_regex boost_program_options')
	include = Dir('include')
	cppflags = "-Wall -Wextra -ansi -pedantic-errors -Wno-deprecated -O2"
	Program('cdc-tool', src, LIBS = libs, CPPPATH = include, CPPFLAGS = cppflags)
elif cc == 'cl':
	#WINDOWS
	libs = Split('libboost_system-vc100-mt-s-1_46_1 libboost_filesystem-vc100-mt-s-1_46_1 libboost_regex-vc100-mt-s-1_46_1 libboost_program_options-vc100-mt-s-1_46_1')
	libpaths = Dir('/Program Files/boost/boost_1_46_1/lib')
	include = [Dir('include'), Dir('/Program Files/boost/boost_1_46_1')]
	Program('cdc-tool', src, LIBPATH = libpaths, LIBS = libs, CPPPATH = include, CPPFLAGS = '/EHsc')

