
# Position Independent Code

## GOT
First, let's see GOT in action. GOT is just a table of addresses of global data, so all the references to data
in our library is through this level of indirection. The benefits of this are:
1. Performing relocations only in the .got section instead of the whole text section
2. That way you can share the text section across processes.

So the GOT is a table of addresses to the global data, so each reference to this data in the code will retreive this address from
the GOT.

Build the makefile and look at the assembly of the `libfoo.so`:
```
00000000000010f9 <func>:
    10f9:	f3 0f 1e fa          	endbr64 
    10fd:	55                   	push   %rbp
    10fe:	48 89 e5             	mov    %rsp,%rbp
    1101:	89 7d fc             	mov    %edi,-0x4(%rbp)
    1104:	48 8b 05 d5 2e 00 00 	mov    0x2ed5(%rip),%rax        # 3fe0 <myglob-0x40>
    110b:	8b 10                	mov    (%rax),%edx
    110d:	8b 45 fc             	mov    -0x4(%rbp),%eax
    1110:	01 c2                	add    %eax,%edx
    1112:	48 8b 05 c7 2e 00 00 	mov    0x2ec7(%rip),%rax        # 3fe0 <myglob-0x40>
    1119:	89 10                	mov    %edx,(%rax)
    111b:	48 8b 05 be 2e 00 00 	mov    0x2ebe(%rip),%rax        # 3fe0 <myglob-0x40>
    1122:	8b 00                	mov    (%rax),%eax
    1124:	5d                   	pop    %rbp
    1125:	c3                   	retq   

```

You can see that any reference to `myglob` is through the use of the `rip` which is the instruction pointer.
For example, in instruction `0x1104` we add `0x2ed5` to the value of `rip`, which is `0x110b` (the next instruction) - which is `0x3fe0`.
So we retreive what is in there - which is the address of the `myglob` variable - into `rax`. Then, we move the content of it into `eax`.

When running with gdb, let's take a look at the disassembly of `func`:
```
(gdb) disassemble func
Dump of assembler code for function func:
   0x00007ffff7fc30f9 <+0>:	endbr64 
   0x00007ffff7fc30fd <+4>:	push   %rbp
   0x00007ffff7fc30fe <+5>:	mov    %rsp,%rbp
   0x00007ffff7fc3101 <+8>:	mov    %edi,-0x4(%rbp)
   0x00007ffff7fc3104 <+11>:	mov    0x2ed5(%rip),%rax        # 0x7ffff7fc5fe0
   0x00007ffff7fc310b <+18>:	mov    (%rax),%edx
   0x00007ffff7fc310d <+20>:	mov    -0x4(%rbp),%eax
   0x00007ffff7fc3110 <+23>:	add    %eax,%edx
   0x00007ffff7fc3112 <+25>:	mov    0x2ec7(%rip),%rax        # 0x7ffff7fc5fe0
   0x00007ffff7fc3119 <+32>:	mov    %edx,(%rax)
   0x00007ffff7fc311b <+34>:	mov    0x2ebe(%rip),%rax        # 0x7ffff7fc5fe0
   0x00007ffff7fc3122 <+41>:	mov    (%rax),%eax
   0x00007ffff7fc3124 <+43>:	pop    %rbp
   0x00007ffff7fc3125 <+44>:	retq   
End of assembler dump.

```

Here you can see for example the 
```
mov 0x2ed5(%rip),%rax
```
which means: Take the value in address %rip (the content of %rip) + 0x2ed5 and put it in %eax.

If we look what's in this address, we can see it holds:
```
(gdb) x 0x7ffff7fc5fe0
0x7ffff7fc5fe0:	0xf7fc6020
```

Looks like another memory address. We can see that the next instruction is:
```
0x00007ffff7fc310b <+18>:	mov    (%rax),%edx
```
This means: Take the value in address stored in %rax and put it in %edx.
What is the value stored in %rax?
```
(gdb) p &myglob
$1 = (int *) 0x7ffff7fc6020 <myglob>
(gdb) 
```
We see that this `0x..c6020` is the address of `myglob`.
So the `0x...c5fe0` is the GOT entry of myglob.

You can also see that this is the .got like this:

We can see by the output of our program that the `libfoo.so` was loaded to `0x7ffff7fc2000`, and the 4th
segment was loaded to `header  3: address=0x7ffff7fc5e78`

Now if we look with `readelf` at this segment, we can see that it includes:
```
Section to Segment mapping:
  Segment Sections...
   00     .note.gnu.property .note.gnu.build-id .gnu.hash .dynsym .dynstr .rela.dyn 
   01     .init .plt .plt.got .text .fini 
   02     .eh_frame_hdr .eh_frame 
   03     .init_array .fini_array .dynamic .got .got.plt .data .bss 

```

It contains a bunch of sections, and if we sum all those before `.got`, we get the size of `0x160`. Add this to the address where 
this segment was loaded, and you get that our myglob entry is in the `.got` in offset `0x8`.

If we take a look at the relocation section `.rela.dyn`, we can see this:
```
Relocation section '.rela.dyn' at offset 0x428 contains 8 entries:
  Offset          Info           Type           Sym. Value    Sym. Name + Addend
000000003e78  000000000008 R_X86_64_RELATIVE                    10f0
000000003e80  000000000008 R_X86_64_RELATIVE                    10b0
000000004018  000000000008 R_X86_64_RELATIVE                    4018
000000003fd8  000100000006 R_X86_64_GLOB_DAT 0000000000000000 __cxa_finalize + 0
000000003fe0  000500000006 R_X86_64_GLOB_DAT 0000000000004020 myglob + 0
000000003fe8  000200000006 R_X86_64_GLOB_DAT 0000000000000000 _ITM_registerTMCloneTa + 0
000000003ff0  000300000006 R_X86_64_GLOB_DAT 0000000000000000 _ITM_deregisterTMClone + 0
000000003ff8  000400000006 R_X86_64_GLOB_DAT 0000000000000000 __gmon_start__ + 0

```

I don't know about the first 3 entries, but you can see that the 4th is set to offset `3fd8`, which is the first entry in the `.got`, one before `myglob`.
Then, the relocation entry of `myglob` says: put in offset `0x3fe0` the value `0x4020` - which is the address of `myglob` relatively to the first byte where the 
library was loaded into.


