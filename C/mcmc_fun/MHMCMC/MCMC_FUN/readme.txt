MHMCMC

Full Metropolis Hasting algorithm - performs all functions and returns file with full parameter distributions


MHMCMC_SINGLE

Single convergence Metropolis hastings algorithm - this is suitable for finding a single solution to a specific problem. The function is identical to MHMCMC except (a) parameter distributions
are NOT written to files (b) the function will automatically stop if P=1 and (c) it will return a P=1 parameter combination. For example, this function is ideal for choosing appropriate initial conditions.

