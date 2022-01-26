//-----------------------------プリプロセッサ関連の宣言------------------------------------------------------------------
#define _CRT_SECURE_NO_WARNINGS     //VisualStudioの警告を無効化
#include <DxLib.h>                  //DXライブラリ
#include <stdio.h>                  //標準ライブラリ
#include <stdlib.h>                 //汎用ユーティリティ
#include <time.h>                   //タイムライブラリ
#include <tchar.h>                  //文字列切り替えマクロ
#include <windows.h>                //windowsライブラリ
#include <string.h>                 //文字列ライブラリ
#include <iostream>                 //マルチスレッド処理で活用
#include <thread>                   //マルチスレッド処理で活用

//-----------------------------プロトタイプ宣言と構造体配列の宣言------------------------------------------------------------------
//プロトタイプ宣言
void file_w();              //ファイル書き込み関数
int file_r();               //ファイル読み込み関数
void stamp(int year, int month, int day, int hour, int minute, int second);     //スタンプデータを配列に保存する関数
int serial();               //シリアル通信関数
void draw(int month);       //スタンプの描画関数
void task();                //シリアル通信のスレッド用関数

// 構造体の定義
typedef struct when {
    /* 年月日と時分秒保存用 */
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
}WHEN;

// 構造体配列の宣言・0で初期化
WHEN when[13][7][8][9] = {};   //12か月間の、6×8マスを保存する。1マス最大保存数は8。添え字は1を開始とし、0は使用しない。

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    //-----------------------------変数宣言と初期化------------------------------------------------------------------

    //変数宣言
    unsigned int Cr;    //カラー変数
    int month;          //何月か
    int flag;           //フラグ変数、クリック判定に使用
    //マウス関連の変数
    int Button;         //どのマウスボタンを押したか
    int ClickX;         //マウスのX座標
    int ClickY;         //マウスのY座標
    int LogType;        //マウスを押したか離したか

    //初期化
    Cr = 0;
    month = 1;
    flag = 0;

    //-----------------------------ＤＸライブラリの初期設定------------------------------------------------------------------

    //起動オプション
    SetBackgroundColor(255, 255, 255);      //背景色を白に変更
    ChangeWindowMode(TRUE);                 //ウィンドウ設定で動作
    SetDrawScreen(DX_SCREEN_BACK);          //表示エリア外の描画を有効化
    SetGraphMode(1024, 576, 32);            //解像度を1024×576に変更
    SetWindowSize(1440, 810);               //ウィンドウサイズは1440×810
    SetFontSize(50);                        //フォントサイズを50に変更
    SetMainWindowText("手洗いチェッカー");  //ウィンドタイトルを手洗いカレンダーに変更
    SetWindowIconID(107);                   //アイコンを変更

    // ＤＸライブラリ初期化処理
    if (DxLib_Init() == -1)
    {
        return -1;      // エラーが起きたら直ちに終了
    }

    //-----------------------------シリアル通信受け付け開始------------------------------------------------------

    //task関数をスレッド化してマルチスレッド処理を行う
    using namespace std;
    thread thd{ task };

    //-----------------------------タイトル画面------------------------------------------------------------------

    //タイトル画面の画像を表示
    LoadGraphScreen(0, -50, "taitoru.png", FALSE);  //タイトル画像を表示
    DrawFormatString(240, 450, GetColor(150, 150, 255), "クリックをしてください");     //テキストを表示
    file_r();                                       //ファイル読み込み

    WaitKey();  // キーの入力待ち

    //-----------------------------メイン画面------------------------------------------------------------------

    while (1)   //この中を無限ループ
    {

        // 画面を初期化
        ClearDrawScreen();

        //カレンダーとスタンプ（見本）の表示
        switch (month) {
        case 1:
            LoadGraphScreen(0, 0, "karennda\\karennda1.png", TRUE);
            LoadGraphScreen(655, -18, "stamp\\stamp1.png", TRUE);
            break;
        case 2:
            LoadGraphScreen(0, 0, "karennda\\karennda2.png", TRUE);
            LoadGraphScreen(655, -16, "stamp\\stamp2.png", TRUE);
            break;
        case 3:
            LoadGraphScreen(0, 0, "karennda\\karennda3.png", TRUE);
            LoadGraphScreen(655, -18, "stamp\\stamp3.png", TRUE);
            break;
        case 4:
            LoadGraphScreen(0, 0, "karennda\\karennda4.png", TRUE);
            LoadGraphScreen(655, -18, "stamp\\stamp4.png", TRUE);
            break;
        case 5:
            LoadGraphScreen(0, 0, "karennda\\karennda5.png", TRUE);
            LoadGraphScreen(655, -18, "stamp\\stamp5.png", TRUE);
            break;
        case 6:
            LoadGraphScreen(0, 0, "karennda\\karennda6.png", TRUE);
            LoadGraphScreen(655, -18, "stamp\\stamp6.png", TRUE);
            break;
        case 7:
            LoadGraphScreen(0, 0, "karennda\\karennda7.png", TRUE);
            LoadGraphScreen(655, -18, "stamp\\stamp7.png", TRUE);
            break;
        case 8:
            LoadGraphScreen(0, 0, "karennda\\karennda8.png", TRUE);
            LoadGraphScreen(655, -18, "stamp\\stamp8.png", TRUE);
            break;
        case 9:
            LoadGraphScreen(0, 0, "karennda\\karennda9.png", TRUE);
            LoadGraphScreen(655, -18, "stamp\\stamp9.png", TRUE);
            break;
        case 10:
            LoadGraphScreen(0, 0, "karennda\\karennda10.png", TRUE);
            LoadGraphScreen(655, -18, "stamp\\stamp10.png", TRUE);
            break;
        case 11:
            LoadGraphScreen(0, 0, "karennda\\karennda11.png", TRUE);
            LoadGraphScreen(655, -18, "stamp\\stamp11.png", TRUE);
            break;
        case 12:
            LoadGraphScreen(0, 0, "karennda\\karennda12.png", FALSE);
            LoadGraphScreen(655, -19, "stamp\\stamp12.png", TRUE);
            break;
        default:
            break;
        }

        //スタンプを描画
        draw(month);

        //三角形の色
        Cr = GetColor(150, 150, 255);

        // 三角形を描画
        if (month != 1) {   //１月でなければ
            //左上に矢印を描画
            DrawFormatString(140, 20, GetColor(150, 150, 255), "←");
        }

        if (month != 12) {  //４月でなければ
            //右上に矢印を描画
            DrawFormatString(860, 20, GetColor(150, 150, 255), "→");
        }

        ScreenFlip();  //裏画面を表画面へ表示
        //-----------------------------マウス処理------------------------------------------------------------------

        // マウスのボタンが押されたり離されたりしたかどうか
        if (GetMouseInputLog2(&Button, &ClickX, &ClickY, &LogType, true) == 0)
        {
            //左クリックをしたら
            if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)
            {
                //左上の三角を押したとき１月でなければ
                if ((130 <= ClickX && ClickX <= 180) && (10 <= ClickY && ClickY <= 60) && (month != 1))
                {
                    month--;        //月を減算
                }
                //右上の三角を押したとき４月でなければ
                if ((860 <= ClickX && ClickX <= 910) && (10 <= ClickY && ClickY <= 60) && (month != 12))
                {
                    month++;        //月を加算
                }
                
                //-----------------------------詳細画面処理------------------------------------------------------------------
                
                int x, y;
                y = 167;
                for (int i = 1; i <= 6; i++) {
                    x = 56;
                    for (int j = 1; j <= 7; j++) {
                        //スタンプを押すと詳細画面が開かれる
                        if ((x <= ClickX && ClickX <= x+75) && (y <= ClickY && ClickY <= y+75) && (when[month][i][j][1].year >= 1)) {   //もしスタンプを押したら
                            if(flag != 1){      //クリック処理のフラグ
                                // 画面を初期化
                                ClearDrawScreen();
                                //詳細画面用の背景画像とスタンプ画像を表示
                                char name[64];
                                switch (month) {
                                case 1:
                                    LoadGraphScreen(0, 0, "karennda\\karennda1_.png", TRUE);
                                    LoadGraphScreen(655, -18, "stamp\\stamp1.png", TRUE);
                                    strcpy(name, "stamp\\stamp1_p.png");
                                    break;
                                case 2:
                                    LoadGraphScreen(0, 0, "karennda\\karennda2_.png", TRUE);
                                    LoadGraphScreen(655, -16, "stamp\\stamp2.png", TRUE);
                                    strcpy(name, "stamp\\stamp2_p.png");
                                    break;
                                case 3:
                                    LoadGraphScreen(0, 0, "karennda\\karennda3_.png", TRUE);
                                    LoadGraphScreen(655, -18, "stamp\\stamp3.png", TRUE);
                                    strcpy(name, "stamp\\stamp3_p.png");
                                    break;
                                case 4:
                                    LoadGraphScreen(0, 0, "karennda\\karennda4_.png", TRUE);
                                    LoadGraphScreen(655, -18, "stamp\\stamp4.png", TRUE);
                                    strcpy(name, "stamp\\stamp4_p.png");
                                    break;
                                case 5:
                                    LoadGraphScreen(0, 0, "karennda\\karennda5_.png", TRUE);
                                    LoadGraphScreen(655, -18, "stamp\\stamp5.png", TRUE);
                                    strcpy(name, "stamp\\stamp5_p.png");
                                    break;
                                case 6:
                                    LoadGraphScreen(0, 0, "karennda\\karennda6_.png", TRUE);
                                    LoadGraphScreen(655, -18, "stamp\\stamp6.png", TRUE);
                                    strcpy(name, "stamp\\stamp6_p.png");
                                    break;
                                case 7:
                                    LoadGraphScreen(0, 0, "karennda\\karennda7_.png", TRUE);
                                    LoadGraphScreen(655, -18, "stamp\\stamp7.png", TRUE);
                                    strcpy(name, "stamp\\stamp7_p.png");
                                    break;
                                case 8:
                                    LoadGraphScreen(0, 0, "karennda\\karennda8_.png", TRUE);
                                    LoadGraphScreen(655, -18, "stamp\\stamp8.png", TRUE);
                                    strcpy(name, "stamp\\stamp8_p.png");
                                    break;
                                case 9:
                                    LoadGraphScreen(0, 0, "karennda\\karennda9_.png", TRUE);
                                    LoadGraphScreen(655, -18, "stamp\\stamp9.png", TRUE);
                                    strcpy(name, "stamp\\stamp9_p.png");
                                    break;
                                case 10:
                                    LoadGraphScreen(0, 0, "karennda\\karennda10_.png", TRUE);
                                    LoadGraphScreen(655, -18, "stamp\\stamp10.png", TRUE);
                                    strcpy(name, "stamp\\stamp10_p.png");
                                    break;
                                case 11:
                                    LoadGraphScreen(0, 0, "karennda\\karennda11_.png", TRUE);
                                    LoadGraphScreen(655, -18, "stamp\\stamp11.png", TRUE);
                                    strcpy(name, "stamp\\stamp11_p.png");
                                    break;
                                case 12:
                                    LoadGraphScreen(0, 0, "karennda\\karennda12_.png", FALSE);
                                    LoadGraphScreen(655, -19, "stamp\\stamp12.png", TRUE);
                                    strcpy(name, "stamp\\stamp12_p.png");
                                    break;
                                default:
                                    break;
                                }
                                //詳細画面の細かいレイアウト
                                SetFontSize(75);                        //フォントサイズを75に変更
                                DrawFormatString(50, 150, GetColor(255, 100, 100), "%d日", when[month][i][j][1].day);    //押された押した日付をテキスト表示
                                SetFontSize(50);                        //フォントサイズを50に変更
                                int k;                                  //ループ用変数k。構造体配列の添え字とかにもなる
                                for (k = 1; when[month][i][j][k].year != 0; k++) {      //押された日付のデータをすべて読む
                                    DrawLine(250, 150 + (50 * k), 700, 150 + (50 * k), GetColor(0, 0, 0));    // 線を描画
                                    DrawFormatString(250, 100 + (50 * k), GetColor(0, 0, 0), "%d回目　%d時%d分%d秒",k,
                                        when[month][i][j][k].hour,when[month][i][j][k].minute, when[month][i][j][k].second);     //いつ手洗いを行ったかテキスト表示
                                    LoadGraphScreen(363, 86 + (50 * k), name, TRUE);    //月のスタンプを表示
                                }
                                SetFontSize(75);                        //フォントサイズを75に変更
                                DrawFormatString(725, 400, GetColor(255, 100, 100), "合計%d回", k - 1);        //合計手洗い回数を表示
                                SetFontSize(25);                        //フォントサイズを20に変更
                                DrawFormatString(750, 515, GetColor(200, 200, 255), "クリックで戻れます");     //説明テキストを表示
                                SetFontSize(50);                        //フォントサイズを50に変更
                                flag = 1;   //クリック処理のフラグ
                            }
                            else {
                                flag = 0;   //クリック処理のフラグ
                            }
                        }
                        x += 141;
                    }
                    y += 64;
                }
                if (flag) {     //クリック処理のフラグ
                    WaitKey();  // キーの入力待ち
                    flag = 1;   //クリック処理のフラグ
                }

            }
            
        }
        //×ボタンで終了
        if (ProcessMessage() != 0) {
            break;
        }
    }

    thd.join();     //ここまでマルチスレッド処理を行う        

    DxLib_End();    // ＤＸライブラリ使用の終了処理

    return 0;       // ソフトの終了
}

