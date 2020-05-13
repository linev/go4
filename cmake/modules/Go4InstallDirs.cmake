
if(NOT DEFINED CMAKE_INSTALL_BINDIR)
  set(CMAKE_INSTALL_BINDIR "bin" CACHE PATH "user executables (bin)")
endif()

if(NOT DEFINED CMAKE_INSTALL_LIBDIR)
  if(gnuinstall)
    set(CMAKE_INSTALL_LIBDIR "lib/root" CACHE PATH "object code libraries (lib/root)")
  else()
    set(CMAKE_INSTALL_LIBDIR "lib" CACHE PATH "object code libraries (lib)")
  endif()
endif()

if(NOT DEFINED CMAKE_INSTALL_INCLUDEDIR)
  if(gnuinstall)
    set(CMAKE_INSTALL_INCLUDEDIR "include/root" CACHE PATH "C header files (include/root)")
  else()
    set(CMAKE_INSTALL_INCLUDEDIR "include" CACHE PATH "C header files (include)")
  endif()
endif()

if(NOT DEFINED CMAKE_INSTALL_SYSCONFDIR)
  if(gnuinstall)
    set(CMAKE_INSTALL_SYSCONFDIR "etc/root" CACHE PATH "read-only single-machine data (etc/root)")
  else()
    set(CMAKE_INSTALL_SYSCONFDIR "etc" CACHE PATH "read-only single-machine data (etc)")
  endif()
endif()