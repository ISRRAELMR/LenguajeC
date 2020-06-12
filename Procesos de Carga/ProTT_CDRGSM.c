/* //////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// 
//
// Archivo : ProTT_CDRGSM.c
//
// Autor   : Israel Melendez Romero
//
// Lenguaje: Ansi C
//
// Objetivo: Procesar archivos TTFiles de GSM y cargar la información a la tabla CDR_GSM
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////// */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ProTT_CDRGSM.h"
#include <floatingpoint.h>
#include <math.h>

int index_buff = 0;
int index_op = 0;
int idx_routes=0;
int idx_switchs=0;
long long idx_portados=0;
int NIVEL_TRACE = 7;

struct node_routes {
        char cve_msc[6];
	char cve_ruta[15];
	char cve_ne[6];
};

struct node_portados {
        long long dn;
	long long fch_inicio;
	long long fch_fin;
        char id_red[5];
};

struct node_switchs {
        char sswitch[10];
	char cve_switch[3];
};

struct nlist {
	long long lldn_min;
	long long lldn_max;
        char id_red[5];
};

static struct nlist *operadores[MAX_OPE];
static struct node_portados *portados[MAX_PORTADOS];

void llOperador(long long lloper,char *out_dn_min,char *out_id_red);

static struct node_routes *routes[MAX_ROUTES];

static struct node_switchs *switches[MAX_SWITCHS];

void strtok3(char *in_strtok3,char caracter,char *out_strtok3);
void strtok2(char *in_strtok2,char caracter,char *out_strtok2);
void strstr2(char *in_strstr2,int start, int end,char *out_strstr2);
char find_ld(char *v_cve_msc,char *v_cve_ruta);
void DelBlk(char *in_DelBlk,char *out_DelBlk);
void calculate_duration(char *v_time_init,char *v_time_end,char *out_duration);
char find_prefijo(char v_num_called[20]);
void GetLast10(char *in_GetLast10,char *out_GetLast10);
void Reverse(char *sourc);
char find_roaming(char *dn_temm,char *location,char v_cve_IMSI[20]);
void calculate_switch(char *v_switch,char *out_switch);

void find_red(long long in_dn,long long in_fecha,char *out_id_red);


/* ///////////////////////////////////////////////////////////////////////
// 
// ProTT_CDRGSM
//
/////////////////////////////////////////////////////////////////////// */


