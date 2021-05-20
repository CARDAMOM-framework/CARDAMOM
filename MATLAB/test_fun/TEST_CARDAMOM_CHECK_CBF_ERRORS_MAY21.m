%Step 1. load existing structure
cbffilename=[getenv('CARDAMOM_DATA_PATH'),'/CARDAMOM_DATA_DRIVERS_EXAMPLE.cbf'];%edited by MS


CBF=CARDAMOM_READ_BINARY_FILEFORMAT(cbffilename);


%Step 2. Test 
CARDAMOM_CHECK_CBF_FOR_ERRORS(CBF)
