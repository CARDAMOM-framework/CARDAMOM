#!/usr/bin/env bash
###############################################################################
# CARDAMOM_COMPILE.sh
###############################################################################
usage() {
  echo "Usage: CARDAMOM_COMPILE.sh " 1>&2
  echo "          -c Compiler - C compiler to use. Defaults to the CARDAMOM_OPT_COMPILER enviroment var, and then gcc" 1>&2
  echo "          -p Cardamom C path - Path to the Cardamom C code directory. Defaults to the CARDAMOM_C_PATH enviroment variable, and then looks for the c path relative to this script assuming the standard cardamom file structure. " 1>&2
  echo "          -d Compile in debug mode - Builds Cardamom C code with debugging symbols" 1>&2
  echo "          -s Compile in sanitize mode - Builds AddressSanitizer and UndefinedBehaviorSanitizer executables for memory debugging" 1>&2
  echo "          -n nc-config path - Path to the nc-config command, used to get info for compileing with the netcdf libraries. Defaults to the value of the CARDAMOM_NC_CONFIG_PATH enviroment variable, then whatever nc-config command can be found with the which command, and then finally defaults to /usr/local/bin/nc-config" 1>&2
  exit 1
}
#Default values for arguments
if [[ -z "${CARDAMOM_OPT_COMPILER}" ]]; then
  COMPILER='gcc'
else
  COMPILER="${CARDAMOM_OPT_COMPILER}"
fi

while getopts ":c:p:dsn:" arg; do
  case "${arg}" in
    c )
  COMPILER=${OPTARG}
      ;;
    p )
  CARDAMOM_C_PATH=${OPTARG}
      ;;
    d )
  DEBUG="True"
      ;;
    s )
  SANITIZE="True"
      ;;
    n )
  CARDAMOM_NC_CONFIG_PATH=${OPTARG}
      ;;
    : )
       echo "Invalid option: ${OPTARG} requires an argument" 1>&2
       usage
       ;;
    \? )
       echo "Unknown option: ${OPTARG}" 1>&2
       usage
       ;;
  esac
done

shift $((OPTIND-1))

#Begin to check manditory variables
if [[ -z "${CARDAMOM_C_PATH}" ]]; then
  CARDAMOM_C_PATH="`dirname \"$0\"`/../C"
  echo "Warning: CARDAMOM_C_PATH enviroment variable was not set, and no -p option was provided. We are guessing it is in $CARDAMOM_C_PATH"
fi

if [[ -z "${CARDAMOM_NC_CONFIG_PATH}" ]]; then
  if which nc-config > /dev/null 2>&1; then
      CARDAMOM_NC_CONFIG_PATH="$(which nc-config)"
  elif /usr/local/bin/nc-config --libs > /dev/null 2>&1; then
      CARDAMOM_NC_CONFIG_PATH="/usr/local/bin/nc-config"
      echo "Warning: the CARDAMOM_NC_CONFIG_PATH enviroment variable was not set, and the -n flag set was not present. We are assuming mac's default path of /usr/local/bin/nc-config"
  else
      echo "Error: could not find any nc-config command. Make sure your netcdf library is installed, and re-run with the -n flag set, or the CARDAMOM_NC_CONFIG_PATH enviroment variable set"
      exit 1
  fi
else
  echo "setting CARDAMOM_NC_CONFIG_PATH to ${CARDAMOM_NC_CONFIG_PATH}"
fi

echo COMPILER=$COMPILER

export NETCDF_LIB_FLAGS="$(${CARDAMOM_NC_CONFIG_PATH} --libs) $(${CARDAMOM_NC_CONFIG_PATH} --cflags)"

# ---------------------------------------------------------
# STANDARD BUILD
# ---------------------------------------------------------
echo ${COMPILER} ${CARDAMOM_C_PATH}/projects/CARDAMOM_GENERAL/CARDAMOM_RUN_MODEL.c -o ${CARDAMOM_C_PATH}/projects/CARDAMOM_GENERAL/CARDAMOM_RUN_MODEL.exe -lm ${NETCDF_LIB_FLAGS}

${COMPILER} ${CARDAMOM_C_PATH}/projects/CARDAMOM_GENERAL/CARDAMOM_RUN_MODEL.c -o ${CARDAMOM_C_PATH}/projects/CARDAMOM_GENERAL/CARDAMOM_RUN_MODEL.exe -lm ${NETCDF_LIB_FLAGS}
if [ $? -ne 0 ]; then
    echo "Error: CARDAMOM_RUN_MODEL did not compile Sucessfully. Aborting."
    exit 1
fi

