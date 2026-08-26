# MCMC in CARDAMOM overview

CARDAMOM uses the `MCMCID` variable in the CBF NetCDF input file to select the
MCMC algorithm used during model-data fusion. MCMC options are stored as
attributes on `MCMCID`.

Common `MCMCID` attributes include:

| Attribute | Meaning |
| --- | --- |
| `nITERATIONS` | Number of production MCMC iterations/generations. |
| `nSAMPLES` | Number of requested output samples. Used to derive `nWRITE` if `nWRITE` is not set. |
| `nWRITE` | Write interval for MCMC output. If unset, CARDAMOM uses `nITERATIONS / nSAMPLES`. |
| `nPRINT` | Interval for printing progress information. |
| `fADAPT` | Fraction of the production run assigned to the first proposal phase for hybrid modes. |
| `nSAMPLES_EDC_SEARCH` | Number of iterations used in the EDC initial-condition search. |
| `seed_number` | Random seed for deterministic/reproducible MCMC initialization and runs. |

## MCMC workflow

CARDAMOM MCMC runs follow the same broad workflow:

1. Read the CBF NetCDF file and `MCMCID` settings.
2. Set the random seed from `seed_number`, using zero if no seed is provided.
3. Search for parameter vectors that satisfy the EDCs.
4. Hand the EDC-passing parameter vectors to the selected MCMC mode.
5. Run production MCMC for `nITERATIONS` iterations.
6. Write output every `nWRITE` iterations.
7. Periodically write a `START` file that can be used to resume a run.

The EDC search and production sampler are separate phases. `nWRITE` controls
production output spacing, not EDC-search output.

## EDC initial-condition search

Before production MCMC, CARDAMOM searches for parameter vectors that satisfy
EDCs. Modes 3-10 use the same deterministic 400-chain EDC search before
mode-specific handoff. This makes comparisons among these modes more direct,
because the initial EDC search uses the same chain count, same proposal path,
same pass threshold, and same seed-controlled random sequence.

For multi-chain EDC searches, CARDAMOM currently stops after more than 10 chains
pass EDCs. Because the code checks `PEDCC > nstartchains` and `nstartchains=10`,
this means at least 11 chains must pass EDCs before the search ends.

## MCMC modes

The CARDAMOM MDF executable currently dispatches the following MCMC modes:

| MCMCID | Status | Production sampler | Chains after EDC handoff | Archive | Main proposal behavior |
| --- | --- | --- | --- | --- | --- |
| 119 | Existing/special | `MHMCMC_119` | 1 | No | Single-chain adaptive Metropolis-Hastings sampler. |
| 2 | Existing | `DEMCMC` | 100 | No | Standard live-chain differential evolution MCMC. |
| 3 | Existing | `ADEMCMC` | 400 | No | `fADAPT` controls an initial `STEP_ADEMCMC` phase, then switches to `STEP_DEMCMC`. |
| 4 | Existing | `AFDEMCMC` | 400 | No | `fADAPT` controls an initial affine/stretch phase, then switches to `STEP_DEMCMC`. |
| 5 | Added/experimental | `DEMCMCZS` | Top 10 EDC-passing chains | Yes | Archive-based differential evolution proposals plus snooker updates. |
| 6 | Added/experimental | `AFDEMCMC` | 400 | Warmup only | Keeps all 400 EDC endpoints; can optionally warm up the best 10 with DEMCMCZS before AFDEMCMC production. |
| 7 | Added/experimental | `AFDEMCMCZS` | 400 affine chains, then top 10 DEMCMCZS chains | Yes | `fADAPT` controls affine phase length, then active chains switch to DEMCMCZS using archive history. |
| 8 | Added/experimental | `DREAMZS` | 400 | Yes | DREAMZS-lite archive proposals with crossover/subspace updates plus snooker updates. |
| 9 | Added/experimental | `HYBRID_AIDE` | 400 | No | Each proposal combines CARDAMOM DEMCMC translation with affine/stretch moves. |
| 10 | Added/experimental | `HYBRID_AIDE_DEMCMC` | 400 | No | `fADAPT` controls an initial HYBRID_AIDE phase, then switches to standard CARDAMOM DEMCMC. |

## Proposal families

Most multi-chain CARDAMOM MCMC modes are built from two proposal ideas:
DEMCMC moves and affine/stretch moves. The different modes mainly change how
these proposals are combined, when the sampler switches between them, and
whether previous samples are stored in an archive.

Mode 119 is the exception. It is the older single-chain adaptive
Metropolis-Hastings path rather than a multi-chain ensemble method.

### Single-chain adaptive Metropolis-Hastings

The mode 119 sampler is the older single-chain adaptive Metropolis-Hastings
path. It proposes from the current parameter vector, adapts proposal covariance
during the adaptation portion of the run, and writes one chain.

### DEMCMC

DEMCMC proposes movement along the difference between two other live chains:

```text
new = current + gamma * (chain_a - chain_b) + noise
```

CARDAMOM's standard `DEMCMC` gamma convention is:

```text
90% of proposals: gamma = 0.1 * 2.38 / sqrt(2 * npars)
10% of proposals: gamma = 1.0
```

This is the core differential-evolution proposal used directly in mode 2 and
used after the initial phase in modes 3, 4, and 10.

### Affine/stretch

Affine ensemble proposals move one chain relative to another live chain using a
stretch factor. This proposal is useful when parameters are correlated because
the ensemble geometry helps choose directions and scales.