//-----------------------------シリアル通信のスレッド用関数------------------------------------------------------------------
void task()
{
    while (1)   //この中を無限ループ
    {
        //×ボタンで終了
        if (ProcessMessage() != 0) {
            break;
        }
        //Sleep(1000);             // 受信バッファにメッセージが溜まるまで待つ
        if (serial() == 1)      //磁気センサーに反応があった場合
        {
            file_w();           //現在時刻をファイル書き込み
            //file_r();           //ファイル読み込み
            Sleep(10000);       //連続感知対策で10秒待機
        }
    }
}

//-----------------------------ファイル書き込み関数------------------------------------------------------------------
void file_w() {

    time_t timer;        /* 時刻を取り出すための型（実際はunsigned long型） */
    struct tm* local;    /* tm構造体（時刻を扱う */

    /* 年月日と時分秒保存用 */
    int year, month, day, hour, minute, second;

    timer = time(NULL);        /* 現在時刻を取得 */
    local = localtime(&timer);    /* 地方時に変換 */

    /* 年月日と時分秒をtm構造体の各パラメタから変数に代入 */
    year = local->tm_year + 1900;        /* 1900年からの年数が取得されるため */
    month = local->tm_mon + 1;        /* 0を1月としているため */
    day = local->tm_mday;
    hour = local->tm_hour;
    minute = local->tm_min;
    second = local->tm_sec;

    FILE* outputfile;         // 出力ストリーム

    outputfile = fopen("data.csv", "a");  // ファイルを書き込み用にオープン(開く)
    if (outputfile == NULL) {          // オープンに失敗した場合
        printf("cannot open\n");         // エラーメッセージを出して
        exit(1);                         // 異常終了
    }

    fprintf(outputfile, "%d,%d,%d,%d,%d,%d\n", year, month, day, hour, minute, second); // ファイルに書く

    fclose(outputfile);          // ファイルをクローズ(閉じる)

    stamp(year, month, day, hour, minute, second);      //構造体配列に時刻を保存
}

