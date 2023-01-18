#include "s21_decimal.h"

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = OK;
  memset(result, 0, sizeof(s21_decimal));
  s21_bigdecimal v_1;
  s21_bigdecimal v_2;
  s21_bigdecimal v_3;
  memset(&v_3, 0, sizeof(s21_bigdecimal));
  convertToBig(&value_1, &v_1);
  convertToBig(&value_2, &v_2);
  s21_big_add(v_1, v_2, &v_3);
  convertToDec(result, &v_3);
  if (res == S21_INF) {
    if (get_sign(&value_1) == 1 || get_sign(&value_2) == 1) {
      res = S21_NEG_INF;
    } else {
      res = S21_INF;
    }
  }
  return res;
}

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = OK;
  memset(result, 0, sizeof(s21_decimal));
  s21_bigdecimal v_1;
  s21_bigdecimal v_2;
  s21_bigdecimal v_3;
  memset(&v_3, 0, sizeof(s21_bigdecimal));
  convertToBig(&value_1, &v_1);
  convertToBig(&value_2, &v_2);
  if ((res = s21_big_sub(v_1, v_2, &v_3)) == OK) {
    convertToDec(result, &v_3);
  }
  return res;
}

int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = OK;
  memset(result, 0, sizeof(s21_decimal));
  s21_bigdecimal v_1;
  s21_bigdecimal v_2;
  s21_bigdecimal v_3;
  memset(&v_3, 0, sizeof(s21_bigdecimal));
  convertToBig(&value_1, &v_1);
  convertToBig(&value_2, &v_2);
  if ((res = s21_big_mul(v_1, v_2, &v_3)) == OK) {
    convertToDec(result, &v_3);
  }
  return res;
}
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = OK;
  memset(result, 0, sizeof(s21_decimal));
  s21_bigdecimal v_1;
  s21_bigdecimal v_2;
  s21_bigdecimal v_3;
  memset(&v_3, 0, sizeof(s21_bigdecimal));
  convertToBig(&value_1, &v_1);
  convertToBig(&value_2, &v_2);
  if ((res = s21_big_div(v_1, v_2, &v_3)) == OK) {
    convertToDec(result, &v_3);
  }
  return res;
}
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result) {
  int res = OK;
  memset(result, 0, sizeof(s21_decimal));
  s21_bigdecimal v_1;
  s21_bigdecimal v_2;
  s21_bigdecimal v_3;
  memset(&v_3, 0, sizeof(s21_bigdecimal));
  convertToBig(&value_1, &v_1);
  convertToBig(&value_2, &v_2);
  if ((res = s21_big_mod(v_1, v_2, &v_3)) == OK) {
    convertToDec(result, &v_3);
  }
  return res;
}
