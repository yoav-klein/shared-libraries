
# Order matters
---

In this setup, we have the `libfoo.so` library referencing the `libbar.so` library
This shows you that:

## Linking order is important
The way the linker works is, that it sees references, and then it knows what to look for in later libraries/object files. So if you try to put `-lbar` before `-lfoo` in the linking order, it will fail !

