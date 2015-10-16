CMake Issue
===========

How do you properly create a library that depends on another library?

In this case, the library is an example _libfoo_ library, and it depends on libpcap,
which is not a CMake project.

The example project compiles and links to libpcap correctly, but is not relocatable.

## To reproduce

1. Clone this repository and `cd` into it.
2. `mkdir build && cd build`
3. `cmake ..`

CMake completes without error, but the generated libfooTargets.cmake file contains
an absolute path to libpcap.so, which is not relocatable.

```
> grep LINK_INTERFACE libfooTargets.cmake
IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "/usr/lib/x86_64-linux-gnu/libpcap.so"
```

This project is a minimal example of an issue I'm having in a larger project.
The problem is I'm actually cross compiling on the real project, thus using
a custom `CMAKE_INSTALL_PREFIX`.  When I execute `make install`, the library
and its libfooTargets.cmake file get copied into a shared directory that mirrors
the target device's filesystem.  This works great on my machine - but the problem
is that I've installed a targets file with a hardcoded path on my machine, so 
other team members are unable to build against libfoo.

### Imported Targets?

I've read [Creating Relocatable Packages](http://cmake.org/cmake/help/v3.3/manual/cmake-packages.7.html#creating-relocatable-packages)
from the CMake documentation, and I"m pretty sure this is exactly what my problem is:

> It is not advisable to populate any properties which may contain paths, such as
> `INTERFACE_INCLUDE_DIRECTORES` and `INTERFACE_LINK_LIBRARIES` with paths relevant to
> dependencies.

> The referenced variables may contain the absolute paths to libraries and include
> directories *as found on the machine the package was made on*.  This would create
> a package with hard-coded paths to dependencies and not suitable for relocation.

Then it says "ideally such dependencies should be used through their own IMPORTED targets."

I haven't been able to figure out how to set this up.  Your help would be greatly appreciated.