int main(int argc, char *argv[])
{
  char filename  [ 80 ],
       filename2 [ 80 ],
       file_op   [ 80 ],
       file_routes [ 80 ],
       file_switchs [ 80 ],
       file_portados [ 80 ],
       v_fch_insercion [20],
       hora_carga[5];

  char car;
  char buff[MAX_LINE];
  char buff_out[MIN_LINE];
  int  p = 0;
/*  int  result  = GFI_ERRC_OK;  */
  long db_error  = 0,
       rownum    = 0;
  char vtoken[100];
  char vtoken_tmp[100];
  char vtoken2[20];
  char vtoken3[20];
  char vtoken4[20];
  char vtoken5[20];

  FILE *output;  FILE *source;
  FILE *rangos;
  FILE *troncales;
  FILE *switchs;
  FILE *a_portados;

  char buff_op[100];
  char buff_routes[100];
  char buff_switchs[50];
  char buff_portados[200];

  char record_type[5] = "\0"; 
  char n_called[40] = "\0";		
  char n_called_IMEI[25] = "\0";	
  char n_called_IMSI[25] = "\0";	
  char n_calling[40] = "\0";	
  char n_calling_IMEI[25] = "\0";	
  char n_calling_IMSI[25] = "\0";	
  char duration[15] = "\0";	
  char date[10] = "\0";	
  char nswitch[15] = "\0";			
  char route_out[20] = "\0";	
  char route_in[20] = "\0";		
  char redirecting_IMSI[25] = "\0";		
  char n_redirecting[25] = "\0";		
  char time_start[15] = "\0";	
  char time_stop[15] = "\0";	
  char register_seizure[15] = "\0";
/* falta VPNstr */
  char v_vpn[3]="\0";
  char v_location_information[25]="\0";
  char first_calling_location[25]="\0";
  char last_called_location[25]="\0";
  char last_calling_location[25]="\0";


  char call_position[20]="\0";
  char carrier_ld[20]="\0";
  char fault_code[20]="\0";
  char cause_call[20]="\0";
  char reference_call[20]="\0";
  char ttfile[25]="\0";
  char identification_call[20]="\0"; 
  char originating_location[25]="\0";
  char terminating_location[25]="\0";


  struct node_routes *np;
  struct node_switchs *np_switchs;
  struct nlist *np_op;
  struct node_portados *np_portados;
  long long v_calcula, v_resultado, v_calcula2=0;
  char v_red[5]="\0";b
  char v_red_op[5]="\0";
  char v_red_portado[5]="\0";

  int long_num=0;
  int long_num2=0;
  int duracion=0,
      duracion_register=0;

  char v_datetime[32] = "\0";
  char v_date[10]="\0";
  char v_switch[5] = "\0";
  char v_dn_tmm[22] = "\0";
  char v_dn_complemento[22] = "\0";
  char v_dn[30] = "\0";
  char v_dn2[30] = "\0";
  char v_entrante_saliente[5] = "\0";
  char v_local_ld;         
  char v_tipo_roaming='\0';
  char v_duracion_seg[8]="\0";
  char v_prefijo='\0';
  char v_dnmin_tmm[20]="\0";
  char v_dnmin_complemento[20]="\0";


  char v_imsi_tmm[25];
  char v_imei_tmm[25];
  char v_duracion_answer[8]="\0";
  char v_duracion_send[8]="\0";  

  char day[4]="\0";
  char month[6]="\0";
  char year[6]="\0";
  char seconds[4]="\0";
  char minutes[4]="\0";
  char hours[4]="\0";

  char hours_answer[4]="\0";
  char minutes_answer[4]="\0";
  char seconds_answer[4]="\0";

  char hours_send[4]="\0";
  char minutes_send[4]="\0";
  char seconds_send[4]="\0";

  sprintf(vtoken,"%c",'\0');
 
  if (argc != 9)
  {
    fprintf(stderr, "Sintaxis: ProTT_CDRGSM arch_fuente arch_destino arch_operadores arch_troncales arch_switchs arch_portados fch_insercion hora_carga  \n");
    exit (3);
  }
  
  strcpy (filename, argv [ 1 ]);
  strcpy (filename2, argv [ 2 ]);  
  strcpy (file_op, argv [ 3 ]);  
  strcpy (file_routes, argv [4]);
  strcpy (file_switchs, argv [5]);
  strcpy (file_portados, argv [6]);
  strcpy (v_fch_insercion, argv [7]);
  strcpy (hora_carga, argv [8]);

    fprintf (stderr, "Entrando a la funcion ProTT_CDRGSM \n");
    fflush  (stderr);

    if (!(output = fopen (filename2, "wt")))
    {
      fprintf (stderr, 
	       "El archivo [%s] no pudo ser abierto para escritura \n",
	       filename2);
      fflush  (stderr);
      exit(3);
    }

    if (!(source = fopen (filename, "r")))
    {
      fprintf (stderr, 
	       "El archivo [%s] no pudo ser abierto para lectura \n",
	       filename);
      fflush  (stderr);
      exit(3);	
    }


    if (!(rangos = fopen (file_op, "r")))
    {
      fprintf (stderr, 
	       "El archivo [%s] no pudo ser abierto para lectura \n",
	       file_op);
      fflush  (stderr);
      exit(3);	
    }


    if (!(troncales = fopen (file_routes, "r")))
    {
      fprintf (stderr, 
	       "El archivo [%s] no pudo ser abierto para lectura \n",
	       file_routes);
      fflush  (stderr);
      exit(3);	
    }


    if (!(switchs = fopen (file_switchs, "r")))
    {
      fprintf (stderr, 
	       "El archivo [%s] no pudo ser abierto para lectura \n",
	       file_switchs);
      fflush  (stderr);
      exit(3);	
    }



    if (!(a_portados = fopen (file_portados, "r")))
    {
      fprintf (stderr, 
	       "El archivo [%s] no pudo ser abierto para lectura \n",
	       file_portados);
      fflush  (stderr);
      exit(3);	
    }



/* se cargan los switchs en un arreglo  */

    sprintf(buff_switchs,"%c",'\0');

    while(fgets(buff_switchs,50,switchs))
    { sprintf(vtoken2,"%c",'\0');
      sprintf(vtoken3,"%c",'\0');      
      index_buff=0; 
      strtok3(buff_switchs,',',vtoken2);
      strtok3(buff_switchs,',',vtoken3);
      np_switchs = (struct node_switchs *) malloc (sizeof(*np_switchs));
      strcpy(np_switchs->sswitch,vtoken2);  
      strcpy(np_switchs->cve_switch,vtoken3);
      switches[idx_switchs] = np_switchs;
      idx_switchs++;
      sprintf(buff_switchs,"%c",'\0');
    }    /* While de lectura de archivo   */ 


  /*   se carga los campos CVE_RUTA y CVE_NE del archivo troncales  al arreglo "routes"     */ 

    sprintf(buff_routes,"%c",'\0');

    while(fgets(buff_routes,100,troncales))
    { sprintf(vtoken2,"%c",'\0');
      sprintf(vtoken3,"%c",'\0');      
      sprintf(vtoken4,"%c",'\0');      
      index_buff=0;
      strtok3(buff_routes,',',vtoken4);
      strtok3(buff_routes,',',vtoken2);
      strtok3(buff_routes,',',vtoken3);
      np = (struct node_routes *) malloc (sizeof(*np));
      strcpy(np->cve_msc,vtoken4);  
      strcpy(np->cve_ruta,vtoken2);  
      strcpy(np->cve_ne,vtoken3);
      routes[idx_routes] = np;
      idx_routes++;
      sprintf(buff_routes,"%c",'\0');
    }    /* While de lectura de archivo   */ 


 /*  subimos el archivo de operadores a un arreglo estatico   */

    sprintf(buff_op,"%c",'\0');
    while(fgets(buff_op,100,rangos))
    { 
      sprintf(vtoken2,"%c",'\0');
      sprintf(vtoken3,"%c",'\0'); 
      sprintf(vtoken4,"%c",'\0');      
      index_buff=0;
      strtok3(buff_op,',',vtoken2);
      strtok3(buff_op,',',vtoken3);
      strtok3(buff_op,',',vtoken4);
      np_op = (struct nlist *) malloc (sizeof(*np_op));
      np_op->lldn_min = atoll(vtoken2);  
      np_op->lldn_max = atoll(vtoken3);
      strcpy(np_op->id_red,vtoken4);
      operadores[index_op] = np_op;
      index_op++;      
      sprintf(buff_op,"%c",'\0');
    }    /* While de lectura de archivo   */ 


 /*  subimos el archivo de portados a un arreglo estatico   */

    sprintf(buff_portados,"%c",'\0');
    while(fgets(buff_portados,100,a_portados))
    { 
      sprintf(vtoken2,"%c",'\0');
      sprintf(vtoken3,"%c",'\0');      
      sprintf(vtoken4,"%c",'\0');
      sprintf(vtoken5,"%c",'\0');
      index_buff=0;
      strtok3(buff_portados,'|',vtoken2);
      strtok3(buff_portados,'|',vtoken3);
      strtok3(buff_portados,'|',vtoken4);
      strtok3(buff_portados,'|',vtoken5);
      np_portados = (struct node_portados *) malloc (sizeof(*np_portados));
      np_portados->dn = atoll(vtoken2);  
      np_portados->fch_inicio = atoll(vtoken3);
      np_portados->fch_fin = atoll(vtoken4);
      strcpy(np_portados->id_red,vtoken5);
      portados[idx_portados] = np_portados;
      idx_portados++;      
      sprintf(buff_portados,"%c",'\0');
    }    /* While de lectura de archivo   */ 



/*  Procesa registros de archivo de entrada   */
    while(fgets(buff,MAX_LINE,source))
      { 
        p=1;
        index_buff=0;
        /* Modificacion usando la funcion strtok  */      
        sprintf(vtoken,"%c",'\0');
        sprintf(vtoken_tmp,"%c",'\0');
        strtok2(buff,',',vtoken_tmp);
        while(p < 170)
        { 
          DelBlk(vtoken_tmp,vtoken);
          if(p == POS_nFrecordType) 
        	strcpy(record_type,vtoken);
	  else if(p==POS_nFidentificationCall)	
		strcpy(identification_call,vtoken);
	  else if(p==POS_nFcallIdentificationCarrier)	
		strcpy(carrier_ld,vtoken);
 	  else if(p==POS_nFcalledPartyNumber)
		{
		if(strlen(vtoken)>19)
		   strstr2(vtoken,0,18,n_called);
                else
		   strcpy(n_called,vtoken);
		}
	  else if(p==POS_nFcalledSubscriberIMEI)	
		strcpy(n_called_IMEI,vtoken);
	  else if(p==POS_nFcalledSubscriberIMSI)	
		strcpy(n_called_IMSI,vtoken);
	  else if(p==POS_nFcallingPartyNumber)	
		{
		if(strlen(vtoken)>19)
		   strstr2(vtoken,0,18,n_calling);		
                else
		   strcpy(n_calling,vtoken);
		}
	  else if(p==POS_nFcallingSubscriberIMEI)	
		strcpy(n_calling_IMEI,vtoken);
	  else if(p==POS_nFcallingSubscriberIMSI)	
		strcpy(n_calling_IMSI,vtoken);


          else if(p==POS_nFfirstCallingLocationInf)
                strcpy(first_calling_location,vtoken);
          else if(p==POS_nFlastCalledLocationInf)
                strcpy(last_called_location,vtoken);
          else if(p==POS_nFlastCallingLocationInf)
                strcpy(last_calling_location,vtoken);



          else if(p==POS_nFcallPosition)
		strcpy(call_position,vtoken);
	  else if(p==POS_nFchargeableDuration)	
		strcpy(duration,vtoken);
	  else if(p==POS_nFdateForStartOfCharge)	
		strcpy(date,vtoken);
	  else if(p==POS_nFexchangeIdentity)			
		strcpy(nswitch,vtoken);
	  else if(p==POS_nfaultCode)			
		strcpy(fault_code,vtoken);
	  else if(p==POS_nFincomingRoute)		
		strcpy(route_in,vtoken);
	  else if(p==POS_nFinternalCausaAndLoc)		
		strcpy(cause_call,vtoken);
	  else if(p==POS_nFnetworkCallReference)		
		strcpy(reference_call,vtoken);
	  else if(p==POS_nForiginatingLocation)		
		strcpy(originating_location,vtoken);
	  else if(p==POS_nFoutgoingRoute)	
		strcpy(route_out,vtoken);
	  else if(p==POS_nFredirectingIMSI)		
		strcpy(redirecting_IMSI,vtoken);
	  else if(p==POS_nFredirectingNumber)		
		strcpy(n_redirecting,vtoken);
	  else if(p==POS_nFterminatingLocation)		
		strcpy(terminating_location,vtoken);
	  else if(p==POS_nFtimeForStartOfCharge)	
	     	strcpy(time_start,vtoken);
 	  else if(p==POS_nFtimeForStopOfCharge)	
              	strcpy(time_stop,vtoken);  /* inicio anexo */
          else if (p==POS_nVPN)
               {
    	         strcpy(v_vpn,vtoken);
                 v_vpn[1]='\0'; 
               }
	  else if(p==POS_nFtimeFromRegisterSeizure)	/* fin anexo */
		strcpy(register_seizure,vtoken);
          sprintf(vtoken,"%c",'\0');
          sprintf(vtoken_tmp,"%c",'\0');
          strtok2(buff,',',vtoken_tmp);
            p++;
        }

	if(p < POS_nFtimeFromRegisterSeizure)
  	{
 	  fprintf (stderr,"El archivo fuente: [%s] no contiene todos los campos necesarios usados para la carga \n",
	       filename);
          fflush  (stderr);
	  exit(3);
        }


    /*	sprintf(v_datetime,"%s%s%s%s%c%s%c%s%c%s%c",day,month,"20",year,' ',  */
	/*	hours,':',minutes,':',seconds,'\0');      */

  	sprintf(v_datetime,"%s%s%s%c","20",date,time_start,'\0'); 
        sprintf(v_date,"%s%s%c","20",date,'\0');     



  /*   Se clasifica el switch        */

        calculate_switch(nswitch,v_switch);



  /*   Se elige el numero de TMM y el DN_Complemento, El CAMPO ENTRANTE_SALIENTE, Local_LD     */
        
       if(strcmp(record_type,T_ENTRANTE)==0)
         {  if(strlen(n_called) > 10)
	      strstr2(n_called,(strlen(n_called)-10),(strlen(n_called)-1),v_dn_tmm);
            else
              strcpy(v_dn_tmm,n_called);
	    if(strlen(n_calling) > 10)
  	      strcpy(v_dn_complemento,n_calling);
            else
	      strcpy(v_dn_complemento,n_calling);
	    strcpy(v_entrante_saliente,"1");
            strcpy(v_imsi_tmm,n_called_IMSI);
            strcpy(v_imei_tmm,n_called_IMEI);
            v_local_ld=find_ld(nswitch,route_in);
            v_tipo_roaming=find_roaming(n_called,terminating_location,n_called_IMSI);
	  }
       else if(strcmp(record_type,T_SALIENTE)==0)
         {  if(strlen(n_calling) > 10)
	      strstr2(n_calling,(strlen(n_calling)-10),(strlen(n_calling)-1),v_dn_tmm);
            else
              strcpy(v_dn_tmm,n_calling);
	    if(strlen(n_called) > 10)
  	      strcpy(v_dn_complemento,n_called);
            else
	      strcpy(v_dn_complemento,n_called);
	    strcpy(v_entrante_saliente,"2");
            strcpy(v_imsi_tmm,n_calling_IMSI);
            strcpy(v_imei_tmm,n_calling_IMEI);
            v_local_ld=find_ld(nswitch,route_out);
	    v_tipo_roaming=find_roaming(n_calling,originating_location,n_calling_IMSI);
         }
       else if(strcmp(record_type,T_DESVIOS)==0)
	 { if(strlen(n_called) > 10)
	      strstr2(n_called,(strlen(n_called)-10),(strlen(n_called)-1),v_dn_tmm);
            else
              strcpy(v_dn_tmm,n_called);
	    if(strlen(n_calling) > 10)
  	      strcpy(v_dn_complemento,n_calling);
            else
	      strcpy(v_dn_complemento,n_calling);
	   strcpy(v_entrante_saliente,"3");
            strcpy(v_imsi_tmm,n_called_IMSI);
            strcpy(v_imei_tmm,n_called_IMEI);
           v_local_ld=find_ld(nswitch,route_in);
	   /* v_tipo_roaming=find_roaming(n_called_IMSI);*/
           v_tipo_roaming='0';
	 }
       else if(strcmp(record_type,T_SMS_SALIENTES)==0)
	 { if(strlen(n_calling) > 10)
	      strstr2(n_calling,(strlen(n_calling)-10),(strlen(n_calling)-1),v_dn_tmm);
            else
              strcpy(v_dn_tmm,n_calling);
	    if(strlen(n_called) > 10)
  	      strcpy(v_dn_complemento,n_called);
            else
	      strcpy(v_dn_complemento,n_called);
	    strcpy(v_entrante_saliente,"5");
            strcpy(v_imsi_tmm,n_calling_IMSI);
            strcpy(v_imei_tmm,n_calling_IMEI);
           v_local_ld=find_ld(nswitch,route_out);
	   v_tipo_roaming=find_roaming(n_calling,originating_location,n_calling_IMSI);
	 }
       else if(strcmp(record_type,T_SMS_ENTRANTES)==0)
	 { if(strlen(n_called) > 10)
	      strstr2(n_called,(strlen(n_called)-10),(strlen(n_called)-1),v_dn_tmm);
            else
              strcpy(v_dn_tmm,n_called);
	    if(strlen(n_calling) > 10)
  	      strcpy(v_dn_complemento,n_calling);
            else
	      strcpy(v_dn_complemento,n_calling);
	   strcpy(v_entrante_saliente,"7");
            strcpy(v_imsi_tmm,n_called_IMSI);
            strcpy(v_imei_tmm,n_called_IMEI);
            v_local_ld=find_ld(nswitch,route_in);
	    v_tipo_roaming=find_roaming(n_called,terminating_location,n_called_IMSI);
	 }
       else if(strcmp(record_type,T_DESVIOS_BUZON)==0)
	 { if(strlen(n_called) > 10)
	      strstr2(n_called,(strlen(n_called)-10),(strlen(n_called)-1),v_dn_tmm);
            else
              strcpy(v_dn_tmm,n_called);
	    if(strlen(n_calling) > 10)
  	      strcpy(v_dn_complemento,n_calling);
            else
	      strcpy(v_dn_complemento,n_calling);
	   strcpy(v_entrante_saliente,"13");
            strcpy(v_imsi_tmm,n_called_IMSI);
            strcpy(v_imei_tmm,n_called_IMEI);
           v_local_ld=find_ld(nswitch,route_in);
	 /*  v_tipo_roaming=find_roaming(n_called_IMSI);  */
           v_tipo_roaming='0'; 
	 }
        else  
          { if(strlen(n_called) > 10)
	      strstr2(n_called,(strlen(n_called)-10),(strlen(n_called)-1),v_dn_tmm);
            else
              strcpy(v_dn_tmm,n_called);
	    if(strlen(n_calling) > 10)
  	      strcpy(v_dn_complemento,n_calling);
            else
	      strcpy(v_dn_complemento,n_calling);
	    strcpy(v_entrante_saliente,"0");
            strcpy(v_imsi_tmm,n_called_IMSI);
            strcpy(v_imei_tmm,n_called_IMEI);
            v_local_ld=find_ld(nswitch,route_in);
            v_tipo_roaming=find_roaming(n_called,terminating_location,n_called_IMSI);
	  }


/* Obtenemos la duracion de la llamada     */

     calculate_duration(time_start,time_stop,v_duracion_seg); 


/* Se obtiene el Prefijo de marcación    */
     v_prefijo=find_prefijo(n_called);


/* Se obtiene el dn_min de DN_TMM y DN_COMPLEMENTO   */

      strcpy(v_dn,v_dn_tmm);
      sprintf(v_red_op,"%c",'\0');
      sprintf(v_dnmin_tmm,"%c",'\0');
       long_num = strlen(v_dn);
       if(long_num >= 10) 
        { if(long_num == 10)
            v_calcula=atoll(v_dn);  
          else
            {
            strstr2(v_dn,long_num-10,long_num-1,v_dn2);
            v_calcula = atoll(v_dn2);
            }
          llOperador(v_calcula,v_dnmin_tmm,v_red_op);
        }
       sprintf(v_red_op,"%c",'\0');
       sprintf(v_dnmin_complemento,"%c",'\0');
       strcpy(v_dn,v_dn_complemento);
       long_num = strlen(v_dn);
       if(long_num >= 10) 
        { if(long_num == 10)
            v_calcula=atoll(v_dn);  
          else
            {
            strstr2(v_dn,long_num-10,long_num-1,v_dn2);
            v_calcula = atoll(v_dn2);
            }
         llOperador(v_calcula,v_dnmin_complemento,v_red_op);
        }


/*======  Obtenemos la cve_red_complemento si el dn_complemento es portado solo cuando el trafico es entrante ======*/

/*       if(strcmp(record_type,T_ENTRANTE)==0)  */
/*         {    				*/
          v_calcula2=atoll(v_date);
          strcpy(v_dn,v_dn_complemento);
          long_num = strlen(v_dn);  
          if(long_num >= 10)
           { 
             if(long_num == 10)
               v_calcula=atoll(v_dn);  
             else
             {
               strstr2(v_dn,long_num-10,long_num-1,v_dn2);
               v_calcula = atoll(v_dn2);
             }

            find_red(v_calcula,v_calcula2,v_red_portado);
          
           }


/*          }    */


if(strcmp(v_red_portado,"\0")!=0)
  strcpy(v_red,v_red_portado);
else
  strcpy(v_red,v_red_op);             

/*===== Fin de busqueda de RED_ORIGEN =======*/


/*Se reporta la first_calling_location o last_called_location dependiendo del sentido de la llamada*/
if (strcmp(record_type,T_SALIENTE)==0)
  strcpy(v_location_information,first_calling_location);
else
  strcpy(v_location_information,last_called_location);


/* se obtiene duracion_answer y duracion_send  */
   if(strlen(duration)>=6)
     {
      strstr2(duration,0,1,hours_answer);
      strstr2(duration,2,3,minutes_answer);
      strstr2(duration,4,5,seconds_answer);
      duracion=((atoi(hours_answer)*3600) +(atoi(minutes_answer)*60)+atoi(seconds_answer) );
      }
    else duracion=0;
    sprintf(v_duracion_answer,"%d%c",duracion,'\0');
    
    if(strlen(register_seizure)>=6)
     {
      strstr2(register_seizure,0,1,hours_send);
      strstr2(register_seizure,2,3,minutes_send);
      strstr2(register_seizure,4,5,seconds_send);

      duracion_register=((atoi(hours_send)*3600)+(atoi(minutes_send)*60)+atoi(seconds_send) );

     }
    else duracion_register=0;
    sprintf(v_duracion_send,"%d%c",(duracion+duracion_register),'\0');
  
   

/* se llena el buffer de salida   */                                                                                       
     sprintf(buff_out,"%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%c%c%c%c%s%c%c%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s%c%s",
v_datetime,',',
v_switch,',',
v_dn_tmm,',',
v_dn_complemento,',',
hora_carga,',',
route_in,',',
route_out,',',
v_entrante_saliente,',',
v_local_ld,',',
v_tipo_roaming,',',
v_duracion_seg,',',
v_prefijo,',',
v_dnmin_tmm,',',
v_dnmin_complemento,',',
v_fch_insercion,',',
v_imsi_tmm,',',
v_imei_tmm,',',
v_duracion_answer,',',
v_duracion_send,',',
carrier_ld,',',
call_position,',',
fault_code,',',
cause_call,',',
reference_call,',',
ttfile,',', 
identification_call,',',
v_location_information,',',
last_calling_location,',',
v_red,',',
v_vpn
);
     fputs(buff_out,output);
     putc('\n',output);


/* limpiamos las variables */

     sprintf(record_type,"%c",'\0');
     sprintf(n_called,"%c",'\0');
     sprintf(n_called_IMEI,"%c",'\0');
     sprintf(n_called_IMSI,"%c",'\0');
     sprintf(n_calling,"%c",'\0');
     sprintf(n_calling_IMEI,"%c",'\0');
     sprintf(n_calling_IMSI,"%c",'\0');
     sprintf(duration,"%c",'\0');
     sprintf(date,"%c",'\0');
     sprintf(nswitch,"%c",'\0');
     sprintf(route_out,"%c",'\0');
     sprintf(route_in,"%c",'\0');
     sprintf(redirecting_IMSI,"%c",'\0');
     sprintf(n_redirecting,"%c",'\0');
     sprintf(time_start,"%c",'\0');
     sprintf(time_stop,"%c",'\0');
     sprintf(register_seizure,"%c",'\0');

     sprintf(buff_out,"%c",'\0');
     sprintf(buff,"%c",'\0');
     sprintf(v_datetime,"%c",'\0');
     sprintf(v_date,"%c",'\0');
     sprintf(v_switch,"%c",'\0');
     sprintf(v_entrante_saliente,"%c",'\0');
     v_local_ld='\0';
     v_tipo_roaming='\0';
     sprintf(v_duracion_seg,"%c",'\0');
     v_prefijo='\0';
/*     sprintf(v_fch_insercion,"%c",'\0');  */
     sprintf(v_imsi_tmm,"%c",'\0');
     sprintf(v_imei_tmm,"%c",'\0');
     sprintf(v_duracion_answer,"%c",'\0');
     sprintf(v_duracion_send,"%c",'\0');
     sprintf(v_dnmin_tmm,"%c",'\0');
     sprintf(v_dnmin_complemento,"%c",'\0');
     sprintf(v_dn,"%c",'\0');
     sprintf(v_dn_tmm,"%c",'\0');
     sprintf(v_dn_complemento,"%c",'\0');
     
     sprintf(day,"%c",'\0');
     sprintf(month,"%c",'\0');
     sprintf(year,"%c",'\0');
     sprintf(seconds,"%c",'\0');
     sprintf(minutes,"%c",'\0');
     sprintf(hours,"%c",'\0');

     sprintf(call_position,"%c",'\0');
     sprintf(carrier_ld,"%c",'\0');
     sprintf(fault_code,"%c",'\0');
     sprintf(cause_call,"%c",'\0');
     sprintf(reference_call,"%c",'\0');
     sprintf(ttfile,"%c",'\0');
     sprintf(identification_call,"%c",'\0');

     sprintf(originating_location,"%c",'\0');
     sprintf(terminating_location,"%c",'\0');
     
     sprintf(last_called_location,"%c",'\0');
     sprintf(last_calling_location,"%c",'\0');
     sprintf(v_location_information,"%c",'\0');
     sprintf(first_calling_location,"%c",'\0');

     sprintf(v_red,"%c",'\0');
     sprintf(v_red_op,"%c",'\0');
     sprintf(v_red_portado,"%c",'\0');


      }   /*  Fin While(fgets(source))   */



  fflush (output);        
  fclose (output);
  fflush (source);
  fclose (source);


    fprintf (stdout, "Saliendo de ProTT_CDRGSM \n");
    /* fflush (stderr); */
    return(0);


}   /*################ fin del programa  ##########################*/



