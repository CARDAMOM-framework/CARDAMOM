Cost function
=============

.. toctree::
   :maxdepth: 2

CARDAMOM is a Bayesian modeling framework designed to compute the posterior distribution of model parameters, based on Bayes' Theorem:

.. math::

    P(\mathbf{x} | \mathbf{O}) = \frac{P(\mathbf{O} | \mathbf{x}) \cdot P(\mathbf{x})}{P(\mathbf{O})}

In this context, :math:`P(\mathbf{x} | \mathbf{O})` represents the posterior distribution of the parameter vector :math:`\mathbf{x}` given the observation matrix :math:`\mathbf{O}`. CARDAMOM approximates :math:`P(\mathbf{x} | \mathbf{O})` using a Markov Chain Monte Carlo (MCMC) sampler. Here, :math:`P(\mathbf{x})` denotes the prior distribution of the parameters, while :math:`P(\mathbf{O} | \mathbf{x})` is the likelihood of observing :math:`\mathbf{O}` given the parameter values in :math:`\mathbf{x}`.

In CARDAMOM, :math:`P(\mathbf{x})` is expanded into two components: the parameter prior range and the ecological dynamic constraints (EDC), resulting in the following expression:

.. math::

    P(\mathbf{x} | \mathbf{O}) \propto P(\mathbf{O} | \mathbf{x}) \cdot P_{\text{prior}}(\mathbf{x}) \cdot P_{\text{EDC}}(\mathbf{x})

To simplify the cost function, we take the logarithm of the posterior distribution, yielding:

.. math::

    \log(P(\mathbf{x} | \mathbf{O})) \propto \log(P(\mathbf{O} | \mathbf{x})) + \log(P_{\text{prior}}(\mathbf{x})) + \log (P_{\text{EDC}}(\mathbf{x}))

This logarithmic transformation converts multiplicative relationships into additive ones, thereby reducing the complexity of the MCMC sampling process by narrowing the search space.

The likelihood function :math:`P(\mathbf{O} | \mathbf{x})` follows a log-normal distribution and is expressed as:

.. math::

    -0.5 \sum_i \frac{(M_i - O_i)^2}{\sigma_i^2}

Where :math:`M` represents the model predictions, :math:`O` represents the observations, and :math:`\sigma` denotes the uncertainty associated with the observations.

The prior distribution :math:`P_{\text{prior}}(\mathbf{x})` is modeled as a log-uniform distribution.





