if(CONFIG_LIBLIGHTMODBUS)

#Get git version (LIGHTMODBUS_GIT_VERSION)
execute_process( COMMAND "git" "describe" "--abbrev=6" "--dirty" "--always" "--tag" RESULT_VARIABLE GIT_EXIT_CODE OUTPUT_VARIABLE PROJECT_GIT_VERSION )
if ( GIT_EXIT_CODE )
	set( PROJECT_GIT_VERSION "no-vcs-found" )
endif( )
string( REGEX REPLACE "\n$" "" PROJECT_GIT_VERSION "${PROJECT_GIT_VERSION}" ) #Strip newline

#Include dirs
zephyr_include_directories(../include)

#List of all slave modules
set( SLAVE_MODULES
	"F01S"
	"F02S"
	"F03S"
	"F04S"
	"F05S"
	"F06S"
	"F15S"
	"F16S"
	"F22S"
	"SLAVE_USER_FUNCTIONS"
	"REGISTER_CALLBACK"

	"COIL_CALLBACK"
)

#List of all master modules
set( MASTER_MODULES
	"F01M"
	"F02M"
	"F03M"
	"F04M"
	"F05M"
	"F06M"
	"F15M"
	"F16M"
	"F22M"
	"MASTER_USER_FUNCTIONS"
	"NO_MASTER_DATA_BUFFER"
	"MASTER_INVASIVE_PARSING"
)

#List of all modules that can be enabled
set( AVAILABLE_MODULES
	"SLAVE_BASE"
	"${SLAVE_MODULES}"
	"MASTER_BASE"
	"${MASTER_MODULES}"

	#Misc
	"EXPERIMENTAL"
)

#List of modules enabled by default
set( DEFAULT_MODULES
	#Slave
	"SLAVE_BASE"
	"F01S" "F02S" "F03S" "F04S" "F05S" "F06S" "F15S" "F16S" "F22S"
	"SLAVE_USER_FUNCTIONS"

	#Master
	"MASTER_BASE"
	"F01M" "F02M" "F03M" "F04M" "F05M" "F06M" "F15M" "F16M" "F22M"
	"MASTER_USER_FUNCTIONS"
)

#Macros to be written to libconf.h
set( LIBCONF "" )

#If module set is not specified, include all known modules
if ( NOT DEFINED MODULES )
	set( MODULES ${DEFAULT_MODULES} )
	message( STATUS "MODULES not set. Enabling deafults.")
endif( )

#Add modules deined in ADD_MODULES
if ( DEFINED ADD_MODULES )
	set( MODULES "${MODULES};${ADD_MODULES}" )
endif( )

#Iterate through modules list
foreach( MODULE ${MODULES} )
	#If that module doesn't exit raise an error
	if ( NOT ${MODULE} IN_LIST AVAILABLE_MODULES )
		message( FATAL_ERROR "`${MODULE}' is not a valid module name. Available modules are: `${AVAILABLE_MODULES}'")
	endif( )

	#If module needs base, but base is not included
	if ( ${MODULE} IN_LIST SLAVE_MODULES AND NOT "SLAVE_BASE" IN_LIST MODULES )
		message( FATAL_ERROR "You seem to be needing ${MODULE} module, but it requires slave base module to be included. Please add SLAVE_BASE to your MODULES list" )
	endif( )
	if ( ${MODULE} IN_LIST MASTER_MODULES AND NOT "MASTER_BASE" IN_LIST MODULES )
		message( FATAL_ERROR "You seem to be needing ${MODULE} module, but it requires master base module to be included. Please add MASTER_BASE to your MODULES list" )
	endif( )

	#Generate proper #define directive
	set( LIBCONF "${LIBCONF}#define LIGHTMODBUS_${MODULE}\n" )
	message( STATUS "Enabling LIGHTMODBUS_${MODULE}" )
endforeach( )

#Fixed size buffers
if ( STATIC_MEM_SLAVE_REQUEST )
	if ( STATIC_MEM_SLAVE_REQUEST LESS 2 )
		message( FATAL_ERROR "STATIC_MEM_SLAVE_REQUEST has to be greater than 1" )
	endif( )
	message( STATUS "Enabling static slave request buffer of size ${STATIC_MEM_SLAVE_REQUEST} bytes" )
	set( LIBCONF "${LIBCONF}#define LIGHTMODBUS_STATIC_MEM_SLAVE_REQUEST ${STATIC_MEM_SLAVE_REQUEST}\n" )
endif( )
if ( STATIC_MEM_SLAVE_RESPONSE )
	if ( STATIC_MEM_SLAVE_RESPONSE LESS 2 )
		message( FATAL_ERROR "STATIC_MEM_SLAVE_RESPONSE has to be greater than 1" )
	endif( )
	message( STATUS "Enabling static slave response buffer of size ${STATIC_MEM_SLAVE_RESPONSE} bytes" )
	set( LIBCONF "${LIBCONF}#define LIGHTMODBUS_STATIC_MEM_SLAVE_RESPONSE ${STATIC_MEM_SLAVE_RESPONSE}\n" )
endif( )
if ( STATIC_MEM_MASTER_REQUEST )
	if ( STATIC_MEM_MASTER_REQUEST LESS 2 )
		message( FATAL_ERROR "STATIC_MEM_MASTER_REQUEST has to be greater than 1" )
	endif( )
	message( STATUS "Enabling static master request buffer of size ${STATIC_MEM_MASTER_REQUEST} bytes" )
	set( LIBCONF "${LIBCONF}#define LIGHTMODBUS_STATIC_MEM_MASTER_REQUEST ${STATIC_MEM_MASTER_REQUEST}\n" )
endif( )
if ( STATIC_MEM_MASTER_RESPONSE )
	if ( STATIC_MEM_MASTER_RESPONSE LESS 2 )
		message( FATAL_ERROR "STATIC_MEM_MASTER_RESPONSE has to be greater than 1" )
	endif( )
	message( STATUS "Enabling static master response buffer of size ${STATIC_MEM_MASTER_RESPONSE} bytes" )
	set( LIBCONF "${LIBCONF}#define LIGHTMODBUS_STATIC_MEM_MASTER_RESPONSE ${STATIC_MEM_MASTER_RESPONSE}\n" )
endif( )
if ( STATIC_MEM_MASTER_DATA )
	if ( STATIC_MEM_MASTER_DATA LESS 2 )
		message( FATAL_ERROR "STATIC_MEM_MASTER_DATA has to be greater than 1" )
	endif( )
	message( STATUS "Enabling static master data buffer of size ${STATIC_MEM_MASTER_DATA} bytes" )
	set( LIBCONF "${LIBCONF}#define LIGHTMODBUS_STATIC_MEM_MASTER_DATA ${STATIC_MEM_MASTER_DATA}\n" )
endif( )

#Library sources list
zephyr_library()
zephyr_library_sources(
	../src/lightmodbus.c
	../src/slave.c
	../src/master.c
	../src/slave/scoils.c
	../src/slave/sregs.c
	../src/master/mpcoils.c
	../src/master/mbcoils.c
	../src/master/mpregs.c
	../src/master/mbregs.c
	)

endif()