/*  funcion que regresa los campos de la cadena fuente aun si son nulos no se brinca los campos   */




void strtok2(char *in_strtok2,char caracter,char *out_strtok2)
{
  *out_strtok2='\0';
  while(in_strtok2[index_buff] != caracter && in_strtok2[index_buff] != '\n' && in_strtok2[index_buff] != '\0')
  { 
    sprintf(out_strtok2,"%s%c%c",out_strtok2,in_strtok2[index_buff],'\0');
    index_buff++;
   }
  if (!(in_strtok2[index_buff]=='\n' || in_strtok2[index_buff] == '\0'))  
      /* printf("Fin de linea \n"); 
  else */ {  
      index_buff++; 
       }  
}




void strtok3(char *in_strtok3,char caracter,char *out_strtok3)
{
  *out_strtok3='\0';
  while(in_strtok3[index_buff] != caracter && in_strtok3[index_buff] != '\n' && in_strtok3[index_buff] != '\0')
  { 
    sprintf(out_strtok3,"%s%c%c",out_strtok3,in_strtok3[index_buff],'\0');
    index_buff++;
   }
      index_buff++; 
}



/*   funcion que obtiene subcadenas de una cadena fuente, se proporciona los indices de la subcadena    */



void strstr2(char *in_strstr2,int start, int end,char *out_strstr2)
{
  int i;
  i=start;
  *out_strstr2='\0';
  while(i<=end && in_strstr2[i] != '\0'&& in_strstr2[i] != '\n')
    {
    if(i>=start && i<=end)
      sprintf(out_strstr2,"%s%c%c",out_strstr2,in_strstr2[i],'\0');
    i++;
    }
}



