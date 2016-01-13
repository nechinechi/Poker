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


#define ahead(x) ((x%13 != 0) ? x-1 : x+12)
#define rear(x) ((x%13 != 12) ? x+1 : x-12)

//--------------------------------------------------------------------
//  関数宣言
//--------------------------------------------------------------------

void check_kind_pair (int hand[], int val[], int kindVal);
void check_number_pair (int hand[], int val[], int numVal);
void check_sequence_pair (int hand[], int val[], int seqVal);
int min(int arr[], int n);


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
  int point;
  int hand[5];
  int val[5] = {0};
  // int num[13] = {0};
  // int sut[4] = {0};
  // int seqVal = 6;
  // int ret_card;
  // int i, j;
  // int k;
  // int t;

  arr_copy(hand, hd, HNUM);
  point = poker_point(hand);
  if ( point >= P6 ) { return -1; }

  // for ( k = 0; k < HNUM; k++ ) { t = hand[k] % 13; num[t]++; }    // 数位
  // for ( k = 0; k < HNUM; k++ ) { t = hand[k] / 13; sut[t]++; }    // 種類

  // same kind pair
  check_kind_pair(hand, val, 3);

  // same number pair
  check_number_pair(hand, val, 4);

  // sequential number
  check_sequence_pair(hand, val, 4);

  // return minimum value card
  return min(val, HNUM);
}


//====================================================================
//  補助関数
//====================================================================

// int check_pair(int num[]) {
//   return 0;
// }

// int check_flash(int sut[]) {
//   int i;
//   for ( i = 0; i < 4; i++ ) { if ( sut[i] > 3 ) { return i; } }
//   return -1;
// }

void check_kind_pair (int hand[], int val[], int kindVal) {
  int i, j;

  for ( i = 0; i < HNUM; i++ ) {
    for ( j = i+1; j < HNUM; j++ ) {
      if ( hand[i] / 13 == hand[j] / 13 ) {
        val[i] += kindVal; val[j] += kindVal;
      }
    }
  }
}


void check_number_pair (int hand[], int val[], int numVal) {
  int i, j;

  for ( i = 0; i < HNUM; i++ ) {
    for ( j = i+1; j < HNUM; j++ ) {
      if ( hand[i] % 13 == hand[j] % 13 ) {
        val[i] += numVal; val[j] += numVal;
      }
    }
  }
}


void check_sequence_pair (int hand[], int val[], int seqVal) {
  int i, j;

  for ( i = 0; i < HNUM; i++ ) {
    for ( j = i+1; j < HNUM; j++ ) {
      if ( ahead(val[i]) == val[j] ) { val[i] += seqVal; val[j] += seqVal; }
        else if ( rear(val[i]) == val[j] ) { val[i] += seqVal; val[j] += seqVal; }
    }
  }
}


int min(int arr[], int n) {
  int i;
  int min = 0;

  for ( i = 1; i < n; i++ ) {
    if ( arr[i] < arr[min] ) { min = i; }
  }

  return min;
}




