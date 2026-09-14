#include "cardamom_random.h"

static pcg32_random_t pcg32_global = {0x853c49e6748fea9bULL, 0xda3e39cb94b95bdbULL};

static uint32_t pcg32_random_r(pcg32_random_t* rng) {
    uint64_t oldstate = rng->state;
    rng->state = oldstate * 6364136223846793005ULL + rng->inc;
    uint32_t xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
    uint32_t rot = oldstate >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
}

static void pcg32_srandom_r(pcg32_random_t* rng, uint64_t initstate, uint64_t initseq) {
    rng->state = 0U;
    rng->inc = (initseq << 1u) | 1u;
    pcg32_random_r(rng);
    rng->state += initstate;
    pcg32_random_r(rng);
}

void cardarand_seed(uint64_t seed) {
    pcg32_srandom_r(&pcg32_global, seed, 0xda3e39cb94b95bdbULL);
}

long cardarand(void) {
    return (long)(pcg32_random_r(&pcg32_global) & 0x7FFFFFFFL);
}

double cardaurand(void) {
    return (double)cardarand() / (double)CARDAMOM_RAND_MAX;
}

double cardarand_uniform(void) {
    return (double)pcg32_random_r(&pcg32_global) / (double)0x100000000ULL;
}