char find_ld(char *v_cve_msc,char *v_cve_ruta)
{
 int j;
 char v_cve_ne[6]="\0";
 if (strlen(v_cve_ruta) > 0 && strlen(v_cve_msc) > 0) {
 struct node_routes *np;
  for(j=0;j<idx_routes;j++)
    {
      np=routes[j];
      if(strcmp(np->cve_msc,v_cve_msc)==0 && strcmp(np->cve_ruta,v_cve_ruta)==0)
         strcpy(v_cve_ne,np->cve_ne);
    }
  if(strcmp(v_cve_ne,T_ENTRANTE_LOCAL)==0||strcmp(v_cve_ne,T_SALIENTE_LOCAL)==0)
   return('1');
  else if(strcmp(v_cve_ne,T_ENTRANTE_LD)==0||strcmp(v_cve_ne,T_SALIENTE_LD)==0)
    return('2');
  else
     return('0');   }
  else
     return('0');
}


char find_roaming(char *dn_temm,char *location,char v_cve_IMSI[20])
{  char vn_IMSI[8]="\0";
   char vdn[12]="\0";
   char nir_dn[6]="\0";
   char nir_location[6]="\0";
   char vtipo_roaming;

   if(strlen(v_cve_IMSI)>6) {
     strncpy(vn_IMSI,v_cve_IMSI,6);
     if(strcmp(vn_IMSI,IMSI_TMM1)!=0 && strcmp(vn_IMSI,IMSI_TMM2)!=0)
       return('1');
     }


   if(strlen(dn_temm)>10) 
      strstr2(dn_temm,(strlen(dn_temm)-10),(strlen(dn_temm)-1),vdn);
   else if(strlen(dn_temm)==10)
     strcpy(vdn,dn_temm);
   else
     return('0');
   
   if(strlen(location)<6)
     return('0');   
   
   strstr2(vdn,0,1,nir_dn);
   strstr2(location,3,4,nir_location);

   if(strcmp(nir_dn,NIR_GUA)!=0 && strcmp(nir_dn,NIR_MEX)!=0 && strcmp(nir_dn,NIR_MTY)!=0)
      strstr2(vdn,0,2,nir_dn);

   if(strcmp(nir_location,NIR_GUA)!=0 && strcmp(nir_location,NIR_MEX)!=0 && strcmp(nir_location,NIR_MTY)!=0)
      strstr2(location,2,4,nir_location);
      
   if(strcmp(nir_dn,nir_location)==0)
      return('0');
   else
      return('3');

} 



