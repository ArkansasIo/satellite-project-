#ifndef ASTERIA_STORAGE_H
#define ASTERIA_STORAGE_H

#include <stdint.h>

typedef struct {
    uint32_t capacity_packets;
    uint32_t used_packets;
    uint32_t dropped_packets;
} StorageState;

void storage_init(StorageState *storage, uint32_t capacity_packets);
uint32_t storage_write_packets(StorageState *storage, uint32_t packets);
uint32_t storage_read_packets(StorageState *storage, uint32_t max_packets);

#endif
