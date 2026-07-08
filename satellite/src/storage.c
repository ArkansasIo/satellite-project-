#include "storage.h"

void storage_init(StorageState *storage, uint32_t capacity_packets) {
    storage->capacity_packets = capacity_packets;
    storage->used_packets = 0;
    storage->dropped_packets = 0;
}

uint32_t storage_write_packets(StorageState *storage, uint32_t packets) {
    uint32_t free_packets = storage->capacity_packets - storage->used_packets;
    uint32_t accepted = packets <= free_packets ? packets : free_packets;
    storage->used_packets += accepted;
    storage->dropped_packets += packets - accepted;
    return accepted;
}

uint32_t storage_read_packets(StorageState *storage, uint32_t max_packets) {
    uint32_t read = storage->used_packets <= max_packets ? storage->used_packets : max_packets;
    storage->used_packets -= read;
    return read;
}