//-----------------------------ファイル読み込み関数------------------------------------------------------------------
int file_r() {
    FILE* fp; // FILE型構造体
    int year, month, day, hour, minute, second; //年、月、日、時間、分、秒を保存する変数を宣言

    fp = fopen("data.csv", "r"); // ファイルを開く。失敗するとNULLを返す。
    if (fp == NULL) {
        //DrawFormatString(0, 0, GetColor(0, 0, 0), "file not open!\n", "data.csv");
        return -1;
    }

    //csvファイルのデータがなくなるまでstamp関数を呼び出す
    while (fscanf(fp, "%d,%d,%d,%d,%d,%d", &year, &month, &day, &hour, &minute, &second) != EOF) {
        stamp(year, month, day, hour, minute, second);
    }

    fclose(fp); // ファイルを閉じる

    return 0;
}

//-----------------------------スタンプ座標計算関数------------------------------------------------------------------
void stamp(int year, int month, int day, int hour, int minute, int second) {
    //変数宣言と初期化
    int start[12] = { 7,3,3,6,1,4,6,2,5,7,3,5 };  //各月の開始x軸を保存する配列 
    int y, x;                                    //y座標とx座標を保存する変数
    int i=1;

    //座標を計算
    y = (start[month - 1] + day - 2) / 7 + 1;  //y座標を計算  
    x = (start[month - 1] + day - 2) % 7 + 1;   //x座標を計算

    while (i <= 8) {
        if (when[month][y][x][i].year == 0) {   //構造体配列の未入力添え字を探す
            when[month][y][x][i] = { year,month, day, hour,minute, second };    //見つけた添え字に時刻を保存
            break;
        }
        i++;
    }
}

