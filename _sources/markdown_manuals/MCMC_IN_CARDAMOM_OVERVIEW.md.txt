# MCMC in CARDAMOM Overview

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

## EDC Initial-Condition Search

Before production MCMC, CARDAMOM searches for parameter vectors that satisfy
EDCs. Modes 3-10 use the same deterministic 400-chain EDC search before
mode-specific handoff. This makes comparisons among these modes more direct,
because the initial EDC search uses the same chain count, same proposal path,
same pass threshold, and same seed-controlled random sequence.

For multi-chain EDC searches, CARDAMOM currently stops after more than 10 chains
pass EDCs. Because the code checks `PEDCC > nstartchains` and `nstartchains=10`,
this means at least 11 chains must pass EDCs before the search ends.

## MCMC Modes

| MCMCID | Production sampler | Chains after EDC handoff | Archive | Main proposal behavior |
| --- | --- | --- | --- | --- |
| 2 | `DEMCMC` | 100 | No | Standard live-chain differential evolution MCMC. |
| 3 | `ADEMCMC` | 400 | No | `fADAPT` controls an initial `STEP_ADEMCMC` phase, then switches to `STEP_DEMCMC`. |
| 4 | `AFDEMCMC` | 400 | No | `fADAPT` controls an initial affine/stretch phase, then switches to `STEP_DEMCMC`. |
| 5 | `DEMCMCZS` | Top 10 EDC-passing chains | Yes | Archive-based differential evolution proposals plus snooker updates. |
| 6 | `AFDEMCMC` | 400 | Warmup only | Keeps all 400 EDC endpoints; can optionally warm up the best 10 with DEMCMCZS before AFDEMCMC production. |
| 7 | `AFDEMCMCZS` | 400 affine chains, then top 10 DEMCMCZS chains | Yes | `fADAPT` controls affine phase length, then active chains switch to DEMCMCZS using archive history. |
| 8 | `DREAMZS` | 400 | Yes | DREAMZS-lite archive proposals with crossover/subspace updates plus snooker updates. |
| 9 | `HYBRID_AIDE` | 400 | No | Each proposal combines CARDAMOM DEMCMC translation with affine/stretch moves. |
| 10 | `HYBRID_AIDE_DEMCMC` | 400 | No | `fADAPT` controls an initial HYBRID_AIDE phase, then switches to standard CARDAMOM DEMCMC. |

## Recommended CBF MCMCID Settings for Comparing Methods

MCMC settings are stored as NetCDF attributes on the `MCMCID` variable in the CBF file. The most important attributes to set for clean sampler comparisons are:

| Attribute | Recommended use |
| --- | --- |
| `MCMCID` | Selects the sampler mode. |
| `nITERATIONS` | Set to the desired production run length. |
| `nWRITE` | Set explicitly for comparison runs so output frequency is standardized. |
| `nPRINT` | Set to a reasonable progress interval. |
| `fADAPT` | Set for sequential hybrid modes that switch proposal behavior during the run. |
| `seed_number` | Keep fixed across comparison runs for reproducible initialization. |

For clean comparisons, use the same values of `nITERATIONS`, `nWRITE`, `nPRINT`, `fADAPT`, and `seed_number` unless the purpose of the experiment is to test one of those settings directly.

Example recommended comparison setup:

```text
nITERATIONS = 350000
nWRITE = 500
nPRINT = 500
fADAPT = 0.3
seed_number = fixed across runs
```

With this setup, each run writes output every 500 production iterations. This makes trace plots and chain-level diagnostics easier to compare across modes.

### Recommended Settings for Routine Runs

For routine science runs, use mode 9 with 350,000 MCMC iterations. Mode 9 has
shown similar behavior to mode 4 while requiring roughly half the wall time for
the same number of iterations.

| Setting | Recommended value |
| --- | --- |
| `MCMCID` | `9` |
| `nITERATIONS` | `350000` |
| `nSAMPLES` | `2000` |
| `nWRITE` | `175` |
| `nPRINT` | `1000` |
| `nSAMPLES_EDC_SEARCH` | `200000` |
| `seed_number` | Set explicitly for reproducibility. |

