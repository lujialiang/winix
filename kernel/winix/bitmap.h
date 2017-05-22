#ifndef _BITMAP_H_
#define _BITMAP_H_

#define BITMASK_NR	32

typedef struct pattern_b{
    unsigned int pattern;
    int size;
}pattern_t;


void init_bitmap();
void bitmap_clear(unsigned long *map, int map_len);
void bitmap_fill(unsigned long *map, int map_len);

int bitmap_search(unsigned long *map, int map_len, int num);
void bitmap_set_bit(unsigned long *map, int map_len,int start);
void bitmap_set_nbits(unsigned long *map, int map_len,int start, int len);
void bitmap_reset_bit(unsigned long *map, int map_len,int start);
void bitmap_reset_nbits(unsigned long *map, int map_len,int start, int len);
int bitmap_search_pattern(unsigned long *map, int map_len, unsigned long pattern, int pattern_len);

pattern_t *extract_pattern(unsigned long *map, int map_len, int heap_break);

#endif

