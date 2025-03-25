# Debugging tips and FAQs

*Q: why is CARDAMOM_RUN_MODEL writing out different results every time on the same (local) machine?*

Likely cause: a defined variable "VAR" (e.g. "double VAR;") also needs to be initialized (e.g. "double VAR=0;") if "VAR" is then used in subsequent operations, otherwise the value of "VAR" is incorrectly set by the (seemingly random) previous memory contents


Issue: CARDAMOM can't find a "starting solution", all solutions are -inf.

Solution:   


MCMC search for EDC=1 starting solution failing



<img width="661" alt="image" src="https://user-images.githubusercontent.com/23563444/178610158-576f959d-3bf6-44a3-a6d7-8de4694148e7.png">
