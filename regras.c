#include <stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include "regras.h"//arquivo que contem as declarações das funções


int regraummetro(float xr,float yr, float xb, float yb){
  //calcula a distancia entre os ponto inicial da bola  e o ponto inicial do robo 
  float dist;
  dist = sqrt(pow(xr-xb, 2) + pow(yr-yb,2));
  //confere se cumpre a regra do ponto inicial do robo estar a um metro da bola
  if (dist <= 1){
    printf("\nO ponto fornecido respeita a regra de um metro de distancia\n");
    return 1;//retorna um valor que será usado como parametro para sair de um loop
  }
  else{
    printf("\nO ponto fornecido esta a mais de um metro de distancia da bola\n");
    return 0;//retorna um valor que será usado como parametro para continuar de um loop
  }
}

int raiointerceptacao(float xr, float yr, float xb, float yb){
  //confere se o robo se encontra com a bola
  float dist;
  dist = sqrt(pow(xr-xb, 2) + pow(yr-yb,2));
  if (dist <= 0.09){
    return 1;//caso seja menor ou igual a 0.09, o robo e a bola se encontrando, quebrando o for loop
  }
  else{
    return 0;//caso seja maior que 0.09, o robo e a bola não se encontram, continuando o for loop
  }
}