Mode 4 remains the conservative fallback option.

| Setting | Recommended value |
| --- | --- |
| `MCMCID` | `4` |
| `nITERATIONS` | `350000` |
| `nSAMPLES` | `2000` |
| `nWRITE` | `175` |
| `nPRINT` | `1000` |
| `nSAMPLES_EDC_SEARCH` | `200000` |
| `seed_number` | Set explicitly for reproducibility. |

### Choosing `fADAPT`

For modes with a phase switch, `fADAPT` controls the fraction of production iterations spent in the first proposal phase.

For example, with:

```text
nITERATIONS = 350000
fADAPT = 0.3
```

the first phase lasts:

```text
0.3 * 350000 = 105000 iterations
```

Mode-specific interpretation:

```text
Mode 4: first 105000 iterations use affine/stretch proposals, then DEMCMC
Mode 7: first 105000 iterations use affine/stretch proposals, then DEMCMCZS
Mode 10: first 105000 iterations use HYBRID_AIDE, then DEMCMC
```

## Proposal Building Blocks and Hybrid Modes

Most of the MCMC modes in this branch are built from two main population-based proposal families: affine/stretch proposals and differential-evolution proposals.

### Affine/Stretch Proposals

Affine-style proposals move one chain relative to another live chain in the ensemble:

```text
new = partner + z * (current - partner)
```

where `z` is a random stretch factor. This proposal is useful when the posterior has elongated or correlated geometry, because the move is defined using the shape of the current ensemble rather than a fixed coordinate-wise step size.

The affine proposal uses the Hastings correction:

```text
(npars - 1) * log(z)
```

### DEMCMC Proposals

DEMCMC proposes movement using the difference between two other live chains:

```text
new = current + gamma * (chain_a - chain_b) + noise
```

This allows the sampler to use the scale and orientation of the ensemble to generate jumps. CARDAMOM's standard `DEMCMC` gamma convention is:

```text
90% of proposals: gamma = 0.1 * 2.38 / sqrt(2 * npars)
10% of proposals: gamma = 1.0
```

### Archive-Based DEMCMCZS Proposals

DEMCMCZS extends DEMCMC by drawing difference vectors from an archive `Z` of previous chain states rather than only from the current live ensemble. This can make proposals cheaper and more diverse, especially when running with fewer active chains. DEMCMCZS also includes occasional snooker proposals.

### DREAMZS-lite Proposals

DREAMZS-lite uses the DEMCMCZS archive structure but adds crossover/subspace updates. This means each proposal can update only a subset of parameters rather than always moving the full parameter vector.

### Sequential Hybrid Modes

Sequential hybrid modes use one proposal family during an early phase of the run and then switch to another proposal family later. The switch point is controlled by `fADAPT`.

For example:

```text
Mode 4: affine/stretch phase -> DEMCMC phase
Mode 7: affine/stretch phase -> DEMCMCZS phase
Mode 10: HYBRID_AIDE phase -> DEMCMC phase
```

These modes are useful for testing whether one proposal is better for burn-in while another is better for production sampling.

### Mixed-Kernel Hybrid Modes

Mixed-kernel modes combine proposal mechanisms inside the production sampler rather than only switching once between phases.

For example:

```text
Mode 5: DEMCMCZS archive proposals + snooker proposals
Mode 8: DREAMZS-lite archive proposals + crossover/subspace updates + snooker proposals
Mode 9: HYBRID_AIDE, which combines DEMCMC translation with affine/stretch movement in each proposal
```

HYBRID_AIDE is the most direct mixed-kernel proposal. Each proposal composes a DEMCMC translation with an affine stretch move, using one of two possible orderings:

```text
DEMCMC translation -> affine stretch
affine stretch -> DEMCMC translation
```

## Adding a new MCMC mode

