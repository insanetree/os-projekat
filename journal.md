# Projekt
## Alokacija memorije
### Vazno
- Pocetak i kraj heapa
- - void* HEAP_START_ADDR
- - void* HEAP_ END_ADDR
- Velicina bloka
- - size_t MEM_BLOCK_SIZE
### MemoryAllocator
Vodi racuna o slobodnoj i zauzetoj memoriji
#### Struktura Memory_Block
```
struct Memory_block {
    uint64 size;
    struct Memory_block* next;
};
```
size - velicina slobodnog bloka u blokovima;  
next - pokazivac na sledeci slobodni blok memorije

#### Inicijalizacija memorije
`void memory_init();`

Ova funkcija inicijalizuje memoriju u smislu da postavi strukturu na pocetak heapa