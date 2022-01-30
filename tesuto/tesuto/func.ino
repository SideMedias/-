void swsetup(void){
  DDRD &= ~(1<<SW2);//sw２のinput設定
  PORTD |= (1<<SW2);//sw2のpullup設定
}
boolean swread(int swnum){
  return (PIND>>swnum)&0x01 ? 0:1;
}
void seg7Init(void){
  for(int i=5; i<=14; i++)pinMode(i,OUTPUT);
    pinMode(20,OUTPUT);
    pinMode(21,OUTPUT);
}

void seg7Disp(int dispNum){
  unsigned char disp[10] = {0x03,0x9f,0x25,0x0d,0x99,0x49,0x41,0x1f,0x01,0x09};
  
   digitalWrite(12,(disp[dispNum]>>7)&0x01);
   digitalWrite(20,(disp[dispNum]>>6)&0x01);
   digitalWrite(5,(disp[dispNum]>>5)&0x01);
   digitalWrite(10,(disp[dispNum]>>4)&0x01);
   digitalWrite(11,(disp[dispNum]>>3)&0x01);
   digitalWrite(14,(disp[dispNum]>>2)&0x01);
   digitalWrite(6,(disp[dispNum]>>1)&0x01);
   
}
void seg7Pos(int pos){
  digitalWrite(DIG1,LOW);
  digitalWrite(DIG2,LOW);
  digitalWrite(DIG3,LOW);
  digitalWrite(DIG4,LOW);

  digitalWrite(pos,HIGH);
}
void timer2OVFInit(void){
  bitClear(TCCR2B, WGM22);
  bitClear(TCCR2A, WGM21);
  bitClear(TCCR2A, WGM20);
  bitSet(TCCR2B,CS22);
  bitClear(TCCR2B,CS21);
  bitSet(TCCR2B,CS20);
  bitSet(TIMSK2,TOIE2); /*Timer2 OVF Enabled*/
}

//LED4桁に表出するための割り込み。j==0:時間設定画面,j==1:countdown画面
ISR(TIMER2_OVF_vect){
  digitalWrite(DIG1,LOW);
  digitalWrite(DIG2,LOW);
  digitalWrite(DIG3,LOW);
  digitalWrite(DIG4,LOW);

  pos = ++pos % 4;
  if(j==1){
    switch(pos){
      case 0:
        seg7Disp(total / 600); break;
      case 1:
        seg7Disp((total / 60) % 10); break;
      case 2:
        seg7Disp((total / 10) % 6); break;
      case 3:
        seg7Disp((total / 1) % 10); break;
    }
  }else if(j==0){
    switch(pos){
      case 0:
        seg7Disp(total / 600); break;
      case 1:
        seg7Disp((total / 60) % 10); break;
      case 2:
        seg7Disp(num1 / 600); break;
      case 3:
        seg7Disp((num1 / 60) % 10); break;
    }   
  }
  digitalWrite(digit[pos],HIGH);
}
void adcInit(void){
  ADMUX = 0b01000001;
  bitSet(DIDR0,ADC1D);
  bitSet(ADCSRA,ADEN);
}
int adcRead(void){
  bitSet(ADCSRA,ADSC);
  while(bitRead(ADCSRA,ADSC));
  return ADC;
}
int setTime(void){
  bitSet(ADCSRA,ADSC);
  switch((adcRead() >> 8) & 0x03){
    case 0:
      return 5*60;
    case 1:
      return 10*60;
    case 2:
      return 15*60;
    case 3:
      return 30*60;
  }
}
//SW2の処理(加算、リセット).j==0:時間設定画面,j==1:countdown画面
void sw2process(void){
  if(j==0){
    if(digitalRead(SW2)==0){
      delay(500);
      total=total+num1;
      if(total>90*60){
        total=0;
      }
    }
  }else if(j==1){
    if(digitalRead(SW2)==0){
      delay(500);
      total=0;
      j=0;
    }
  }
}
void int0ExInit(void){
  pinMode(SW1,INPUT);
  bitClear(EICRA,ISC00);
  bitSet(EICRA,ISC01);
  bitSet(EIMSK,INT0);
}
//sw1割り込み
ISR(INT0_vect){
  j=1;//coutdown表示へ
  
  if(k){
    k=1;
  }
  else{
    k=0;
  }
  /*
  bitClear(TCCR2A,COM2B0);
  bitClear(TCCR2B, CS22);
  cli(); 
  timer2OVFInit();
  sei();*/
}
void timer1COMPInit(void){
  bitClear(TCCR1B, WGM13);
  bitSet(TCCR1B, WGM12);
  bitClear(TCCR1A, WGM11);
  bitClear(TCCR1A, WGM10);
  bitClear(TCCR1B,CS12);//最初は停止
  bitClear(TCCR1B,CS11);
  bitClear(TCCR1B,CS10);
  OCR1A = CTC_COMP_VALUE; /*比較値設定*/
  bitSet(TIMSK1,OCIE1A); /*Timer1 COMP Enabled*/
}
//sw1の処理。(スタート、ストップ)
void sw1process(void){
  if(k==1){
    bitSet(TCCR1B,CS12);//カウントダウン開始
  }
  else if(k==0){
    bitClear(TCCR1B,CS12);//カウントダウン停止
  }
}
//カウントダウン割り込み
ISR(TIMER1_COMPA_vect){
  total=total-1;
}
//buzzerセットアップ関数
void timer2COMPInit(void) {
  pinMode(BZ, OUTPUT);
  bitClear(TCCR2A, COM2B1);
  bitSet(TCCR2A, COM2B0);
  bitClear(TCCR2B, WGM22);
  bitSet(TCCR2A, WGM21);
  bitClear(TCCR2A, WGM20);
  bitSet(TCCR2B, CS22);
  bitClear(TCCR2B, CS21);
  bitClear(TCCR2B, CS20);
}
void buzz(void){ 
  cli();
  timer2COMPInit();//timer2の割り込みをbuzzer用にセットアップ
  sei();
  OCR2A =0;//音の高さ

  seg7Disp(4);
  seg7Pos(DIG1);
  seg7Pos(DIG3);
  seg7Pos(DIG2);
  seg7Pos(DIG4);
}
