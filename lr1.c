#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "stdkey.h"
#include <unistd.h>

//var conf
unsigned int seed=123;
#define mapA 9999
#define mapB 9999
#define monA 30
#define monB 30
#define crd 10 //cantidad de repeticiones de detecion
#define sjm 15 //separacion jugador-monitor
#define cj 2 //cantidad de jugadores
#define st 100000 //sleep time
#define ppE 700//puntos para un espacio
#define ppB 200//puntos para un #
#define ppEN 90//puntos para un !
#define ppM 10//puntos para un $
#define cjas 30//cantidad de jugadas antes de un salto
#define ppg 10//puntage para poder ganar
//var save
unsigned int date[cj][6];//ubi:0,1 win:2,3 vivo o muerto:4
char names[cj][20];//nombre de los jugadores
char* pmap;
char* pmon;
unsigned int trash=0;

char* map(unsigned int a,unsigned int b){
 if(a>=mapA){return (char*)&trash;}
 if(b>=mapB){return (char*)&trash;}
 return &pmap[a*mapA+b];
}
char* mon(unsigned int a,unsigned int b){
 if(a>=monA){return (char*)&trash;}
 if(b>=monB){return (char*)&trash;}
 return &pmon[a*monA+b];
}

void gene(){
 srand(seed);
 //sistema de general el mapa
 for(unsigned int a=0;a<mapA;a++){
  for(unsigned int b=0;b<mapB;b++){
   unsigned int c=rand()%(ppE+ppB+ppEN+ppM);
   if(c<ppE){*map(a,b)=0;}
   if((c>=ppE)&&(c<ppE+ppB)){*map(a,b)=1;}
   if((c>=ppE+ppB)&&(c<ppE+ppB+ppEN)){*map(a,b)=2;}
   if((c>=ppE+ppB+ppEN)&&(c<ppE+ppB+ppEN+ppM)){*map(a,b)=3;}
  }
 }
 return;
}

