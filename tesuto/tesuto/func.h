#define DIG1 13
#define DIG2 8
#define DIG3 21
#define DIG4 7
#define SW1 2
#define SW2 4
#define CTC_COMP_VALUE 31249
#define BZ 3
void swsetup(void);
boolean swread(int swnum);

void seg7Init(void);//7seg表示器GPIO端子出力設定
void seg7Disp(int dispnum);//LEDカソードに出力（LED光らす）関数
void seg7Pos(int pos);//4桁のうちどの桁に表示するかの関数
void timer2OVFInit(void);//timer2を割り込みを使うためのセットアップ

void adcInit(void);//EDC1を単一で使うためのレジスタセットアップ
int adcRead(void);//ADC1のアナログ値を読み取って返す関数

int setTime(void);//ADCHレジスタ２バイトから計測時間を生成する関数
void sw2process(void);//設定時間加算関数
void int0ExInit(void);//sw1割り込みセットアップ関数
void timer1COMPInit(void);//カウントダウン割り込みセットアップ(最初は)
void sw1process(void);//sw1の処理(ストップ、リスタート)
void timer2COMPInit(void);//buzzerセットアップ関数