//-----------------------------スタンプ描画関数------------------------------------------------------------------
void draw(int month) {
    //変数宣言
    int x, y;
    char pic[256];

    //月ごとに画像の参照場所を変更
    switch (month) {
    case 1:
        strcpy(pic, "stamp\\stamp1_p.png");
        break;
    case 2:
        strcpy(pic, "stamp\\stamp2_p.png");
        break;
    case 3:
        strcpy(pic, "stamp\\stamp3_p.png");
        break;
    case 4:
        strcpy(pic, "stamp\\stamp4_p.png");
        break;
    case 5:
        strcpy(pic, "stamp\\stamp5_p.png");
        break;
    case 6:
        strcpy(pic, "stamp\\stamp6_p.png");
        break;
    case 7:
        strcpy(pic, "stamp\\stamp7_p.png");
        break;
    case 8:
        strcpy(pic, "stamp\\stamp8_p.png");
        break;
    case 9:
        strcpy(pic, "stamp\\stamp9_p.png");
        break;
    case 10:
        strcpy(pic, "stamp\\stamp10_p.png");
        break;
    case 11:
        strcpy(pic, "stamp\\stamp11_p.png");
        break;
    case 12:
        strcpy(pic, "stamp\\stamp12_p.png");
        break;
    default:
        break;
    }

    //座標は以下の通りに変化
    //y=167・・・+64+64+64・・・489 　y座標の最小値は167,加算64,最大値489,最大要素数は6
    //x=56・・・+141+141+141・・・903 x座標の最小値は56,加算141,最大値903,最大要素数は7
    y = 167;
    for (int i = 1; i <= 6; i++) {
        x = 56;
        for (int j = 1; j <= 7; j++) {
            if (when[month][i][j][1].year >= 1) {           //もしスタンプデータがあれば
                LoadGraphScreen(x, y, pic, TRUE);  //スタンプを描画
            }
            x += 141;
        }
        y += 64;
    }
}
/*
 * WIN32APIでRS-232C通信
 * COM3ポートをオープンし、9600bps,データ長8ビット,パリティなし,ストップビット長1ビット,
 * RTSフローONで5秒間受信し、アドレスrecievedDataに受信したデータを格納します。
 * その後、「dummy data」という文字列を同じポートへ送信して終了します。
 */

