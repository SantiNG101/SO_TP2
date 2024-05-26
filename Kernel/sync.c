//
// Created by Santi on 5/26/2024.
//

#include "include/sync.h"

void enterRegion(int *lock) {
    while (__sync_lock_test_and_set(lock, 1)) {
        // Busy-wait until the lock is released
    }
}

void leaveRegion(int *lock, int mode) {
    __sync_lock_release(lock);
    //Write function to use mode once scheduler is done
}
