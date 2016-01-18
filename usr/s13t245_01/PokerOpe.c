//====================================================================
//  工学部「情報環境実験２」(富永)  C言語プログラミング  例題
//  ポーカーゲームの戦略
//--------------------------------------------------------------------
//  Poker  usr/s13t200_03/  PokerOpe.c
//  Linux CentOS 6.7  GCC 4.4.7
//--------------------------------------------------------------------
//  富永研究室  tominaga 富永浩之
//  2015.11.17
//====================================================================


//====================================================================
//  仕様
//====================================================================

/*--------------------------------------------------------------------

手札、場札、チェンジ数、テイク数、捨札を引数とし、捨札を決める。
返却値は、捨札の位置である。-1のときは、交換しないで、手札を確定させる。
関数 strategy() は、戦略のインタフェースであり、この関数内で、
実際の戦略となる関数を呼び出す。手札と捨札は、不正防止のため、
変更不可なので、局所配列にコピーして、整列などの処理を行う。
複数の戦略を比較したり、パラメタを変化させて、より強い戦略を目指す。

---------------------------------------------------------------------*/


//====================================================================
//  前処理
//====================================================================

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Poker.h"

/*
    1 1 1 1
    C D H S
*/
#define sutS 0
#define sutH 1
#define sutD 2
#define sutC 3
#define bitS 1
#define bitH 2
#define bitD 4
#define bitC 8
#define ahead(x) ((x%13 != 0) ? x-1 : x+12)
#define rear(x) ((x%13 != 12) ? x+1 : x-12)

typedef struct _Node {   // 手札カードのインデックスを格納
  int index;
  struct _Node *next;
} Node;

typedef struct {        // 手札カードの情報を保持
  int num;
  int sut;
  Node *node;
} State;

//--------------------------------------------------------------------
//  関数宣言
//--------------------------------------------------------------------

void get_state(int hand[], State state[]);
void count_suite_pair(int hand[], int sut[]);
int min(int arr[], int n);
void plus_value(Node *node, int val[], int t);
void set_value_4number(State state[], int val[]);
void set_value_4straight(State state[], int val[]);

//====================================================================
//  戦略
//====================================================================

/*--------------------------------------------------------------------
//  ユーザ指定
//--------------------------------------------------------------------

最初の手札のまま交換しない。

hd : 手札配列
fd : 場札配列(テイク内の捨札)
cg : チェンジ数
tk : テイク数
ud : 捨札配列(過去のテイクも含めた全ての捨札)
us : 捨札数

--------------------------------------------------------------------*/

int strategy(const int hd[], const int fd[], int cg, int tk, const int ud[], int us)
{
  int point;           // 現在の手札の点数
  int hand[5];         // 手札のコピー
  State state[13] = {{0, 1, NULL}};    // 手札の状態
  int sut[5] = {0};    // 種類の同じカードの数
  int val[5] = {0};    // カードの重み
  int sutVal;
  int i;

  arr_copy(hand, hd, HNUM);
  point = poker_point(hand);
  if ( point >= P6 ) { return -1; }

  get_state(hand, state);

  // same kind pair
  count_suite_pair(hand, sut);
  for ( i = 0; i < HNUM; i++ ) {
    // kind pair
    switch ( sut[i] ) {
      case 3 : sutVal = 4;  break;     // 3 kind pair
      case 4 : sutVal = 24;  break;    // 4 kind pair
      default : sutVal = 0;
    }
    val[i] += sutVal;
  }

  // same number pair
  set_value_4number(state, val);

  // set value sequential
  set_value_4straight(state, val);

/*
  // sequential number
  count_sequence_pair(hand, seq);
*/

  // return minimum value card
  return min(val, HNUM);
}


//====================================================================
//  補助関数
//====================================================================


void get_state(int hand[], State state[])
{
  int t;
  Node new;
  Node *node, *prev;
  int i;

  for ( i = 0; i < HNUM; i++ ) {
    t = hand[i] % 13;        // カードの数字を算出
    state[t].num++;          // カードの数字の枚数
    switch ( hand[i] / 13 ) {    // カードの種類を算出
      case sutS : state[t].sut += bitS; break;
      case sutH : state[t].sut += bitH; break;
      case sutD : state[t].sut += bitD; break;
      case sutC : state[t].sut += bitC; break;
    }

    //---- 新たなノードを生成
    new.index = i;     // インデックスを格納
    new.next = NULL;   // 次のノードへのポインタを初期化

    //---- ノードを連結
    if ( state[i].node == NULL ) {
      state[i].node = &new;     // ノードへのポインタを格納
    } else {
      node = state[i].node;             // 最初のノードへ遷移
      while ( node->next != NULL ) {    // 
        // prev = node;
        node = node->next;
      }
      node->next = &new;        // ノードへのポインタを格納
    }
  }
}


void count_suite_pair(int hand[], int sut[])
{
  int i, j;

  for ( i = 0; i < HNUM; i++ ) {
    for ( j = i+1; j < HNUM; j++ ) {
      if ( hand[i] / 13 == hand[j] / 13 ) {
        sut[i]++; sut[j]++;
      }
    }
  }
}


void plus_value(Node *node, int val[], int t)
{
  while ( node != NULL ) {
    val[node->index] += t;
    node = node->next;
  }
}


void set_value_4number(State state[], int val[])
{
  Node *node;
  int i;

  for ( i = 0; i < 13; i++ ) {
    node = state[i].node;
    switch ( state[i].num ) {
      case 3 : plus_value(node, val, 3); break;
      case 4 : plus_value(node, val, 5); break;
    }
  }
}


void set_value_4straight(State state[], int val[])
{
  Node *node;
  int start, end;
  int i, j;

  for ( i = 0; i < 13; i++ ) {
    if ( state[i].num ) {
      if ( start ) { end++; }
        else { end = start = i; }
    } else {
      if ( end-start+1 > 4 ) {
        for ( j = start; j <= end; j++ ) {
          node = state[j].node;
          plus_value(node, val, 15);
        }
      }
    }
  }
}


int min(int arr[], int n)
{
  int i;
  int min = 0;

  for ( i = 1; i < n; i++ ) {
    if ( arr[i] < arr[min] ) { min = i; }
  }

  return min;
}