void calculate_duration(char *v_time_init,char *v_time_end,char *out_duration)
{
 char v_duration_call[8]="\0";
 char hours_init[4];
 char minutes_init[4];
 char seconds_init[4];
 char hours_end[4];
 char minutes_end[4];
 char seconds_end[4];
 int tot_seconds_init=0;
 int tot_seconds_end=0;
 *out_duration='\0'; 
 if(strlen(v_time_init)>=6 && strlen(v_time_end)>=6) 
  {
   strstr2(v_time_init,0,1,hours_init);
   strstr2(v_time_init,2,3,minutes_init);
   strstr2(v_time_init,4,5,seconds_init);
   tot_seconds_init = (atoi(hours_init)*3600)+(atoi(minutes_init)*60)+atoi(seconds_init);
   
   strstr2(v_time_end,0,1,hours_end);
   strstr2(v_time_end,2,3,minutes_end);
   strstr2(v_time_end,4,5,seconds_end);
   tot_seconds_end = (atoi(hours_end)*3600)+(atoi(minutes_end)*60)+atoi(seconds_end);

   if((tot_seconds_end-tot_seconds_init) < 0)
   sprintf(out_duration,"%d%c",((tot_seconds_end+86400)-tot_seconds_init),'\0');
   else if((tot_seconds_end-tot_seconds_init) > 0)
    sprintf(out_duration,"%d%c",(tot_seconds_end-tot_seconds_init),'\0');
   else
    strcpy(out_duration,"0");
  }
   else 
    strcpy(out_duration,"0");
}





