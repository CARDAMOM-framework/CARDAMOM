%Step 1. load existing structure
cbfffilename=[getenv('CARDAMOM_DATA_PATH'),'/CARDAMOM_DATA_DRIVERS_EXAMPLE.cbf'];


CBF=CARDAMOM_READ_BINARY_FILEFORMAT(cbffilename);


%Step 2. Test 
CARDAMOM_CHECK_CBF_FOR_ERRORS(CBF)