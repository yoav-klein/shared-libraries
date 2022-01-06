
# Position Independent Code

## PLT

If we disassemble `libfoo.so`

```
000000000001159 <func>:
    1159:	f3 0f 1e fa          	endbr64 
    115d:	55                   	push   %rbp
    115e:	48 89 e5             	mov    %rsp,%rbp
    1161:	48 83 ec 10          	sub    $0x10,%rsp
    1165:	89 7d fc             	mov    %edi,-0x4(%rbp)
    1168:	48 8b 05 71 2e 00 00 	mov    0x2e71(%rip),%rax        # 3fe0 <myglob-0x50>
    116f:	8b 10                	mov    (%rax),%edx
    1171:	8b 45 fc             	mov    -0x4(%rbp),%eax
    1174:	01 c2                	add    %eax,%edx
    1176:	48 8b 05 63 2e 00 00 	mov    0x2e63(%rip),%rax        # 3fe0 <myglob-0x50>
    117d:	89 10                	mov    %edx,(%rax)
    117f:	b8 00 00 00 00       	mov    $0x0,%eax
    1184:	e8 d7 fe ff ff       	callq  1060 <func2@plt>
    1189:	48 8b 05 50 2e 00 00 	mov    0x2e50(%rip),%rax        # 3fe0 <myglob-0x50>
    1190:	8b 00                	mov    (%rax),%eax
    1192:	c9                   	leaveq 
    1193:	c3                   	retq   


```

we can see that the call in `func` to `func2` is:
```
    1184:	e8 d7 fe ff ff       	callq  1060 <func2@plt>
```

And if we look at `0x1060` we'll see:
```
Disassembly of section .plt.sec:

0000000000001060 <func2@plt>:
    1060:	f3 0f 1e fa          	endbr64 
    1064:	f2 ff 25 ad 2f 00 00 	bnd jmpq *0x2fad(%rip)        # 4018 <func2+0x2edf>
    106b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000001070 <bar@plt>:
    1070:	f3 0f 1e fa          	endbr64 
    1074:	f2 ff 25 a5 2f 00 00 	bnd jmpq *0x2fa5(%rip)        # 4020 <bar>
    107b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)



```

You can see that there's a PLT entry for `func2` and for `bar`, which are the 2 functions called by foo.

The PLT entry jumps to an address stored in %rip + 0x2fad (which is `0x4018`). 
So it retreives whatever is in `0x4018` and jumps to this address.

Recall that the PLT entry retreives the address to jump to from a corresponding GOT entry.

If we look at our sections, we'll see that this address falls in the `.got.plt` section:
```
 Section Headers:
  [Nr] Name              Type             Address           Offset
       Size              EntSize          Flags  Link  Info  Align
  [ 0]                   NULL             0000000000000000  00000000
       0000000000000000  0000000000000000           0     0     0
  [ 1] .note.gnu.propert NOTE             00000000000002a8  000002a8
       0000000000000020  0000000000000000   A       0     0     8
  [ 2] .note.gnu.build-i NOTE             00000000000002c8  000002c8
       0000000000000024  0000000000000000   A       0     0     4
  [ 3] .gnu.hash         GNU_HASH         00000000000002f0  000002f0
       0000000000000030  0000000000000000   A       4     0     8
  [ 4] .dynsym           DYNSYM           0000000000000320  00000320
       00000000000000d8  0000000000000018   A       5     1     8
  [...]
  [19] .got              PROGBITS         0000000000003fd8  00002fd8
       0000000000000028  0000000000000008  WA       0     0     8
  [20] .got.plt          PROGBITS         0000000000004000  00003000
       0000000000000028  0000000000000008  WA       0     0     8


```



