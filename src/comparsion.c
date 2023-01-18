#include <stdio.h>

#include "s21_decimal.h"

int s21_is_equal(s21_decimal a, s21_decimal b) {
  equal_degree(&a, &b);
  int result = 0;
  if ((get_sign(&a) == get_sign(&b) && a.bits[0] == b.bits[0] &&
       a.bits[1] == b.bits[1] && a.bits[2] == b.bits[2]) ||
      (a.bits[0] == 0 && b.bits[0] == 0 && a.bits[1] == 0 && b.bits[1] == 0 &&
       a.bits[2] == 0 && b.bits[2] == 0)) {
    result = 1;
  }
  return result;
}
int s21_is_not_equal(s21_decimal a, s21_decimal b) {
  return !s21_is_equal(a, b);
}

int s21_is_less(s21_decimal a, s21_decimal b) {
  s21_bigdecimal big_a, big_b;
  convertToBig(&a, &big_a);
  convertToBig(&b, &big_b);
  big_equal_degree(&big_a, &big_b);
  int result = 0;
  int sign1 = big_get_sign(&big_a), sign2 = big_get_sign(&big_b);
  int is_zero = -1;
  for (int i = 8 * 32 - 32 - 1; i >= 0; i--) {
    if (big_get_bit(&big_a, i) < big_get_bit(&big_b, i)) {
      result = 1;
      break;
    } else if (big_get_bit(&big_a, i) > big_get_bit(&big_b, i)) {
      result = 0;
      break;
    }
    if (i == 0 && is_zero != 0 && !big_get_bit(&big_a, i) &&
        !big_get_bit(&big_b, i))
      is_zero = 1;
    else if (big_get_bit(&big_a, i) || big_get_bit(&big_b, i))
      is_zero = 0;
  }
  if (is_zero == 1)
    result = 0;
  else if (sign1 > sign2)
    result = 1;
  else if (sign1 < sign2)
    result = 0;
  else if (sign1 == sign2 && sign1 == 1 &&
           (big_a.bits[0] != big_b.bits[0] || big_a.bits[1] != big_b.bits[1] ||
            big_a.bits[2] != big_b.bits[2] || big_a.bits[3] != big_b.bits[3] ||
            big_a.bits[4] != big_b.bits[4] || big_a.bits[5] != big_b.bits[5] ||
            big_a.bits[6] != big_b.bits[6])) {
    result = !result;
  }
  return result;
}

int s21_is_greater_or_equal(s21_decimal a, s21_decimal b) {
  return !s21_is_less(a, b);
}

int s21_is_greater(s21_decimal a, s21_decimal b) {
  s21_bigdecimal big_a, big_b;
  convertToBig(&a, &big_a);
  convertToBig(&b, &big_b);
  big_equal_degree(&big_a, &big_b);
  int result = 0;
  int sign1 = big_get_sign(&big_a), sign2 = big_get_sign(&big_b);
  int is_zero = -1;
  for (int i = 8 * 32 - 32 - 1; i >= 0; i--) {
    if (big_get_bit(&big_a, i) > big_get_bit(&big_b, i)) {
      result = 1;
      break;
    } else if (big_get_bit(&big_a, i) < big_get_bit(&big_b, i)) {
      result = 0;
      break;
    }
    if (i == 0 && is_zero != 0 && !big_get_bit(&big_a, i) &&
        !big_get_bit(&big_b, i))
      is_zero = 1;
    else if (big_get_bit(&big_a, i) || big_get_bit(&big_b, i))
      is_zero = 0;
  }
  if (is_zero == 1)
    result = 0;
  else if (sign1 > sign2)
    result = 0;
  else if (sign1 < sign2)
    result = 1;
  else if (sign1 == sign2 && sign1 == 1 &&
           (big_a.bits[0] != big_b.bits[0] || big_a.bits[1] != big_b.bits[1] ||
            big_a.bits[2] != big_b.bits[2] || big_a.bits[3] != big_b.bits[3] ||
            big_a.bits[4] != big_b.bits[4] || big_a.bits[5] != big_b.bits[5] ||
            big_a.bits[6] != big_b.bits[6])) {
    result = !result;
  }
  return result;
}

int s21_is_less_or_equal(s21_decimal a, s21_decimal b) {
  return !s21_is_greater(a, b);
}