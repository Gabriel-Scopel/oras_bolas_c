/*
Grupo:
Cayque Gastón Cicarelli    (RA: 22.221.005-6)
Gabriel de Souza Scopel    (RA: 22.221.026-2)
Rodrigo Moreira Doraciotto (RA: 22.221.022-1)
Kaique da Silva Fernandes  (RA: 22.221.011-4)
*/

#include <stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include "regras.c" //header das regras de um metro da bola e raio de interpretação
#include "retarobo.c" //formação da reta do robo no campo
#define VELMAX 2.8 // velocidade maxima do robo
#define INDICE 400 // indice na lista xb e yb onde o robo para
#define PI 3.14
typedef struct val Struct;//

//struct que guarda os valores em 3 vetores tempo, x e y da bola do arquivo 
struct val{
    float tem[1001];
    float xb[1001];
    float yb[1001];
};
//struct que salva o ponto de início do robo
typedef struct{
  float xr;
  float yr;
}robo;

//declarações  das funções
int simulacao(); 
Struct arquivo(); 
float MovRoboBolaXY(Struct bolas, float xbf, float ybf, float xr, float yr, double rad);


int main(){
    int resp;
    printf("\n Bem Vindo ao Oras Bolas em C\n");
    while(1){
        resp = menu(); //chama a função menu do arquivo retarobo.c
        if (resp == 1){
            simulacao(); //chama a função simulação
        }
        else if (resp == 2){
            break; //fecha o programa
        }
        else{
            printf("\n Digite uma opcao valida! Voltando ao Menu\n"); //opção não esperada volta ao menu
        }
    }
}

int simulacao(){
    Struct bolas; // chama a struct com os dados do arquivo
    robo inicio; //chama a struct para ter os dados do robo
    int i;
    float tempof, xri, yri, ybf, xbf, m, c, graus;
    double rad;
    robo *pr = (robo*) malloc(sizeof(robo)); //ponteiro para o ponto do robo
    bolas = arquivo(); //chama a função que lê o arquivo retornando uma Struct e salvando na Struct bolas
    //salva os dados iniciais quando estão no começo da trajetória
    float xb = *(bolas.xb+0);
    float yb = *(bolas.yb+0);
    //while true para conferir se o ponto do robo está a 1 metro da bola
    while(1){
      printf("\nDigite o X inicial do robo:\n R: ");
      scanf("%f", &xri);//salva o x do robo
      pr->xr = xri;// salva o valor na struct *(obs final do arquivo)
      printf("\nDigite o Y inicial do robo:\n R: ");
      scanf("%f", &yri);//salva da struct robo yr
      pr->yr = yri; // salva o valor na struct robo *(obs)
      int valido = regraummetro(pr->xr, pr->yr, xb, yb);//chama a função que confere a distancia do ponto do robo até o ponto inicial da bola
      if (valido == 1){
        break;// sai do while true, pois cumpre a regra
      }
      else{
        continue;// continua no whie true até ser dado um ponto que cumpra a regra
      }
    }
    // salva o valor do ponto da bola onde o robo vai parar
    xbf = *(bolas.xb+INDICE);
    ybf = *(bolas.yb+INDICE);
    // salva o ponto de inicio do robo para ser usado nas funções
    rad = RetaRoboTraj(xbf, ybf, pr->xr, pr->yr); // descobre o angulo que será usado na próxima função
    // descobre componentes da reta no campo
    m = (xbf-(pr->xr))/(ybf-(pr->yr)); //multiplicador de x da reta
    c = (pr->yr) - (pr->xr*m); // constante da reta
    graus = rad*180/PI; //conversão de radianos para graus
    tempof = MovRoboBolaXY(bolas, xbf, ybf,pr->xr, pr->yr, rad); //chama a função que retorna o tempo para o encontro da bola
    // achando o ponto de encontro da bola com o robo
    int indi=0;
    for(i=0 ; i<1001;i++){
      if (*(bolas.tem+i) == tempof){
          indi = i; // indice da lista onde o robo encontra a bola
          break;
        }
    }
    printf("\n**************************************");
    printf("\n Ponto inicial do robo: (%.2f, %.2f)\n", pr->xr, pr->yr);
    printf("\n Ponto final do robo: (%.2f, %.2f)\n", xbf, ybf);
    printf("\n Ponto de encontro da bola com o robo: (%.2f, %.2f)\n", *(bolas.xb+indi), *(bolas.yb+indi));
    printf("\n Incinacao da reta: %.2f (%.2f em rad)\n", graus, rad);
    printf("\n Reta no campo 9x6: y = %.2fx + (%.2f)\n", m, c);
    printf("\n O Robo se encontra com a Bola em :%.2f\n", tempof);
    printf("\n Simulacao feita =)\n");
    printf("**************************************\n");
    free(pr);
  return 0;
}

