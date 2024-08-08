//	TB6559FG.ino
//
// https://coskxlabsite.stars.ne.jp/html/for_students/M5Stack/PWMTest/PWMTest2.html
//
#include <M5Stack.h>

#define	DUTY_BITS		8
#define	MAX_FREQ_PWM	320000.0
#define	MIN_FREQ_PWM	10000.0
#define	DUTY_VALUE		128
										// PWM周波数の最大値 Maxfreq = 80000000.0/2^n[Hz] = 312500[Hz]
#define	PWM_CH		2	// PWMチャンネル
#define	PWM_PIN		2	// PWM出力に使用するGPIO PIN番号
#define	NSLEEP_PIN	5
#define	EN_PIN		16	
#define	DIR_PIN		17	

double	g_nFreqPWM		= MIN_FREQ_PWM;	//
bool g_bEnable			= false;

////////////////////////////////////////
//
void DisplayValue()
{
	M5.Lcd.setCursor(10, 0);
	M5.Lcd.setTextSize(3);
//	M5.Lcd.setTextFont(3);
	M5.Lcd.print("Frequency");
	M5.Lcd.setCursor(10, 40);
	M5.Lcd.printf("  %6.0f pps ", g_nFreqPWM);
	M5.Lcd.setCursor(10, 80);
	M5.Lcd.print("RPM");
	M5.Lcd.setCursor(10, 120);
	M5.Lcd.printf("  %6.0f rpm ", (g_nFreqPWM * 60 / 6400));
	M5.Lcd.setCursor(10, 160);
	M5.Lcd.printf("EN %s",  g_bEnable ? "ON " : "OFF");
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
	pinMode(NSLEEP_PIN,	OUTPUT); 
	pinMode(EN_PIN,		OUTPUT); 
	pinMode(DIR_PIN,	OUTPUT); 

	digitalWrite(NSLEEP_PIN, HIGH);
	digitalWrite(EN_PIN, LOW);
	digitalWrite(DIR_PIN, LOW);

	// チャンネルと周波数の分解能を設定
	ledcSetup(PWM_CH, 0, DUTY_BITS);

	// PWM出力ピンとチャンネルの設定
	ledcAttachPin(PWM_PIN, PWM_CH);

	// デューティーの設定と出力開始
	ledcWrite(PWM_CH, DUTY_VALUE);
}

////////////////////////////////////////
//
void loop()
{
	int nLastFreqPWM = g_nFreqPWM;
	bool bLastEnable = g_bEnable;

	//static int ValueIndex = 0;
	M5.update();

	if (M5.BtnA.wasPressed()) {
		g_nFreqPWM += 10000.0;

		if (g_nFreqPWM > MAX_FREQ_PWM)
			g_nFreqPWM = MAX_FREQ_PWM;

		Serial.printf("%.1f\r\n", g_nFreqPWM);
	}

	if (M5.BtnB.wasPressed()) {
		if (g_bEnable) {
			g_bEnable = false;
			ledcSetup(PWM_CH, 0, DUTY_BITS);

		} else {
			g_bEnable = true;
			ledcSetup(PWM_CH, g_nFreqPWM, DUTY_BITS);
		}

		digitalWrite(EN_PIN, g_bEnable ? HIGH : LOW);
	}

	if (M5.BtnC.wasPressed()) {
		g_nFreqPWM -= 10000.0;

		if (g_nFreqPWM < MIN_FREQ_PWM)
			g_nFreqPWM = MIN_FREQ_PWM;

		Serial.printf("%.1f\r\n", g_nFreqPWM);
	}

	if (bLastEnable != g_bEnable)
		DisplayValue();

	if (nLastFreqPWM != g_nFreqPWM) {

//		M5.Speaker.tone(523.251, 200); //Peett
//		M5.Speaker.tone(659.255, 200); //Peett
//		M5.Speaker.tone(783.991, 200); //Peett

		DisplayValue();

		// チャンネルと周波数を更新
		if (g_bEnable)
			ledcSetup(PWM_CH, g_nFreqPWM, DUTY_BITS);
		else
			ledcSetup(PWM_CH, 0, DUTY_BITS);

		// 出力再開
//		ledcWrite(PWM_CH, DUTY_VALUE);
	}
}
