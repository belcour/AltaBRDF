import os, sys, glob


try:
  mitsubaPath = os.environ['MITSUBA_DIR']
except:
  print "You must set MITSUBA_DIR in your environment."
  print "Source setpath.sh from Mitsuba directory."
  sys.exit(1)

try:
  altaPath = os.environ['ALTA']
except:
  print "You must set ALTA in your environment."
  print "Source setpath.sh from ALTA directory."
  sys.exit(1)

# Mitsuba configuration part
#
env = Environment()

env.AppendUnique(CPPDEFINES = 'SINGLE_PRECISION')
env.AppendUnique(CPPDEFINES = 'SPECTRUM_SAMPLES=3')

if sys.platform == 'posix' or sys.platform == 'linux2':
	env.AppendUnique(CCFLAGS = ['-g'])
	env.AppendUnique(CPPPATH = [os.path.join(mitsubaPath, 'include')])
	env.AppendUnique(CPPPATH = [os.path.join(altaPath, 'sources'), os.path.join(altaPath, 'external/build/include')])

	if os.path.exists('/usr/include/eigen3'):
		env.AppendUnique(CPPPATH = ['/usr/include/eigen3'])

	libpath = os.path.join(mitsubaPath, 'dist')
	env.AppendUnique(LIBPATH = [libpath])
	env.AppendUnique(LIBPATH = [os.path.join(altaPath, 'sources/build')])
	libs = [f for f in os.listdir(libpath) if f.endswith('.so')]
	env.AppendUnique(LIBS = [libs, 'core'])
	env.AppendUnique(RPATH = libpath)
	#env.AppendUnique(LINKFLAGS = '-headerpad_max_install_names')

	env['SHLIBPREFIX']=''
	build = env.SharedLibrary('alta_brdf', ['brdf.cpp'])
	install_path = os.path.join(mitsubaPath, 'dist/plugins')
	env.Install(install_path, build)
	env.Alias('install', install_path)

elif sys.platform == 'darwin':
	env.AppendUnique(CCFLAGS = ['-arch', 'x86_64', '-mmacosx-version-min=10.9', '-stdlib=libstdc++', '-g'])
	env.AppendUnique(CPPPATH = [os.path.join(mitsubaPath, 'dependencies/include')])
	env.AppendUnique(CPPPATH = [os.path.join(mitsubaPath, 'Mitsuba.app/Headers/include')])
	env.AppendUnique(CPPPATH = [os.path.join(altaPath, 'sources'), os.path.join(altaPath, 'external/build/include')])

	libpath = os.path.join(mitsubaPath, 'Mitsuba.app/Contents/Frameworks/')
	env.AppendUnique(LIBPATH = [libpath])
	env.AppendUnique(LIBPATH = [os.path.join(altaPath, 'sources/build')])
	libs = [f for f in os.listdir(libpath) if f.endswith('.dylib')]
	libs.remove('libboost_python26.dylib')
	libs.remove('libboost_python33.dylib')
	libs.remove('libboost_python34.dylib')
	env.AppendUnique(LIBS = [libs, 'core'])
	env.AppendUnique(RPATH = libpath)
	env.AppendUnique(LINKFLAGS = ['-arch', 'x86_64', '-mmacosx-version-min=10.9', '-headerpad_max_install_names', '-stdlib=libstdc++'])

	build = env.SharedLibrary('alta_brdf', ['brdf.cpp'])

	#cmd   = 'install_name_tool -add_rpath ' + libpath + ' libMitsuba-Darwin-x86_64.dylib'
	#patch = env.Command(target = "patchbuild", source = "libMitsuba-Darwin-x86_64.dylib", action = cmd)

	Depends(patch, build)
	install_path = os.path.join(mitsubaPath, 'Mitsuba.app/plugins')
	env.Install(install_path, build)
	env.Alias('install', install_path)

else:

	print "<<ERROR>> There is no compilation toolchain for your system."
	sys.exit(1)

#end
