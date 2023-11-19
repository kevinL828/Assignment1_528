# Compiler settings
# GNU compiler
GCC = gcc
# Intel compiler
ICC = icc

# Default rule
all: ci fi comp fomp icomp ifomp

# make ci compiles
ci: cInsertion.c coordReader.c
	$(GCC) -std=gnu99 cInsertion.c coordReader.c -o ci.exe -lm

# make fi compiles
fi: fInsertion.c coordReader.c
	$(GCC) -std=gnu99 fInsertion.c coordReader.c -o fi.exe -lm

# make comp compiles
comp: ompcInsertion.c coordReader.c
	$(GCC) -std=gnu99 ompcInsertion.c coordReader.c -o comp.exe -fopenmp -lm

# make fomp compiles
fomp: ompfInsertion.c coordReader.c
	$(GCC) -std=gnu99 ompfInsertion.c coordReader.c -o fomp.exe -fopenmp -lm

# make icomp compiles
icomp: ompcInsertion.c coordReader.c
	$(ICC) -std=gnu99 ompcInsertion.c coordReader.c -o icomp.exe -qopenmp -lm

# make ifomp comliles
ifomp: ompfInsertion.c coordReader.c
	$(ICC) -std=gnu99 ompfInsertion.c coordReader.c -o ifomp.exe -qopenmp -lm

