/* ///////////////////////////////////////////////////////////////////////
//
// Telefonica MoviStar S.A. de C.V.
//
// Archivo : ProTT_GSM.h
//
// Autor   : Israel Melendez
//
// Lenguaje: Ansi C
//
// Objetivo: Procesar TTFILES de GSM y cargarlos a la BD
//
// Historia: 30/Jul/2004 --> Creacion
// Actualizacion: 06/Jun/2006 --> Se modifica para tomar en cuenta los campos de fin de llamada
//                19/Septiembre/2007--> Se modifica para tomar en cuenta las Celdas Origen y Destino
//                27/Mayo/2008 --> Se agrega calculo de la red complemento 
//
///////////////////////////////////////////////////////////////////////// */

#define LONG_HEADER_INTF   151
#define MAX_OPE 120000         /* Maximo del arreglo de Operadores   */

#define MAX_PORTADOS 20000000         /* Maximo del arreglo de Portados   */

#define MAX_ROUTES 40000     /*  Maximo del arreglo de las troncales   */
#define MAX_SWITCHS 1000     /*  Maximo del arreglo de los switchs   */
#define MAX_LINE 1000
#define MIN_LINE 300
#define POS_nFrecordType		1

#define POS_nFidentificationCall	16
#define POS_nFcallIdentificationCarrier 17

#define POS_nFcalledPartyNumber		24
#define POS_nFcalledSubscriberIMEI	25
#define POS_nFcalledSubscriberIMSI	26
#define POS_nFcallingPartyNumber	28
#define POS_nFcallingSubscriberIMEI	30
#define POS_nFcallingSubscriberIMSI	31

#define POS_nFcallPosition	        32

#define POS_nFfirstCallingLocationInf   35
#define POS_nFlastCalledLocationInf     36
#define POS_nFlastCallingLocationInf    37


#define POS_nFchargeableDuration	40
#define POS_nFdateForStartOfCharge	49
#define POS_nFexchangeIdentity		56

#define POS_nfaultCode			57
	
#define POS_nFincomingRoute		79

#define POS_nFinternalCausaAndLoc	85
#define POS_nFnetworkCallReference	97

#define POS_nForiginatingLocation	105

#define POS_nFoutgoingRoute		108
#define POS_nFredirectingIMSI		117
#define POS_nFredirectingNumber		118

#define POS_nFterminatingLocation	141

#define POS_nFtimeForStartOfCharge	142
#define POS_nFtimeForStopOfCharge	143
#define POS_nFtimeFromRegisterSeizure	145
#define POS_nVPN                      169

/*=============================   Tipos de Switch    */
#define SWITCH_1	"GGUA2"
#define SWITCH_2	"GMEX2"
#define SWITCH_3	"GTIJ2"
#define SWITCH_4	"GMTY2"
#define SWITCH_5	"GJUA2"
#define SWITCH_6	"GPUE2"
#define SWITCH_7	"GLEO2"
#define SWITCH_8	"GLPZ2"
#define SWITCH_8_B	"GPAZ2"
#define SWITCH_9	"GHER2"
#define SWITCH_9_B	"GHMO2"
#define SWITCH_10	"GCUL2"
#define SWITCH_11	"GMER2"
#define SWITCH_12	"GMAT2"
#define SWITCH_13	"GME22"
#define SWITCH_14	"GMY22"
#define SWITCH_15	"GME32"
#define SWITCH_16	"GME42"
#define SWITCH_17	"GAGS2"
#define SWITCH_18	"GVER2"



/*============================   tipos de registro ENTRANTE_SALIENTE    */
#define T_ENTRANTE	"04"
#define T_SALIENTE	"01"
#define T_DESVIOS	"03"
#define T_SMS_SALIENTES	"05"
#define T_SMS_ENTRANTES	"07"
#define T_DESVIOS_BUZON	"13"

/*============================== Usados para identificar Llamadas locales ó de larga distancia  */
#define T_ENTRANTE_LOCAL "ELO"	
#define	T_SALIENTE_LOCAL "SLO"
#define T_ENTRANTE_LD    "ELD"
#define T_SALIENTE_LD    "SLD"

/*============================= Usados para identificar si el telefono es nacional ó Extranjero */
#define IMSI_TMM1 "334030"
#define IMSI_TMM2 "334020"


/*============================= Usados para identificar los NIR de Monterrey, Mexico y Guadalajara */
#define NIR_GUA "33"
#define NIR_MEX "55"
#define NIR_MTY "81"


/*============================= Usados para identificar los prefijos  */
/* PREFIJOS de 5 dijitos  */
#define PREFIJO5_1 	"01800"
#define PREFIJO5_2 	"01900"

/* PREFIJOS de 3 dijitos */
#define PREFIJO3_1	"044"
#define PREFIJO3_2	"045"

/* PREFIJOS de 2 Dijitos  */
#define PREFIJO2_1	"00"
#define PREFIJO2_2	"01"



#define SQL_LONG_DATE      21

#define BCF_ERRC_CANNOT_OPEN_FILE 1000
#define BCF_ERRC_INCOMPLETE_FIELDS 2000












