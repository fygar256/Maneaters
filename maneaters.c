//
//  昔懐かし、CBM-3032のようなキャラクタゲームを
//  ncurseswで実現。
//  Maneaters. Ver 1.3
//

#include  <stdio.h>
#include  <stdlib.h>
#include  <time.h>

//  ncurseswを使うには、以下の２つのヘッダファイル
#include  <ncursesw/ncurses.h>
#include  <locale.h>

//  出力する文字を、定数として定義しています。
#define ENEMY   "Ｏ"
#define PLAYER  "＠"
#define ROCK    "＃"
#define CRASH   "Ｘ"
#define SPACE   "　"

//  0〜xまでの乱数を発生させる関数の定義。
#define rnd(x) (rand()%x)

int enemyl[40];   //  敵が生きているかのフラグ
int enemyx[40];   //  敵ｘ座標
int enemyy[40];   //  敵ｙ座標

int rockl[320];   //  岩があるかどうかのフラグ
int rockx[320];   //  岩ｘ座標
int rocky[320];   //  岩ｙ座標

int enemymax=12;  // 最初の敵の数

int rockmax=120;  // 最初の岩の数

int playerx,playery;  // プレイヤー座標

int h,w;              // 画面height,width

int xsize,ysize;      // ゲーム画面サイズ。
                      // 80x25の画面を、等幅フォントの正方形の全角UTF-8
                      // で、使っているので、
                      // ここでは、xsizeは、40となります。

int gamestat;         // ゲームの状態： ０　プレイ中
                      //                １　負け
                      //              　３　勝ち

/*
 * プレイヤーが動いた時、
 * 何かに当たったかどうかの判断
 * 返り値; １：衝突　０：何もなし
 */
int check_player_crash() {
  //
  //  敵に当たったかどうかの判定
  //
  for(int i=0;i<enemymax;i++) {
    if((enemyl[i]==1)&&(playerx==enemyx[i]) && (playery==enemyy[i]))
      return(1);
    }
  //
  //  岩に当たったかどうかの判定
  //
  for(int i=0;i<rockmax;i++) {
    if((rockl[i]==1)&&(playerx==rockx[i]) && (playery==rocky[i]))
      return(1);
    }
  return(0);
}

/*
 * 画面に文字を表示する関数。
 * 全角単位で扱っているので、
 * x座標を２倍しています。
 */
int putchara(int x,int y,char *str){
  move(y,x*2);
  addstr(str);
}

/*
 * ゲームの初期化
 */
int init_game() {
  int i;
  clear();            // 画面クリア

  gamestat=0;         // ゲーム状態を初期化

  // 乱数の種を、timeから取り、初期化。
  srand((unsigned)time(NULL));

  //
  //  岩を置く。
  //  0番目はプレイヤーと敵との衝突回避のため、
  //  ダミーを画面中心に置く。そのため、岩の個数は、
  //  rockmax-1となる。
  // 
  rockx[0]=xsize/2;
  rocky[0]=ysize/2;
  rockl[0]=0;
  for(i=1;i<rockmax;i++) {
    putchara(rockx[i]=rnd(xsize),rocky[i]=rnd(ysize),ROCK);
    rockl[i]=1;
    }

  //  敵を岩とぶつからないように置く。
  i=0;
  while(i<enemymax) {
    int ex,ey,f;
    ex = rnd(xsize);
    ey = rnd(ysize);
    f = 0;
    for(int j=0;j<rockmax;j++) {
      if ((ex==rockx[j])&&(ey==rocky[j]))
        f=f|1;
      }
    if (f==0) {
      putchara(enemyx[i]=ex,enemyy[i]=ey,ENEMY);
      enemyl[i]=1;
      i++;
      }
    }

  // プレイヤーを画面中心に置く。
  putchara(playerx=xsize/2,playery=ysize/2,PLAYER);

}

/*
 * 敵達を動かす
 */
void  move_maneaters() {
  int dx,dy;
  int x,y;

  for(int i=0;i<enemymax;i++) {
    if (enemyl[i]==0)   // 敵が死んでいたら、スキップ
          continue;
    //
    //  プレイヤーを追いかけるように差分を求める
    //
    dx=0,dy=0;
    if (enemyx[i]<playerx) dx=1;
    if (enemyx[i]>playerx) dx=-1;
    if (enemyy[i]<playery) dy=1;
    if (enemyy[i]>playery) dy=-1;

    //  今の位置の敵の姿を消す
    putchara(enemyx[i],enemyy[i],SPACE);

    //  敵の動いた先の座標を求める
    x=enemyx[i]+dx,y=enemyy[i]+dy;

    //
    //  敵同士の衝突のチェック
    //
    for(int j=0;j<enemymax;j++) {
      if ((i!=j)&&(enemyl[j]==1) && (x==enemyx[j]) && (y==enemyy[j])) {
          enemyl[i]=0;          // 敵が生きているフラグを降ろす。
          }
      }

    //
    //  岩との衝突チェック
    //
    for(int j=0;j<rockmax;j++) {
      if ((rockl[j]==1)&&(x==rockx[j]) && (y==rocky[j])) { // 岩に当たったか
        rockl[j]=enemyl[i]=0;
        putchara(x,y,SPACE);  // 岩を消す
        }
      }

    enemyx[i]=x,enemyy[i]=y;  // 敵を一歩進める
    if (enemyl[i])    // 敵が生きているか？
      putchara(enemyx[i],enemyy[i],ENEMY);  // 敵を描画

    //
    //  プレイヤーと衝突したかどうかチェック
    //
    if ((x==playerx) && (y==playery))
        gamestat=1;     // 衝突したならば、負け
    }

}

