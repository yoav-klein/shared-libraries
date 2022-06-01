# Linking order in CMake
---

In this example, we have 2 static libraries built in CMake: LibFoo and LibBar.
Both use the `libmyprintf` library, which in this case simulates some external library.

Now, we have the Application that links with `libfoo` and `libbar`.

The key point here is that in the linking order in the `target_link_libraries`, Bar and Foo 
must appear BEFORE `libmyprintf`.