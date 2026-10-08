Observation operators
=====================

.. toctree::
   :maxdepth: 2

Description and details of CARDAMOM observation operators





To write a new observation operator for a time series observation: 

#. choose cbf quantity name and model quantity name (e.g., ``GPP`` and ``M_GPP``), and type (``TIMESERIES_OBS_STRUCT``, ``SINGLE_OBS_STRUCT``, ``TIMESERIES_DRIVER_STRUCT``)

#. in ``CARDAMOM_DATA_STRUCTURE.c``, model quantity name (e.g., ``M_GPP``) is instantiated 

#. in ``CARDAMOM_NETCDF_DATA_STRUCTURE.c``, cbf quantity name (e.g., ``GPP``) is instantiated as a specific type (e.g., ``TIMESERIES_OBS_STRUCT GPP``) 

#. in ``CARDAMOM_READ NETCDF_DATA.c``

   a. cbf quantity name (e.g., ``"GPP"``) gets included, with the corresponding function for its type (e.g., ``DATA->GPP=READ_NETCDF_TIMESERIES_OBS_FIELDS(ncid, "GPP");`` for time series type obs, or other for single obs or driver types);

   #. If timeseries obs, also call the preprocess function (e.g., ``TIMESERIES_OBS_STRUCT_PREPROCESS(&DATA->GPP);``)

#. in ``DALEC_OBSERVATION_OPERATORS.c``,

   a. create boolean flag to support the obsop, (e.g.: ``bool SUPPORT_GPP_OBS;``)

   #. instantiate any sub quantities that might be needed to evaluate the observation operator (e.g., ``int Rauto_flux; int GPP_flux;`` where both ``GPP_flux`` and ``Rauto_flux`` are quantities used in the CUE observation operator)

   #. set the boolean flag to false by default in the obsop data structure (e.g., ``OBSOPE->SUPPORT_GPP_OBS=false;`` ) This gets changed in ``DALEC_1100.c`` on a case by case basis.

   #. Add to DALEC obsope structure (e.g., ``if (O->SUPPORT_GPP_OBS){DALEC_OBSOPE_GPP(D, O);}``)

   #. write the observation operator evaluation as a function of the data structure and the obsop structure, e.g.:

   .. code-block:: C

		int DALEC_OBSOPE_GPP(DATA * D, OBSOPE * O){
		
		//GPP timeseries length
		int N=D->ncdf_data.TIME_INDEX.length;
		
		//Time varying GPP 
		TIMESERIES_OBS_STRUCT TOBS=D->ncdf_data.GPP;
		if (TOBS.validobs){int n;
		    for (n=0;n<N;n++){
		        D->M_GPP[n]=D->M_FLUXES[D->nofluxes*n+O->GPP_flux];
		        }
		    };
		
		
		//Mean GPP 
		SINGLE_OBS_STRUCT SOBS=D->ncdf_data.Mean_GPP;
		if (SOBS.validobs){
		    int n;
		    D->M_Mean_GPP=0;
		    for (n=0;n<N;n++){
		        D->M_Mean_GPP+=D->M_FLUXES[n*D->nofluxes+O->GPP_flux];
		        };
		    D->M_Mean_GPP=D->M_Mean_GPP/(double)N;
		    } 
		
		return 0; 
		}
			

#. in ``DALEC_ALL_LIKELIHOOD.c``, 
   
   a. add to the list of likelihood indices (e.g., ``int GPP;``) 
   
   #. increment likelihood index count (e.g., ``…20,21,22};``) 
   
   #. add to the likelihood function (e.g., ``if (O->SUPPORT_GPP_OBS){ML[LI.GPP]=CARDAMOM_TIMESERIES_OBS_LIKELIHOOD(&D.ncdf_data.GPP, D.M_GPP);};``)

#. in ``DALEC_1100.c``, 
   
   a. create and assign the model quantities that are needed to evaluate the obsop, matching variable names declared in step b of modifying ``DALEC_OBSERVATION_OPERATORS.c``  (e.g., ``OBSOPE.GPP_flux=F.gpp;``) and 
   
   #. turn the obsop on (e.g., ``OBSOPE.SUPPORT_GPP_OBS=true;``)

#. If this is a completely new variable for the CBF, then in ``CARDAMOM_READ_BINARY_DATA.c``, add a memory allocation (e.g., ``DATA->M_GPP=calloc(Ntimesteps,sizeof(double));``) **NOTE:** can trigger bus error if skipped

#. When putting data into the CBF under the new field, also check 
   
   #. unc name matches filter type, etc (e.g., ``opt_filter=1`` takes ``single_mean_unc``, nothing else will work), 
   
   #. filter logic matches obs values ranges (e.g., don’t use ``opt_unc_type=1`` on something with negative values), 
   
   #. fill value encoding & time encoding (check using `xarray <https://docs.xarray.dev/en/stable/generated/xarray.DataArray.encoding.html>`_ or `ncdump <https://www.unidata.ucar.edu/software/netcdf/workshops/2011/utilities/Ncdump.html>`_)