echo ${COMPILER} ${CARDAMOM_C_PATH}/projects/CARDAMOM_MDF/CARDAMOM_MDF.c -o ${CARDAMOM_C_PATH}/projects/CARDAMOM_MDF/CARDAMOM_MDF.exe -lm ${NETCDF_LIB_FLAGS} 

${COMPILER} ${CARDAMOM_C_PATH}/projects/CARDAMOM_MDF/CARDAMOM_MDF.c -o ${CARDAMOM_C_PATH}/projects/CARDAMOM_MDF/CARDAMOM_MDF.exe -lm ${NETCDF_LIB_FLAGS}
if [ $? -ne 0 ]; then
    echo "Error: CARDAMOM_MDF did not compile Sucessfully. Aborting."
    exit 1
fi

# ---------------------------------------------------------
# DEBUG BUILD (-d)
# ---------------------------------------------------------
if [[ ! -z "${DEBUG}" ]]; then
  echo ${COMPILER} ${CARDAMOM_C_PATH}/projects/CARDAMOM_GENERAL/CARDAMOM_RUN_MODEL.c -g -o ${CARDAMOM_C_PATH}/projects/CARDAMOM_GENERAL/CARDAMOM_RUN_MODEL_debug.exe -lm ${NETCDF_LIB_FLAGS}
  
  ${COMPILER} ${CARDAMOM_C_PATH}/projects/CARDAMOM_GENERAL/CARDAMOM_RUN_MODEL.c -g -o ${CARDAMOM_C_PATH}/projects/CARDAMOM_GENERAL/CARDAMOM_RUN_MODEL_debug.exe -lm ${NETCDF_LIB_FLAGS}
  if [ $? -ne 0 ]; then
      echo "Error: CARDAMOM_RUN_MODEL_debug did not compile Sucessfully. Aborting."
      exit 1
  fi

  echo ${COMPILER} ${CARDAMOM_C_PATH}/projects/CARDAMOM_MDF/CARDAMOM_MDF.c -g -ggdb3 -o ${CARDAMOM_C_PATH}/projects/CARDAMOM_MDF/CARDAMOM_MDF_debug.exe -lm ${NETCDF_LIB_FLAGS}
  
  ${COMPILER} ${CARDAMOM_C_PATH}/projects/CARDAMOM_MDF/CARDAMOM_MDF.c -g -ggdb3 -o ${CARDAMOM_C_PATH}/projects/CARDAMOM_MDF/CARDAMOM_MDF_debug.exe -lm ${NETCDF_LIB_FLAGS}
  if [ $? -ne 0 ]; then
      echo "Error: CARDAMOM_MDF_debug did not compile Sucessfully. Aborting."
      exit 1
  fi
fi

# ---------------------------------------------------------
# SANITIZE BUILD (-s)
# ---------------------------------------------------------
if [[ ! -z "${SANITIZE}" ]]; then
  SAN_FLAGS="-g -O0 -Wall -fsanitize=address,undefined -fno-omit-frame-pointer"
  
  echo ${COMPILER} ${CARDAMOM_C_PATH}/projects/CARDAMOM_GENERAL/CARDAMOM_RUN_MODEL.c ${SAN_FLAGS} -o ${CARDAMOM_C_PATH}/projects/CARDAMOM_GENERAL/CARDAMOM_RUN_MODEL_asan.exe -lm ${NETCDF_LIB_FLAGS}
  
  ${COMPILER} ${CARDAMOM_C_PATH}/projects/CARDAMOM_GENERAL/CARDAMOM_RUN_MODEL.c ${SAN_FLAGS} -o ${CARDAMOM_C_PATH}/projects/CARDAMOM_GENERAL/CARDAMOM_RUN_MODEL_asan.exe -lm ${NETCDF_LIB_FLAGS}
  
  if [ $? -ne 0 ]; then
      echo "Error: CARDAMOM_RUN_MODEL_asan did not compile Sucessfully. Aborting."
      exit 1
  fi

  echo ${COMPILER} ${CARDAMOM_C_PATH}/projects/CARDAMOM_MDF/CARDAMOM_MDF.c ${SAN_FLAGS} -o ${CARDAMOM_C_PATH}/projects/CARDAMOM_MDF/CARDAMOM_MDF_asan.exe -lm ${NETCDF_LIB_FLAGS}
  
  ${COMPILER} ${CARDAMOM_C_PATH}/projects/CARDAMOM_MDF/CARDAMOM_MDF.c ${SAN_FLAGS} -o ${CARDAMOM_C_PATH}/projects/CARDAMOM_MDF/CARDAMOM_MDF_asan.exe -lm ${NETCDF_LIB_FLAGS}
  
  if [ $? -ne 0 ]; then
      echo "Error: CARDAMOM_MDF_asan did not compile Sucessfully. Aborting."
      exit 1
  fi
fi

echo "All requested files compiled sucessfully"