int main(){
 //inicializacion
 printf("SEED ");scanf("%u",&seed);
 pmap=(char*)calloc(mapA*mapB,sizeof(char));
 if(pmap==NULL){printf("Error de memoria : MAP\n");return 0;}
 pmon=(char*)calloc(monA*monB,sizeof(char));
 if(pmon==NULL){printf("Error de memoria : MON\n");return 0;}
 for(unsigned int a=0;a<cj;a++){
  date[a][0]=rand()%mapA;
  date[a][1]=rand()%mapB;
  date[a][2]=((int)date[a][0]+sjm+1-monA)<0?0:(date[a][0]+sjm+1-monA);
  date[a][3]=((int)date[a][1]+sjm+1-monB)<0?0:(date[a][1]+sjm+1-monB);
  date[a][4]=0;
  date[a][5]=0;
  *map(date[a][0],date[a][1])=0;
  printf("Nombre para el jugador N*%u.\n",a);
  scanf("%s",&names[a][0]);
 }
 //generar mapa
 gene();
 //iniciar ubicaciones
 unsigned int ja=0;//jugador actual
 unsigned int ua[4];//user actual
 unsigned int acion=0;//acion echa?
 unsigned int cjv=cj;//cantidad de jugadores vivos
 while(true){
  if((bool)(date[ja][4])){
   ja=(ja+1)%cj;
   continue;
  }
  if(cjv==1){
   system("clear");
   printf("A ganado el jugador %s por que todos los demas han muerto!\n",&names[ja][0]);
   free(pmap);
   free(pmon);
   return 0;
  }
  ua[0]=date[ja][0];
  ua[1]=date[ja][1];
  ua[2]=date[ja][2];
  ua[3]=date[ja][3];
  //renderizador
  for(unsigned int a=0;a<monA;a++){
   for(unsigned int b=0;b<monB;b++){
    switch(*map(date[ja][2]+a,date[ja][3]+b)){
     case 1:{
      *mon(a,b)=1;
     }break;
     case 2:{
      *mon(a,b)=3;
     }break;
     case 3:{
      *mon(a,b)=4;
     }break;
     default:{
      *mon(a,b)=0;
     }break;
    };
    for(unsigned int c=0;c<cj;c++){
     if(((date[ja][2]+a==date[c][0])&&(date[ja][3]+b==date[c][1]))&&(!((bool)(date[c][4])))){
      *mon(a,b)=2;
     }
    }
   }
  }
  //printar en pantalla
  system("clear");
  printf("\e[0;36m\e[46m");for(unsigned int b=0;b<monB+2;b++){printf("L");}printf("\e[0;37m\e[40m\n");
  for(unsigned int a=0;a<monA;a++){
   printf("\e[0;36m\e[46mP\e[0;37m\e[40m");
   for(unsigned int b=0;b<monB;b++){
    switch(*mon(a,b)){
     case 1:{
      printf("\e[0;37m\e[40m#\e[0;37m\e[40m");
     }break;
     case 2:{
      printf("\e[0;34m\e[42m&\e[0;37m\e[40m");
     }break;
     case 3:{
      printf("\e[0;30m\e[41m!\e[0;37m\e[40m");
     }break;
     case 4:{
      printf("\e[0;30m\e[45m$\e[0;37m\e[40m");
     }break;
     default:{
      printf(" ");
     }break;
    };
   }
   printf("\e[0;36m\e[46mP\e[0;37m\e[40m\n");
  }
  printf("\e[0;36m\e[46m");for(unsigned int b=0;b<monB+2;b++){printf("L");}printf("\e[0;37m\e[40m\n");
  
  printf(" - %s(%u) - $%u/%u M%u/%u { [%u %u] [%u %u] }\n",&names[ja][0],ja,date[ja][5],ppg,acion,cjas,date[ja][0],date[ja][1],date[ja][2],date[ja][3]);
  usleep(st);
  //detecion de teclas
  for(unsigned int a=0;a<crd;a++){
   if(kbhit()){
    switch(getch()){
     case 'q':{
      free(pmap);
      free(pmon);
      system("clear");
      return 0;
     }break;
     case '8':{ua[0]--;acion+=1;goto con1;}break;
     case '2':{ua[0]++;acion+=1;goto con1;}break;
     case '4':{ua[1]--;acion+=1;goto con1;}break;
     case '6':{ua[1]++;acion+=1;goto con1;}break;
     case '7':{ua[0]--;ua[1]--;acion+=1;goto con1;}break;
     case '9':{ua[0]--;ua[1]++;acion+=1;goto con1;}break;
     case '1':{ua[0]++;ua[1]--;acion+=1;goto con1;}break;
     case '3':{ua[0]++;ua[1]++;acion+=1;goto con1;}break;
     case 'e':{ua[0]--;acion+=1;goto con1;}break;
     case 'c':{ua[0]++;acion+=1;goto con1;}break;
     case 's':{ua[1]--;acion+=1;goto con1;}break;
     case 'f':{ua[1]++;acion+=1;goto con1;}break;
     case 'w':{ua[0]--;ua[1]--;acion+=1;goto con1;}break;
     case 'r':{ua[0]--;ua[1]++;acion+=1;goto con1;}break;
     case 'x':{ua[0]++;ua[1]--;acion+=1;goto con1;}break;
     case 'v':{ua[0]++;ua[1]++;acion+=1;goto con1;}break;
    };
   }
  }
  con1:
  //aplicador de normas del mundo
   //sistema de mover !
  
   //limite y colisiones de movimiento de los jugadores y la pantalla
  if(((ua[0]<mapA)&&(ua[1]<mapB))&&(*map(ua[0],ua[1])!=1)){
   date[ja][0]=ua[0];
   date[ja][1]=ua[1];
   date[ja][2]=((int)ua[0]+sjm+1-monA)<0?0:(ua[0]+sjm+1-monA);
   date[ja][3]=((int)ua[1]+sjm+1-monB)<0?0:(ua[1]+sjm+1-monB);
  }
   //sistema de interacion
  switch(*map(date[ja][0],date[ja][1])){
   case 2:{//!
    date[ja][4]=1;
    cjv--;
   }break;
   case 3:{//$
    date[ja][5]++;
    if(date[ja][5]>ppg){
     system("clear");
     printf("A ganado el jugador %s por llegar a los puntos necesarios!\n",&names[ja][0]);
     free(pmap);
     free(pmon);
     return 0;
    }
    *map(date[ja][0],date[ja][1])=0;
   }break;
  };
  if(acion>cjas){
   ja=(ja+1)%cj;
   acion=0;
  }
 }
 return 0;
}
