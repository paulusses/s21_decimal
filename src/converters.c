#include "s21_decimal.h"

int s21_from_int_to_decimal(int src, s21_decimal *dst) {
  int result = OK;
  if (dst) {
    memset(dst, 0, sizeof(s21_decimal));
    dst->bits[0] = src;
    if (src < 0) {
      set_sign(dst);
      dst->bits[0] = ~dst->bits[0] + 1;
    }
  } else {
    result = CONVERTATION_FAULT;
  }
  return result;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst) {
  int result = OK;
  if (dst) {
    memset(dst, 0, sizeof(s21_decimal));
  }
  if (isnan(src) || src == INFINITY || src == -INFINITY) {
    result = CONVERTATION_FAULT;
    if (src == -INFINITY) {
      dst->bits[3] = -2147483648;
    }
  }
  if ((fabs(src) > 0 && fabs(src) < 1e-28) || fabs(src) >= pow(2, 96)) {
    result = CONVERTATION_FAULT;
  }
  if (dst && (result != CONVERTATION_FAULT)) {
    char *str = calloc(129, sizeof(char));
    int scale = 0;
    if (src < 0) {
      set_sign(dst);
    }
    src = fabs(src);
    ftd(src, str, &scale);
    set_scale(dst, scale);
    transferto2(&str);
    for (size_t i = 0; i < strlen(str); i++) {
      if (str[strlen(str) - 1 - i] == '1') set_bit(dst, i);
    }
    free(str);
  } else {
    result = CONVERTATION_FAULT;
  }
  return result;
}

int s21_from_decimal_to_int(s21_decimal src, int *dst) {
  int result = OK;
  int signRes = get_sign(&src);
  if (dst) {
    s21_truncate(src, &src);
    *dst = src.bits[0];
    if (signRes) {
      *dst *= -1;
    }
    if (src.bits[1] != 0 || src.bits[2] != 0) {
      result = CONVERTATION_FAULT;
    }
  } else {
    result = CONVERTATION_FAULT;
  }
  return result;
}
int s21_from_decimal_to_float(s21_decimal src, float *dst) {
  int result = OK;
  if (dst != NULL) {
    long double x = 0.0;
    for (int i = 0; i < 96; i++) {
      int checkbit = get_bit(&src, i);
      x = x + (powl(2.0L, i) * checkbit);
    }
    int sign = get_bit(&src, 127);
    nullify_bit(&src, 127);
    int ra = get_scale(&src);
    while (ra) {
      x /= 10.0L;
      ra--;
    }
    if (sign) {
      x = -x;
    }
    *dst = (float)x;
  } else {
    result = CONVERTATION_FAULT;
  }
  return result;
}
