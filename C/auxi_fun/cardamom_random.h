#ifndef CARDAMOM_RANDOM_H
#define CARDAMOM_RANDOM_H

#include <stdint.h>

typedef struct {
    uint64_t state;
    uint64_t inc;
} pcg32_random_t;

void cardarand_seed(uint64_t seed);
long cardarand(void);
double cardarand_uniform(void);

#define CARDAMOM_RAND_MAX 2147483647L

#endif