Now let's see it  in action in gdb. Start the program with gdb, and disassemble `func`:
```
(gdb) disassemble func
Dump of assembler code for function func:
   0x00007ffff7fc3159 <+0>:	endbr64 
   0x00007ffff7fc315d <+4>:	push   %rbp
   0x00007ffff7fc315e <+5>:	mov    %rsp,%rbp
   0x00007ffff7fc3161 <+8>:	sub    $0x10,%rsp
   0x00007ffff7fc3165 <+12>:	mov    %edi,-0x4(%rbp)
   0x00007ffff7fc3168 <+15>:	mov    0x2e71(%rip),%rax        # 0x7ffff7fc5fe0
   0x00007ffff7fc316f <+22>:	mov    (%rax),%edx
   0x00007ffff7fc3171 <+24>:	mov    -0x4(%rbp),%eax
   0x00007ffff7fc3174 <+27>:	add    %eax,%edx
   0x00007ffff7fc3176 <+29>:	mov    0x2e63(%rip),%rax        # 0x7ffff7fc5fe0
   0x00007ffff7fc317d <+36>:	mov    %edx,(%rax)
   0x00007ffff7fc317f <+38>:	mov    $0x0,%eax
   0x00007ffff7fc3184 <+43>:	callq  0x7ffff7fc3060 <func2@plt>
   0x00007ffff7fc3189 <+48>:	mov    0x2e50(%rip),%rax        # 0x7ffff7fc5fe0
   0x00007ffff7fc3190 <+55>:	mov    (%rax),%eax
   0x00007ffff7fc3192 <+57>:	leaveq 
   0x00007ffff7fc3193 <+58>:	retq   

```

You can see the call to `func2@plt`, let's see what's in there:
```

(gdb) disassemble 0x7ffff7fc3060
Dump of assembler code for function func2@plt:
   0x00007ffff7fc3060 <+0>:	endbr64 
   0x00007ffff7fc3064 <+4>:	bnd jmpq *0x2fad(%rip)        # 0x7ffff7fc6018 <func2@got.plt>
   0x00007ffff7fc306b <+11>:	nopl   0x0(%rax,%rax,1)

```

We can see that it jumps to whatever is in `0x7ffff7fc6018`
```
(gdb) x 0x7ffff7fc6018
0x7ffff7fc6018 <func2@got.plt>:	0xf7fc3030
```

Now, our `func2` calls `bar()`, which is in another library `libbar.so`. Let's see the call to it:
```
(gdb) disassemble func2
Dump of assembler code for function func2:
   0x00007ffff7fc3139 <+0>:	endbr64 
   0x00007ffff7fc313d <+4>:	push   %rbp
   0x00007ffff7fc313e <+5>:	mov    %rsp,%rbp
   0x00007ffff7fc3141 <+8>:	sub    $0x10,%rsp
   0x00007ffff7fc3145 <+12>:	movl   $0x2,-0x4(%rbp)
   0x00007ffff7fc314c <+19>:	mov    $0x0,%eax
   0x00007ffff7fc3151 <+24>:	callq  0x7ffff7fc3070 <bar@plt>
   0x00007ffff7fc3156 <+29>:	nop
   0x00007ffff7fc3157 <+30>:	leaveq 
   0x00007ffff7fc3158 <+31>:	retq   
```

As above, we can see the call to the PLT entry of `bar`, let's see it:
```
(gdb) disassemble 0x7ffff7fc3070
Dump of assembler code for function bar@plt:
   0x00007ffff7fc3070 <+0>:	endbr64 
   0x00007ffff7fc3074 <+4>:	bnd jmpq *0x2fa5(%rip)        # 0x7ffff7fc6020 <bar@got.plt>
   0x00007ffff7fc307b <+11>:	nopl   0x0(%rax,%rax,1)
```

You see it jumps to whatever is in `0x7ffff7fc6020`:
```
(gdb) x 0x7ffff7fc6020
0x7ffff7fc6020 <bar@got.plt>:	0xf7fc3040
```

So `func2@plt` jumped to `0xf7fc3030` and `bar@plt` jumps to `0xf7fc3040`

Our library was loaded to `0x7ffff7fc2000`. Let's decrement this value from `0x7ffff7fc3030`, and we get `0x1030`.

