gyp --depth . -Dlibrary=static_library -Dcomponent=static_library -Dlinux_use_tcmalloc=0 -Dchromeos=0 -Dcflag="[ '-g', '-O0', '-fpermissive', '-Wall', '-openmp']" $1