int serial() {
    HANDLE comPort = CreateFile(_T("COM3"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL); // シリアルポートを開く
    DCB dcb; // シリアルポートの構成情報が入る構造体
    GetCommState(comPort, &dcb); // 現在の設定値を読み込み
    dcb.BaudRate = 9600; // 速度
    dcb.ByteSize = 8; // データ長
    dcb.Parity = NOPARITY; // パリティ
    dcb.StopBits = ONESTOPBIT; // ストップビット長
    dcb.fOutxCtsFlow = FALSE; // 送信時CTSフロー
    dcb.fRtsControl = RTS_CONTROL_ENABLE; // RTSフロー
    SetCommState(comPort, &dcb); // 変更した設定値を書き込み
    //Sleep(1000); // 受信バッファにメッセージが溜まるまで待つ
    DWORD errors; // エラーが起きた場合、エラーコードが入る
    COMSTAT comStat; // 通信状態バッファ
    ClearCommError(comPort, &errors, &comStat); // 入出力バッファの情報を通信状態バッファへ取り込む
    int lengthOfRecieved = comStat.cbInQue; // 通信状態バッファ内の受信したメッセージ長を取得する
    HANDLE hhp = HeapCreate(0, 0, 0); // ヒープハンドルを生成
    char* recievedData = (char*)HeapAlloc(hhp, 0, sizeof(char) * (lengthOfRecieved + 1)); // 受信したメッセージ長 + 終端文字分のデータバッファをヒープ上に確保
    DWORD numberOfGot; // 受信バッファから実際に読み取ったバイト数が入る
    ReadFile(comPort, recievedData, lengthOfRecieved, &numberOfGot, NULL); // 受信バッファからデータバッファへ取り込む
    recievedData[lengthOfRecieved] = '\0'; // 受信したデータに終端文字を追加
    char tmp = *recievedData;
    CloseHandle(comPort); // シリアルポートを閉じる
    HeapDestroy(hhp); // ヒープを解放

    return tmp - 48;
}