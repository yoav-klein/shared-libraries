

# Symbol tables

This folder comes to demonstrate symbol tables.
Each symbol in an object file is represented by a structure:

```
typedef struct elf64_sym {
  Elf64_Word st_name;		/* Symbol name, index in string tbl */
  unsigned char	st_info;	/* Type and binding attributes */
  unsigned char	st_other;	/* No defined meaning, 0 */
  Elf64_Half st_shndx;		/* Associated section index */
  Elf64_Addr st_value;		/* Value of the symbol */
  Elf64_Xword st_size;		/* Associated symbol size */
} Elf64_Sym;

```

## Object file (.o)
### Functions
So in our example, if you look at the symbol table of `main.o` you can see that:
```
 Num:    Value          Size Type    Bind   Vis      Ndx Name

 23: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND foo
 24: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND somewhere_else
 25: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND printf

```
These are symbols that are referenced in our file but are not defined here.
The `st_shndx` filed of these is marked as UND - undefined. That means that those functions are not defined in 
this object, and they'll be searched for somewhere else.

Functions that are defined in this object look like this:
```
 Num:    Value          Size Type    Bind   Vis      Ndx Name

 6: 0000000000000000    23 FUNC    LOCAL  DEFAULT    1 only_here
...
 24: 0000000000000017    23 FUNC    GLOBAL DEFAULT    1 in_main
 25: 000000000000002e    73 FUNC    GLOBAL DEFAULT    1 main
```
Note that `only_here`, which is defined as `static`, has the LOCAL binding, as opposed to main and in_main which have the GLOBAL binding.
Also note that all three have the index of 1, which is the `.text` section.

The `Value` in this case represents an offset within a section. And if we look at the disassembly, we could see that the offset of `in_main` is indeed 17:
```
0000000000000017 <in_main>:
  17:	f3 0f 1e fa          	endbr64 
  1b:	55                   	push   %rbp
```

### Data
Let's look at the data symbols:
```
 7: 0000000000000004     4 OBJECT  LOCAL  DEFAULT    3 static_int
 ...
 21: 0000000000000000     4 OBJECT  GLOBAL DEFAULT    3 glob_int
 22: 0000000000000000     8 OBJECT  GLOBAL DEFAULT    6 my_name
 ...
 25: 0000000000000000     0 NOTYPE  GLOBAL DEFAULT  UND somewhere_else

```
You can see that all are of type OBJECT - which is a data symbol.
`static_int` and `glob_int` both refer to section 3, which is the `.data` section.
Note that `static_int` has the LOCAL binding, since it's declared as `static`.

`somewhere_else` is defined as UND - which means that it is not defined here.

## Executable
Let's compile our program with 
```
$ make static
```

This will compile `libfoo.a` and link our main with it statically.

If we now look at our symbol table, we can see that the undefined symbols are resolved:
```
71: 0000000000001204    43 FUNC    GLOBAL DEFAULT   16 foo
...
75: 0000000000001197    73 FUNC    GLOBAL DEFAULT   16 main
76: 0000000000001180    23 FUNC    GLOBAL DEFAULT   16 in_main
...
78: 0000000000004024     4 OBJECT  GLOBAL DEFAULT   26 somewhere_else

```
You can see that the symbols that were undefined in the `main.o` object file are now resolved and point to a memory address.

Also note that when looking at a symbol table of an executable, the Value is a memory address, and not an offset within a section.