//
//  ゲームのメインループ
//
int main_game() {
  int f,dx,dy;
  while(1) {

/*
 *  プレイヤー処理
 */
  switch(getch()) {     // プレイヤーキー入力と移動方向の取得
    case  '7': dx=-1,dy=-1; break;
    case  '8': dx= 0,dy=-1; break;
    case  '9': dx= 1,dy=-1; break;
    case  'u': case  '4': dx=-1,dy=0; break;
    case  'i': case  '5': dx= 0,dy=0; break;
    case  'o': case  '6': dx= 1,dy=0; break;
    case  'j': case  '1': dx=-1,dy=1; break;
    case  'k': case  '2': dx= 0,dy=1; break;
    case  'l': case  '3': dx= 1,dy=1; break;
    default:
      continue;
    }

    // プレイヤーが画面からはみ出ないか？
    if (((playerx+dx)>=0) && ((playerx+dx)<xsize) &&
        ((playery+dy)>=0) && ((playery+dy)<ysize)) {

      putchara(playerx,playery,SPACE);  //  プレイヤーを一歩進ませる。
      playerx+=dx;
      playery+=dy;
      putchara(playerx,playery,PLAYER);

      // プレイヤーが、岩や敵に当たったら、ゲームエンド。
      if (check_player_crash()) {
              gamestat=1;
              return(0);
            }
      }
    /*
     *  敵処理
     */
    move_maneaters();
    if (gamestat!=0) return(0);

    /*
     * マンイーターが全滅していたら勝ち
     */
    f=0;
    for(int j=0;j<enemymax;j++)
      f|=enemyl[j];
    if (f==0) {
      gamestat=3;
      return(0);
      }
  }
}
    
/*
 * もう一度プレイするかどうかを聞く関数
 * 返り値；０：しない　１：する
 */
int tryagainp() {
  printw("Try Again? [y/n]");
  while(1) {
    switch(getch()) {
      case  'n': return(0);
      case  'y': return(1);
      default: continue;
      }
    }
}

/*
 *  ゲームの説明画面
 *
 */
void  instruction() {
    clear();      // 画面のクリア

    printw("Maneaters Ver 1.3\n"); // printwで、stdscrに出力します。

    printw("Mission : マンイーターを消して生き残れ！\n");
    printw("%s -- Maneater, 段階的に追い詰める敵\n",ENEMY);
    printw("%s -- Rock, 触るとマンイーターや自分が死ぬ \n",ROCK);
    printw("%s -- Player, コントロールしてマンイーターを岩に当て、\n",PLAYER);
    printw("              生き残れ！",PLAYER);
    printw("\n");
    printw("Key control:    　        Tenkey:     \n");
    printw(" ７  ８  ９            ７　８  ９   \n");
    printw("  ↖ ↑  ↗                 ↖ ↑  ↗     \n");
    printw("ｕ← ｉ →ｏ             ４← ５→ ６   \n");
    printw("  ↙ ↓  ↘                 ↙ ↓  ↘     \n");
    printw(" ｊ  ｋ   ｌ           １  ２  ３   \n");
    printw("\n");
    printw(" 'i' と '5' は、マンイーターのみが動き、自分は動かない\n");
    printw(" このプログラムは、Ctrl-Cで止まります.\n");
    printw("             Good Luck\n");
    printw("キーを押してスタート。\n");

    getch();    // 一文字キー入力待ち。
  }

//
//  ゲームの説明画面とメインループのループ
//
void  play_game() {
  while(1) {

    instruction();    //  ゲーム説明画面を呼び出す。

    init_game();      //  ゲームの初期化

    main_game();      //  ゲームのメインループ

    if (gamestat==1) {  //  プレイヤー負け
      putchara(playerx,playery,CRASH);
      move(0,0);
      printw("You Lose ");
      }

    if (gamestat==3) {  // プレイヤー勝ち
      move(0,0);
      printw("You Win!! ");
      }

    if (tryagainp()==0) return; // リプレイしなければリターン。
    }
}
/*
 * main関数
 */
int main() {
  setlocale( LC_ALL,"" ); // ncurseswで、UTF-8 を使うために、
                          // LC_ALLをセットする。

  initscr();              // 画面の初期化

  noecho();               // キーボードエコーをなしにする

  curs_set(0);            // カーソルを非表示にする。

  getmaxyx(stdscr,h,w);   // 画面サイズを、h,wに格納する
                          //  普通、80x25

  xsize=w/2;              //  ゲームで使う画面サイズを求める。

  ysize=h;                //  ゲームで使う画面のYサイズを求める。

  play_game();            //  ゲームを走らせる。

  endwin();               //  画面を閉じる。

  exit(0);                //  終了。
}

