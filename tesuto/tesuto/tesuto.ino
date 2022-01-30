#include"func.h"

unsigned char digit[4]={DIG1,DIG2,DIG3,DIG4};
unsigned char pos=0;
int num1=0;
int total=0;
int j=0;
int k=0;

void setup(){
  swsetup();
  seg7Init();//7segの繋がるpinのセットアップ
  adcInit();//可変抵抗器の入力pinセットアップ
  
  cli();
  int0ExInit();//sw1割り込みセットアップ
  timer2OVFInit();//7segオーバーフローセットアップ
  timer1COMPInit();//カウントダウン割り込みセットアップ(最初は停止)
  sei();
}
void loop(){
  num1=setTime();
  sw2process();
  sw1process();
  //if(total==0){
  //  buzz();
  //}
}