Let's disassemble our `libfoo.so` and see what's in that address:
```
Disassembly of section .plt:

0000000000001020 <.plt>:
    1020:	ff 35 e2 2f 00 00    	pushq  0x2fe2(%rip)        # 4008 <_GLOBAL_OFFSET_TABLE_+0x8>
    1026:	f2 ff 25 e3 2f 00 00 	bnd jmpq *0x2fe3(%rip)        # 4010 <_GLOBAL_OFFSET_TABLE_+0x10>
    102d:	0f 1f 00             	nopl   (%rax)
    1030:	f3 0f 1e fa          	endbr64 
    1034:	68 00 00 00 00       	pushq  $0x0
    1039:	f2 e9 e1 ff ff ff    	bnd jmpq 1020 <.plt>
    103f:	90                   	nop
    1040:	f3 0f 1e fa          	endbr64 
    1044:	68 01 00 00 00       	pushq  $0x1
    1049:	f2 e9 d1 ff ff ff    	bnd jmpq 1020 <.plt>
    104f:	90                   	nop

```

OK, notice that `func2@plt` and `bar@plt` both jump to different points in this procedure (it's not a function actually, just a series of code..).
Both those entries eventually jump to the first entry in this section `0x1020`, which I suppose calls the resolver in the dynamic linker.
 
Now the interesting part. Let's move forward after the call to `func2`, and then see what's in that GOT entry:
```
// this is right before we step into func2, still pointing to the .plt section
(gdb) x 0x7ffff7fc6018
0x7ffff7fc6018 <func2@got.plt>:	0xf7fc3030

// now stepping in
(gdb) step
func2 () at foo.c:9
9	{

// now see the value again:
(gdb) x 0x7ffff7fc6018
0x7ffff7fc6018 <func2@got.plt>:	0xf7fc3139

```

Now we get a different address, let's see what's in there:
```
(gdb) disassemble 0xf7fc3139
No function contains specified address.
(gdb) disassemble 0x7ffff7fc3139
Dump of assembler code for function func2:
=> 0x00007ffff7fc3139 <+0>:	endbr64 
   0x00007ffff7fc313d <+4>:	push   %rbp
   0x00007ffff7fc313e <+5>:	mov    %rsp,%rbp
   0x00007ffff7fc3141 <+8>:	sub    $0x10,%rsp

```

That's our `func2` !!

Note that `bar@plt` still points to the .plt:
```
(gdb) disassemble 0x7ffff7fc3070
Dump of assembler code for function bar@plt:
   0x00007ffff7fc3070 <+0>:	endbr64 
   0x00007ffff7fc3074 <+4>:	bnd jmpq *0x2fa5(%rip)        # 0x7ffff7fc6020 <bar@got.plt>
   0x00007ffff7fc307b <+11>:	nopl   0x0(%rax,%rax,1)
End of assembler dump.

(gdb) x 0x7ffff7fc6020
0x7ffff7fc6020 <bar@got.plt>:	0xf7fc3040

```

Now let's step over bar and see that chagnges:
```
(gdb) n
12		bar();
...

// let's check the value now
(gdb) x 0x7ffff7fc6020
0x7ffff7fc6020 <bar@got.plt>:	0xf7fbe119

// let's see what's in there
(gdb) disassemble 0x7ffff7fbe119
Dump of assembler code for function bar:
   0x00007ffff7fbe119 <+0>:	endbr64 
   0x00007ffff7fbe11d <+4>:	push   %rbp
   0x00007ffff7fbe11e <+5>:	mov    %rsp,%rbp
   0x00007ffff7fbe121 <+8>:	lea    0xed8(%rip),%rdi        # 0x7ffff7fbf000
   0x00007ffff7fbe128 <+15>:	callq  0x7ffff7fbe050 <puts@plt>
   0x00007ffff7fbe12d <+20>:	nop
   0x00007ffff7fbe12e <+21>:	pop    %rbp
   0x00007ffff7fbe12f <+22>:	retq   

```

And this is how PLT works !!



