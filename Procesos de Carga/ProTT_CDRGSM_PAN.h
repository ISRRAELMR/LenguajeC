/* /////////////////////////////////////////////////////////////////////////
//
// TELEFONICA MOVILES, S.A. de C.V.
//
// Archivo : ProTT_GSM.h
//
// Autor   : Isrrael Melendez Romero
//
// Lenguaje: Ansi C
//
// Objetivo: Procesar TTFILES de GSM PANAMA y cargarlos a la BD
//
// Historia: 02/Ago/2005 --> Creacion
//           24/Octubre/2007 --> Se Agregaron los campos CVE_CARRIER_LD,CVE_POS_TRAZO_LLAMADA,CVE_FALLA 
//                               y CVE_REFERENCIA_LLAMADA
///////////////////////////////////////////////////////////////////////// */

#define LONG_HEADER_INTF   151
#define MAX_OPE 80000         /* Maximo del arreglo de Operadores   */
#define MAX_PORTADOS 20000000    /* Maximo del arreglo de Portados   */
#define MAX_ROUTES 20000     /*  Maximo del arreglo de las troncales   */
#define MAX_SWITCHS 30000     /*  Maximo del arreglo de los switchs   */
#define MAX_LINE 1000
#define MIN_LINE 300
#define POS_nFrecordType		1
#define POS_nFcallIdentificationCarrier 17   

#define POS_nFcalledPartyNumber		24
#define POS_nFcalledSubscriberIMEI	25
#define POS_nFcalledSubscriberIMSI	26
#define POS_nFcallingPartyNumber	28
#define POS_nFcallingSubscriberIMEI	30
#define POS_nFcallingSubscriberIMSI	31

#define POS_nFcallPosition	        32   


#define POS_nFlastCalledLocationInf     36
#define POS_nFlastCallingLocationInf    37

#define POS_nFchargeableDuration	40
#define POS_nFdateForStartOfCharge	49
#define POS_nFexchangeIdentity		56

#define POS_nfaultCode			57   
	
#define POS_nFincomingRoute		79

#define POS_nFinternalCauseAndLoc       85

#define POS_nFnetworkCallReference	97   


#define POS_nFoutgoingRoute		108
#define POS_nFredirectingIMSI		117
#define POS_nFredirectingNumber		118
#define POS_nFtimeForStartOfCharge	142
#define POS_nFtimeForStopOfCharge	143
#define POS_nFtimeFromRegisterSeizure	145

/* =========================== Dummy de Desvios a Buzon ================*/
#define DUMMY_BUZON      "66101001"


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
#define IMSI_TMM1 "714020"
#define IMSI_TMM2 "714020"

/*============================= Usados para identificar los prefijos  */
/* PREFIJOS de 5 digitos  */
#define PREFIJO5_1 	"91200"
#define PREFIJO5_2 	"01900"
 


/* PREFIJOS de 3 digitos */

#define PREFIJO3_1	"507"
#define PREFIJO3_2	"912"
#define PREFIJO3_3	"820"
#define PREFIJO3_4	"816"


/* PREFIJOS de 2 Digitos  */
#define PREFIJO2_1	"00"
#define PREFIJO2_2	"01" 





#define SQL_LONG_DATE      21

#define BCF_ERRC_CANNOT_OPEN_FILE 1000
#define BCF_ERRC_INCOMPLETE_FIELDS 2000




 
 
 

      











