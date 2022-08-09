## Appendix <a name="appendix"/>

### CARDAMOM model library <a name="cardamom-model-library"/>

List and brief description of currently supported models\

Can use the COMPLEX effort to document all the models here, including some examples of the figures.\

#### TABLE A1. CARDAMOM model IDs.

| Group                            | Model ID   | Parent ID(s) if relevant | Description                                         | Details (POC)                   | Status                          |
|----------------------------------|----------  |--------------------------|-----------------------------------------------------|---------------------------------|---------------------------------|
| Low complexity                   | 101        |                          | 3-pool DALEC                                        |                                 | Published (Famiglietti et al., 2021)                   |
| DALEC C only                     | 400        |                          |                                                     | Bloom et al., 2016              | Published                       |
| DALEC C + plant-available water. | 803        |                          |                                                     |                                 |                                 |
|                                  | 804        |                          |                                                     |                                 |                                 |
|                                  | 805        |                          |                                                     |                                 |                                 |
|                                  | 806        |                          |                                                     |                                 |                                 |
|                                  | 807        |                          |                                                     |                                 |                                 |
|                                  | 808        |                          |                                                     |                                 |                                 |
|                                  | 809        |                          |                                                     |                                 |      Published (Yin et al., 2021)     |
|                                  | 810        |                          | Non-binary EDCs                                     |                                 | Exploratory                     |
|                                  | 811        | 809                      |                                                     |                                 | Published (Quetin et al., 2020) |
|                                  | 812        | 813                      | 813 with uWUE                                       |                                 |                                 |
|                                  | 813        | 811                      | 811 with Shuang Ma’s temp fix                       |                                 | Published (Bloom et al., 2020)  |
|                                  | 820        | 811                      | Climate-sensitive mortality                         |                                 |                                 |
|                                  | 821        |                          | Ball-berry scheme                                   |                                 |    In prep.                     |
|                                  | 830        |     809                  |     Stephanie Stettz old weather GPP test           |                                 |                                 |
|                                  | 831        |       809                |    Stettz et al. cold weather GPP                   |                                 |     Stettz et al., 2021, in prep.      |
|                                  | 840        |         No idea          |         No idea                                     |                                 |                                 |
| DALEC + PAW + PUW                | 1000       |      811                 |                                                     |                                 |  Published (Famiglietti et al., 2021)   |
|                                  | 1001       | 1000                     | 1000 with max infiltration                          | Paul Levine                     |                                 |
|                                  | 1002       | 1000                     | 1000 with uWUE                                      | Paul Levine                     |                                 |
|                                  | 1003       | 1000                     | 1000 with surface runoff proportional to P          | Paul Levine                     | Published (Famiglietti et al., 2021)  |
|                                  | 1004       | 1000                     | Testing new modular parameter definitions           | Anthony Bloom                   | In prep.                        |
|                                  | 1005       | 1000                     | Boese et al, 2017 (uWUE + rad)                      | Paul Levine, Yan Yang           |    Yang et al., in prep.        |
|                                  | 1006       | 1005                     | 1005 with continuous EDCs                           | Paul Levine                     | Exploratory                     |
|                                  | 1007       | 1005                     | 1005 with max infiltration                          | Paul Levine                      | Exploratory                     |
|                                  | 1008       | 1002                     | 1002 with surface runoff proportional to P          | Paul Levine                      | Exploratory                     |
|                                  | 1009       | 1005                     | 1005 with surface runoff proportional to P          | Paul Levine                       | Exploratory                     |
|                                  | 1010-1014       |                          | CH4 module                                          | Ma                              | In prep.                        |
|                                  | 1015-1019  |                          | snow module                                          | Ma                              | In prep.                        |
|                                  | 1020       |                          |                                                     | Norton                          | In prep.                        |
|                                  | 1021       |                          |                                                     | Norton                          | In prep.                        |
|                                  | 1025       |                          |                                                     | Norton                          | In prep.                        |
|                                  | 1030--1039 | 1000                     | VPD-GPP sensitivity                                 | Paul Levine                     |     In prep.                  |
|                                  | 1040       | 1005                     | Nutrient model                                      | Anthony Bloom                   | In prep.                        |
|                                  | 1045       | 1005                     | Agriculture and non-fire disturbance model                                   | Anthony Bloom                   | In prep.                        |
|                                  | 1050       | 1000                     | Nitrogen model                                      | Renato Braghiere                | In prep.                        |
|                                  | 1060--1069 | 1000                     | Hydrology from Massoud et al, 2021                  | Paul Levine                     |                                 |
| Flagship testing                 | 1090--1099 |101x,105x,106x,110x       | Testing of flagship model                           |                                 | For testing only                |
| Flagship                         | 1100--1110 |101x,105x,106x            | "Kitchen sink" model                                |                                 | Exploratory                     |
| frozen 1100 for fluxnetCH4 and TROPIC GC4x5 analysis     | 1107 |            | gradually take in new updates from 1100, ensure a working version for research purpose    |         | Shuang Ma                     |
| DALEC + FF                       | 1200       |                          |                                                     |                                 | Exploratory                     |

### Standard Input, Outputs, Parameters <a name="cardamom-input-output-parameters"/>