Struct arquivo(){
    Struct bola; // chama a Struct bola para salvar o conteudo do arquivo
    float *tempo, *xbola, *ybola; // ponteiros para cada vetor do Struct
    //alocação dinamica de vetores
    tempo = (float*) malloc(1001*sizeof(float));
    xbola = (float*) malloc(1001*sizeof(float));
    ybola = (float*) malloc(1001*sizeof(float));
    //quarga o enderço do vetor
    
    tempo = &bola.tem[0];
    xbola = &bola.xb[0];
    ybola = &bola.yb[0];
    int i;
    FILE *fp;
    fp = fopen("trajetoria_bola.txt", "r");// abre arquivo
    if (fp == NULL){
        printf(" Nao abriu o arquivo =(\n");//caso o arquivo não abra
        exit(1);
    }
    fseek(fp,13,SEEK_SET);//pula a primeira linha do arquivo que contém letras
    for(i=0; i < 1001; i++){
        fscanf(fp, "%f %f %f", &xbola[i], &ybola[i], &tempo[i]);//captura os dados do arquivo
    }
    fclose(fp);// fecha arquivo
    return bola;//retorna a Struct com seus vetores
}

float MovRoboBolaXY(Struct bolas, float xbf, float ybf, float xr, float yr, double rad){
    int i;
    float deltax, deltay, xb, yb, ax, ay, vx, vy, tpara, t, SRoboY, SRoboX; //variaveis usadas para contas 
    double coss, seno;
    float resp; // variavel de codição ao raio de interceptação
    //cossseno e seno do angulo da reta
    coss = cos(rad);
    seno = sin(rad);
    // calculo da aceleração máxima
    ay = VELMAX*seno;
    ax = VELMAX*coss;
    // calculo para o tempo em que o robo irá desacelerar
    deltax = xbf - xr;
    deltay = ybf - yr;
    tpara = sqrt(pow(deltax/ax, 2));
    // for até o robo se encontrar com a bola
    for (i =0; i<1001; i++){
        t = *(bolas.tem+i);//variavel do tempo para os cáculos
        if (t<=1){
          //momento de aceleração do robo
          SRoboY = (ay*(pow(t,2)))/2 + yr; //posição do robo em X em função do tempo
          vy = ay*t;//velocidade em Y em função do tempo
          SRoboX = (ax*(pow(t,2)))/2 + xr; //posição do robo em Y em função do tempo
          vx = ax*t;//velocidade em X em função do tempo(se repete em cada else if)
        }
        else if(t>1 && t<=tpara){
          //momento de velocidade constante do robo
          vy = ay;
          SRoboY = vy*(t-1) + (ay/2 + yr);
          vx = ax;
          SRoboX = vx*(t-1) + (ax/2 + xr);
        }
        else if(t>tpara && t<=(tpara+1)){
          //momento de desaceleração do robo
          vy = ay - (ay*(t-tpara));
          SRoboY = ay*(t-tpara) - ((ay*(pow(t-tpara,2)))/2) + deltay - (ay/2) + yr;
          vx = ax - (ax*(t-tpara));
          SRoboX = ax*(t-tpara) - ((ax*(pow(t-tpara,2)))/2) + deltax - (ax/2) + xr;
        }
        else if(t>tpara+1){
          //momento em que o robo chega ao ponto da trajetória da bola
          vy = 0;
          vx = 0;
          ax = 0;
          ay = 0;
          SRoboX = deltax + xr;
          SRoboY = deltay + yr;
        }
        //posição da bola no tempo i
        xb = *(bolas.xb+i);
        yb = *(bolas.yb+i);
        //variavel que recebe 1 caso distancia entre a bola e o robo seja menor que 0.09
        resp = raiointerceptacao(SRoboX, SRoboY, xb, yb);//chamada da função
        if (resp == 1){
            return t;//retorna o tempo de encontro da bola
        }
    }
}

//(obs: não consegui achar outra forma de fazer isso, tentei colocar o pr->xr no scanf mas ele só fechava o programa depois de digitar um numero do scanf)