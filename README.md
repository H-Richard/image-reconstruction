School Project for Software Tools and Systems (CSCB09). A program that stores gray-scale images into quad trees then
reconstructs them with various depths and thresholds that down-scales the resolution. 

USAGE:

Compilation

```bash
gcc quad.c pgmio.c roundtrip.c
```

Running the program

```bash
a.exe inputfile outputfile threshold maximum depth
```

EXAMPLE:

```bash
gcc quad.c pgmio.c roundtrip.c
a.exe test.pgm output.pgm 50 80
```

test.pgm: 
