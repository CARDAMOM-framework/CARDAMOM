Modified Farquhar-von Caemmerer-Berry model
===============================

.. toctree::
   :maxdepth: 2
   :caption: Contents:

Description and equations for Modified Farquhar-von Caemmerer-Berry model photosynthesis scheme

To model photosynthesis and hence calculate GPP we apply a vairant of the Farquhar-von Caemmerer-Berry model adapted from \citet{Liu:2021vu}.

The absorbed photosynthetically active radiation (APAR) driving photosynthesis is calculated using the Beer-Lambert law:

\begin{equation}
    APAR = SW \cdot (1 - \rho_\text{leaf}) \cdot (1 - \exp(-K \cdot LAI \cdot \Omega))
\end{equation}

where $SW$ is the incoming shortwave radiation, $\rho_\text{leaf}$ is the a paramater describing the PAR canopy reflectance, $K$ is the vegetation extinction coefficient \citep{Campbell:1998to}, $LAI$ is the leaf area index, and $\Omega$ is the clumping index \citep{Braghiere:2019wu}.

Gross Primary Production (GPP) is calculated based on C3 and C4 photosynthesis biochemistry to determine potential leaf-level photosynthesis (unstressed by water availability). This is expressed in terms of two potentially limiting rates:

\begin{enumerate}
    \item Rubisco-limited rate ($a_1$):
    \begin{equation}
        a_1 = \begin{cases}
            V_\text{cmax} \cdot \frac{c_i - c_p}{c_i + K_c \cdot (1 + 209/K_o)} & \text{for C3} \\
            V_\text{cmax} & \text{for C4}
        \end{cases}
    \end{equation}
    
    where $V_\text{cmax}$ (mol CO$_2$ m$^{-2}$ s$^{-1}$) is the maximum rate of carboxylation.
    
    \item Light-limited rate ($a_2$):
    \begin{equation}
        a_2 = \begin{cases}
            J \cdot \frac{c_i - c_p}{4(c_i + 2c_p)} & \text{for C3} \\
            J & \text{for C4}
        \end{cases}
    \end{equation}
    
    where $J$ is the rate of electron transport.
\end{enumerate}

The total net carbon assimilation ($A_n$) is given by the weighted sum of C3 and C4 net carbon assimilation fractions:

\begin{equation}
    A_n = \max(0, \min(a_1 \cdot \beta, a_2) - R_d)
\end{equation}

where $\beta$ is the moisture stress factor related to the mean soil moisture concentration in the root zone, and $R_d$ is the leaf dark respiration, calculated as $0.015 \cdot V_\text{cmax} \cdot \beta$.

GPP, representing the total canopy photosynthesis, is calculated by integrating leaf-level photosynthesis over the entire canopy leaf area index:

\begin{equation}
    GPP = A_n \cdot \frac{1 - \exp(-K \cdot LAI \cdot \Omega)}{K}
\end{equation}