As previously stated, MCMC modes in CARDAMOM are selected through the `MCMCID` value in the CBF
NetCDF file. Adding a new mode usually requires changes in two places: the MDF
driver that dispatches the selected sampler, and the MCMC function files that
implement the proposal logic.

### Main files to update

| File | What to change |
| --- | --- |
| `C/projects/CARDAMOM_MDF/CARDAMOM_MDF.c` | Include the new sampler file, assign the chain count for the new `MCMCID`, and add a `case` in the MCMC dispatcher. |
| `C/projects/CARDAMOM_MDF/MCMC_SETUP/PROJECT_FUN/FIND_EDC_INITIAL_VALUES.c` | Decide how the new mode should receive EDC-passing initial chains. For fair comparisons, new multi-chain modes should usually use the same deterministic 400-chain EDC search as modes 3-10. |
| `C/mcmc_fun/MHMCMC/MCMC_FUN/<NEW_MODE>.c` | Implement the production sampler loop. This usually handles chain state, likelihood calls, acceptance, output writing, and restart writing. |
| `C/mcmc_fun/MHMCMC/MCMC_FUN/STEP_<NEW_MODE>.c` | Optional but preferred if the proposal step is complex. Keeps proposal construction separate from the sampler loop. |
| `PYTHON/CARDAMOM_WRITE_CBF_NC_FILE.py` | Update only if the new mode needs new CBF attributes or defaults. |

### A Brief Implementation checklist

1. Pick an unused `MCMCID`.

2. Add the sampler include in `CARDAMOM_MDF.c`.

3. Set the intended number of chains for the new mode in `CARDAMOM_MDF.c`.

4. Add a new `case` in the `switch (MCOPT.mcmcid)` block that calls the new sampler.

5. Decide how EDC initialization should work in `FIND_EDC_INITIAL_VALUES.c`.

6. Make sure the new mode receives `PI.parini` in the expected shape:
   `nchains * npars`.

7. Use the same output-writing convention as the existing samplers:
   write results every `MCO.nWRITE` iterations.

8. Write restart files consistently if the sampler supports restart behavior.

9. Recompile CARDAMOM and confirm that the executable timestamp changed. This ensures the run is using the newly compiled code rather than an older executable. 

10. Run short diagnostic tests before launching long production runs.

### Things to watch out for

The EDC search and production sampler are separate phases. If the goal is to
compare MCMC algorithms, the new mode should not accidentally use a different
EDC search path, chain count, seed sequence, or EDC stopping rule unless that is
the intended experiment.

Chain count matters. Some modes keep all 400 EDC endpoints, while others rank
EDC-passing chains and continue with a smaller active ensemble. This affects
both the math of the sampler and the shape of the output files.

Archive-based methods need extra care. If the sampler uses an archive `Z`, decide
what goes into the archive, when archive rows are added, and whether early affine
or warmup samples should be included.

Proposal symmetry matters. Standard DEMCMC proposals are usually symmetric and
do not need a Hastings correction. Affine/stretch proposals are not symmetric
and require the affine Hastings correction. Hybrid proposals need careful
checking so that the acceptance probability matches the actual proposal
mechanism.

Restart behavior can differ by mode. If a new sampler does not support restart
or append behavior, document that clearly and avoid silent reuse of incompatible
`START` files.

Output size can grow quickly. `nWRITE`, `nITERATIONS`, and `nchains` together
control how many parameter vectors are written. For early diagnostic runs, set
`nWRITE` explicitly so the output is interpretable and comparable across modes.

## FAQ

### Q: Why does the first half of output look systematically different?

A: MCMC starts from initial parameter combinations and can take time to converge.
It is common to discard the first portion of MCMC outputs as burn-in. The exact
burn-in fraction should be chosen based on the sampler, run length, and
diagnostics for the analysis.

<img width="356" alt="image" src="https://user-images.githubusercontent.com/23563444/208981078-91bc8c55-c6de-407e-aff4-98e74aad02d7.png">

Figure 1: Example of full output where early samples differ from later samples.