char find_prefijo(char v_num_called[20])
{
 char v_prefijo[10]="\0";
 char v_p[5]="\0";
 char prefijo='0';
 int long_called;
 long_called = strlen(v_num_called); 
 if(long_called > 10)
  { strncpy(v_p,v_num_called,2); 
   if(long_called >= 12 && strcmp(v_p,PREFIJO2_1)==0)
      prefijo='1';
   else
     {
      if(long_called == 13)
       {    strncpy(v_prefijo,v_num_called,3);
            if(strcmp(v_prefijo,PREFIJO3_1)==0)
              prefijo='3';
            else if(strcmp(v_prefijo,PREFIJO3_2)==0)
              prefijo='6';   
       }
       else if(long_called >= 12)
       {  strncpy(v_prefijo,v_num_called,5);
          if(strcmp(v_prefijo,PREFIJO5_1)==0)
            prefijo='4';
          else if(strcmp(v_prefijo,PREFIJO5_2)==0)
            prefijo='5';
          else if(strcmp(v_p,PREFIJO2_2)==0)
               prefijo='2';
       }

     }
   }
   else prefijo='0';
 
return(prefijo);
}



void GetLast10(char *in_GetLast10,char *out_GetLast10)
{
  char *source="\0";
  *out_GetLast10='\0';
  strcpy(source,in_GetLast10);
  Reverse(source);
  strncpy(out_GetLast10,source,10); 
  out_GetLast10[10] = '\0';    
  Reverse(out_GetLast10);
}


