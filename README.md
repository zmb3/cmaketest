Relocatable CMake Libraries
===========================

How do you properly create a library that depends on another library?

In this case, the library is an example _libfoo_ library, and it depends on libpcap,
which is not a CMake project.

This project started out as an example to illustrate the problem.  After the wonderful 
folks on the CMake mailing list helped me out, I figured I'd update the example to 
show the solution.

If you try toproject compiles and links to libpcap correctly, but is not relocatable.

## To reproduce

1. Clone this repository and `cd` into it.
2. Checkout the original commit: `git checkout 720d9dd`
3. `mkdir build && cd build`
4. `cmake ..`

CMake completes without error, but the generated libfooTargets.cmake file contains
an absolute path to libpcap.so, which is not relocatable.

```
> grep LINK_INTERFACE libfooTargets.cmake
IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "/usr/lib/x86_64-linux-gnu/libpcap.so"
```

### Imported Targets

The solution is to use imported targets and the find dependency macro.

Start by reading [Creating Relocatable Packages](http://cmake.org/cmake/help/v3.3/manual/cmake-packages.7.html#creating-relocatable-packages)
from the CMake documentation.

Here's the relevant portion:

> It is not advisable to populate any properties which may contain paths, such as
> `INTERFACE_INCLUDE_DIRECTORES` and `INTERFACE_LINK_LIBRARIES` with paths relevant to
> dependencies.

> The referenced variables may contain the absolute paths to libraries and include
> directories *as found on the machine the package was made on*.  This would create
> a package with hard-coded paths to dependencies and not suitable for relocation.

Then it says "ideally such dependencies should be used through their own IMPORTED targets."

First, update the FindPCAP.cmake module so that it creates an imported target.

```cmake
if (PCAP_FOUND AND NOT TARGET PCAP)
   add_library(PCAP IMPORTED SHARED)
   set_target_properties(PCAP PROPERTIES IMPORTED_LOCATION ${PCAP_LIBRARY})
   set(PCAP_LIBRARIES PCAP)
endif()
```

Now update the library's package config file:

```cmake
include(CMakeFindDependencyMacro)
find_dependency(PCAP)
```

Lastly, make sure the CMakeLists.txt file is using the variable you set for your imported target:

```cmake
target_link_libraries(libfoo PUBLIC ${PCAP_LIBRARIES})
```
