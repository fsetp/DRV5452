//	TB6559FG.ino
//
// https://coskxlabsite.stars.ne.jp/html/for_students/M5Stack/PWMTest/PWMTest2.html
//
#include <M5Stack.h>

const uint8_t	nResBits	= 8;		// PWMに使用するビット数　n=1～16[bit]
										// PWM周波数の最大値 Maxfreq = 80000000.0/2^n[Hz] = 312500[Hz]
const uint8_t	PWM_CH		= 2;		// PWMチャンネル
const uint8_t	PWM_PIN		= 2;		// PWM出力に使用するGPIO PIN番号
const uint8_t	EN_PIN		= 3;		
const uint8_t	DIR_PIN		= 1;		


const int g_nDutyPWM		= 128;		// デューティ n / 255
const double MaxFreePWM		= 160000.0;	//
const double MinFreePWM		= 40000.0;	//
double	g_nFreqPWM			= MinFreePWM;	//
bool g_bEnable				= false;

////////////////////////////////////////
//
void DisplayValue()
{
	M5.Lcd.setCursor(10, 0);
	M5.Lcd.setTextSize(3);
//	M5.Lcd.setTextFont(3);
	M5.Lcd.print("Duty Value");
	M5.Lcd.setCursor(10, 40);
	M5.Lcd.printf("  %d / 255   ", g_nDutyPWM);
	M5.Lcd.setCursor(10, 80);
	M5.Lcd.print("Frequency");
	M5.Lcd.setCursor(10, 120);
	M5.Lcd.printf("  %.1f	", g_nFreqPWM);
}

////////////////////////////////////////
//
void setup()
{
	M5.begin();
//	delay(10);
	M5.Power.begin();
//	delay(10);
	Serial.begin(115200);
//	delay(10);

	DisplayValue();

	pinMode(PWM_PIN,	OUTPUT); 
	pinMode(EN_PIN,		OUTPUT); 
	pinMode(DIR_PIN,	OUTPUT); 

	// チャンネルと周波数の分解能を設定
	ledcSetup(PWM_CH, g_nFreqPWM, nResBits);

	// PWM出力ピンとチャンネルの設定
	ledcAttachPin(PWM_PIN, PWM_CH);

	// デューティーの設定と出力開始
	ledcWrite(PWM_CH, g_nDutyPWM);
}

////////////////////////////////////////
//
void loop()
{
	int nLastFreqPWM = g_nFreqPWM;

	//static int ValueIndex = 0;
	M5.update();

	if (M5.BtnA.wasPressed()) {
		g_nFreqPWM += 10000.0;

		if (g_nFreqPWM > MaxFreePWM)
			g_nFreqPWM = MaxFreePWM;

		Serial.printf("%.1f\r\n", g_nFreqPWM);
	}

	if (M5.BtnB.wasPressed()) {
		if (g_bEnable)
			g_bEnable = false;

		else
			g_bEnable = true;

		digitalWrite(EN_PIN, g_bEnable ? HIGH : LOW);
	}

	if (M5.BtnC.wasPressed()) {
		g_nFreqPWM -= 10000.0;

		if (g_nFreqPWM < MinFreePWM)
			g_nFreqPWM = MinFreePWM;

		Serial.printf("%.1f\r\n", g_nFreqPWM);
	}

	if (nLastFreqPWM != g_nFreqPWM) {

//		M5.Speaker.tone(523.251, 200); //Peett
//		M5.Speaker.tone(659.255, 200); //Peett
//		M5.Speaker.tone(783.991, 200); //Peett

		DisplayValue();

		// チャンネルと周波数を更新
		ledcSetup(PWM_CH, g_nFreqPWM, nResBits);

		// 出力再開
		ledcWrite(PWM_CH, g_nDutyPWM);
	}
}