void Reverse(char *sourc)
{
  int c,i,j;
  for(i=0,j=strlen(sourc)-1;i<j;i++,j--)
    {
	c = sourc[i]; 
      sourc[i] = sourc[j];
      sourc[j] = c;	
    }
}



void DelBlk(char *in_DelBlk,char *out_DelBlk)  
{
  int i,j;
  strcpy(out_DelBlk,"\0");
  j=0;
  for(i=0;i<strlen(in_DelBlk);i++)
    {
	if(in_DelBlk[i] != ' ')
	  { sprintf(out_DelBlk,"%s%c%c",out_DelBlk,in_DelBlk[i],'\0');
	    j++; 
         }
    }
}



/* llOperador: encuentra un operador en operadores[0]...operadores[n-1] */

void llOperador(long long lloper,char *out_dn_min,char *out_id_red)
{       
  long p_init,p_end,p_medium;
  struct nlist *np_op;
  *out_id_red='\0';
  *out_dn_min=='\0';
  for(p_init=0L,p_end=index_op-1;p_init<=p_end;)
    { 
      if(p_init==p_end)            /* si el array tiene solo un elemento  */
        {
          np_op = operadores[p_init]; 
          if(np_op->lldn_min <= lloper && np_op->lldn_max >= lloper)   /* rango encontrado */
            {
              sprintf(out_dn_min,"%lld%c",np_op->lldn_min,'\0');  
              strcpy(out_id_red,np_op->id_red);
              break;
            }
          else
            {
              sprintf(out_dn_min,"%c",'\0');
              break;
            }
        }
      p_medium=(p_init+p_end)/2;
      np_op = operadores[p_medium]; 
      if(np_op->lldn_min <= lloper && np_op->lldn_max >= lloper)  /* Si coincide con el central */
        {
  	  sprintf(out_dn_min,"%lld%c",np_op->lldn_min,'\0');  
          strcpy(out_id_red,np_op->id_red);
          break;
        }
      else if(lloper > np_op->lldn_min  && lloper > np_op->lldn_max)
         p_init=p_medium+1;
      else
         p_end=p_medium-1;
     }
}



