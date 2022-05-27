#include <stdlib.h>
#include<stdio.h>
#include<math.h>
#include<string.h>
#include "retarobo.h"//arquivo que contem as declarações das funções

double RetaRoboTraj(float xbf, float ybf, float xr, float yr){
  //calculo para descobrir o angulo da reta
  float deltaY = ybf-yr;
  float deltaX = xbf-xr;
  float m = deltaY/deltaX;
  double rad= atan(m);
  return rad;// retorna o angulo em radianos
}

int menu(){
    //print do menu 
    int n;
    printf("\n-----------------------\n");
    printf(" Digite uma opcao:\n  1- Comecar simluacao\n  2- Sair\n");
    printf("-----------------------\n R: ");
    scanf("%d", &n);
    return n; //retorna o valor digitado
}