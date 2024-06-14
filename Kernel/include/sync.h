//
// Created by Santi on 5/26/2024.
//

#ifndef SO_TP2_SYNC_H
#define SO_TP2_SYNC_H

// Enter the critical section
void enterRegion(int *lock);

// Leave the critical section
void leaveRegion(int *lock, int mode);

#endif //SO_TP2_SYNC_H