void find_red(long long in_dn,long long in_fecha,char *out_id_red)
{
 *out_id_red='\0';        
  long long p_init,p_end,p_medium;
  struct node_portados *np_portados;
  for(p_init=0LL,p_end=idx_portados-1;p_init<=p_end;)
    { 
      if(p_init==p_end)            /* si el array tiene solo un elemento  */
        { np_portados = portados[p_init]; 
          if(np_portados->dn == in_dn && np_portados->fch_inicio <= in_fecha && np_portados->fch_fin >= in_fecha)   /* rango encontrado */
            {
             strcpy(out_id_red,np_portados->id_red);
  	     }

        }
      p_medium=(p_init+p_end)/2;
      np_portados = portados[p_medium]; 
      if(np_portados->dn == in_dn && np_portados->fch_inicio <= in_fecha && np_portados->fch_fin >= in_fecha)  /* Si coincide con el central */
         {
          strcpy(out_id_red,np_portados->id_red);
          break;
         }
      else if(in_dn > np_portados->dn)
         p_init=p_medium+1;
      else if(in_dn == np_portados->dn)
         {
           if(in_fecha > np_portados->fch_inicio && in_fecha > np_portados->fch_fin)
             p_init=p_medium+1;
           else
             p_end=p_medium-1; 
         }
      else
         p_end=p_medium-1;
    }
     
}





void calculate_switch(char *v_switch,char *out_switch)
{
 struct node_switchs *vnp_switchs;
 int i;
 *out_switch='\0'; 
 for(i=0;i<idx_switchs;i++)
    {
      vnp_switchs=switches[i];
      if(strcmp(vnp_switchs->sswitch,v_switch)==0)
           strcpy(out_switch,vnp_switchs->cve_switch);
    }
 
}


