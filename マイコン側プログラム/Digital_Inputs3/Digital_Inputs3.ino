const int DIN_PIN = 7;

void setup()
  {
    pinMode( DIN_PIN, INPUT );  /* デジタルピン7番を入力モードに設定 */
    Serial.begin( 9600 );       /* シリアル通信を初期化する。通信速度は9600bps */
  }

void loop()
    {
      int value;

      value = digitalRead( DIN_PIN );   /* センサの値の読み取り */

      /* センサに反応があった場合だけ処理 */
      if(value == 0 )
      {
        Serial.println( 1 );    /* ソフト側に1を送信 */

        delay( 1 );   /* 一度のプッシュで複数回送信しないために、1秒間の待機 */
      }

      delay( 1 );   /*0.1秒の待機 */
    }