```text
new = chain_ref + z * (current - chain_ref)
```

The affine proposal has a non-symmetric proposal density, so CARDAMOM applies
the affine Hastings correction:

```text
(npars - 1) * log(z)
```

where `z` is the stretch factor. This correction is required so that the
proposal preserves detailed balance.

This is the core affine proposal used directly in modes 4, 6, and 7, and used
as one part of the hybrid AIDE proposal in modes 9 and 10.

### Derived modes

The remaining multi-chain modes are combinations or extensions of DEMCMC and
affine/stretch proposals:

| Method | Derived from | Description |
| --- | --- | --- |
| `ADEMCMC` | DEMCMC | Uses an adaptive differential-evolution-style proposal during the first `fADAPT` fraction of mode 3, then switches to DEMCMC. |
| `AFDEMCMC` | Affine/stretch + DEMCMC | Uses affine/stretch proposals during the first `fADAPT` fraction of mode 4, then switches to DEMCMC. |
| `DEMCMCZS` | DEMCMC | Uses archive differences from `Z` instead of only the current live ensemble, with occasional snooker proposals. |
| `AFDEMCMCZS` | Affine/stretch + DEMCMCZS | Starts with affine/stretch proposals, then switches to archive-based DEMCMCZS proposals. |
| `DREAMZS-lite` | DEMCMCZS | Adds crossover/subspace updates so some proposals update only a subset of parameters. |
| `HYBRID_AIDE` | Affine/stretch + DEMCMC | Composes a DEMCMC translation with an affine/stretch move in a single proposal. |
| `HYBRID_AIDE_DEMCMC` | HYBRID_AIDE + DEMCMC | Uses HYBRID_AIDE for the first `fADAPT` fraction, then switches to DEMCMC. |

The archive-based modes, `DEMCMCZS` and `DREAMZS-lite`, are still DEMCMC-family
methods. They differ from standard DEMCMC because proposal differences can be
drawn from previous samples stored in `Z`, not only from the current live
ensemble.

The AIDE-family modes combine the two core proposal ideas. In `HYBRID_AIDE`,
each chain independently chooses one of two proposal orderings:

```text
DEMCMC translation -> affine stretch
affine stretch -> DEMCMC translation
```

The DEMCMC translation uses CARDAMOM's standard DEMCMC gamma convention. The
affine part uses the affine Hastings correction:

```text
(npars - 1) * log(z)
```

## Suggested settings

For standard mode comparisons, change only the settings required to select the
mode or proposal split. Keep all other settings fixed:

| Setting | Suggested value |
| --- | --- |
| `seed_number` | Same value across all compared runs. |
| `nSAMPLES_EDC_SEARCH` | Same value across all compared runs. |
| `nITERATIONS` | Same value across all compared runs. |
| `nWRITE` | Set explicitly, especially for early-chain diagnostics. |
| `nPRINT` | Same value across all compared runs. |
| CBF data and priors | Identical except for intentional `MCMCID` settings. |
| Executable | Same compiled executable. |
| `START` file | Avoid unintended reuse when testing EDC behavior. |

If `nWRITE` is unset, CARDAMOM derives it from:

```text
nITERATIONS / nSAMPLES
```

This is convenient for standard output, but explicit `nWRITE` is preferred when
the goal is to compare early chain behavior.

Suggested comparison settings:

| Purpose | `MCMCID` | Key settings |
| --- | --- | --- |
| Existing 400-chain affine-only baseline | 4 | `fADAPT=1`; fixed `nITERATIONS`; explicit `nWRITE`. |
| Existing 400-chain DEMCMC-only baseline | 4 | `fADAPT=0`; fixed `nITERATIONS`; explicit `nWRITE`. |
| Existing 400-chain affine-to-DEMCMC baseline | 4 | `0 < fADAPT < 1`; fixed `nITERATIONS`; explicit `nWRITE`. |
| Small ranked DEMCMCZS test | 5 | Same EDC settings; production uses top EDC-passing chains. |
| Full 400-chain EDC endpoint test | 6 | Same EDC settings; keeps all 400 chains for AFDEMCMC production. |
| Affine-to-DEMCMCZS test | 7 | `fADAPT` sets affine fraction; DEMCMCZS uses ranked active chains after switch. |
| DREAMZS comparison | 8 | Same EDC settings; explicit `nWRITE`; compare against modes 3 and 4. |
| AIDE comparison | 9 | Same EDC settings; explicit `nWRITE`; compare against modes 3 and 4. |
| AIDE-to-DEMCMC comparison | 10 | `fADAPT` sets AIDE fraction; remaining run uses DEMCMC. |

For mode 4:

```text
fADAPT = 1.0 -> affine-only production
fADAPT = 0.0 -> DEMCMC-only production
0 < fADAPT < 1 -> affine phase followed by DEMCMC phase
```

For mode 10:

```text
fADAPT = 0.3 -> first 30% HYBRID_AIDE, remaining 70% DEMCMC
```

## FAQ

### Q: Why does the first half of output look systematically different?

A: MCMC starts from initial parameter combinations and can take time to converge.
It is common to discard the first portion of MCMC outputs as burn-in. The exact
burn-in fraction should be chosen based on the sampler, run length, and
diagnostics for the analysis.

<img width="356" alt="image" src="https://user-images.githubusercontent.com/23563444/208981078-91bc8c55-c6de-407e-aff4-98e74aad02d7.png">

Figure 1: Example of full output where early samples differ from later samples.
