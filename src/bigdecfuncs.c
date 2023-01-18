#include "s21_decimal.h"

void print_big_decimal(s21_bigdecimal src) {
  for (int i = 255; i >= 0; i--) {
    printf("%d", big_get_bit(&src, i));
    if (i == 224 || i == 192 || i == 160 || i == 128 || i == 96 || i == 64 ||
        i == 32) {
      printf(" ");
    }
  }
}

int big_add(s21_bigdecimal value_1, s21_bigdecimal value_2,
            s21_bigdecimal *result) {
  int res = OK;
  int dop_bit = 0;
  memset(result, 0, sizeof(s21_bigdecimal));
  for (int i = 0; i < 224; i++) {
    int bit_1 = big_get_bit(&value_1, i);
    int bit_2 = big_get_bit(&value_2, i);
    if (bit_1 == 1 && bit_2 == 1) {
      if (dop_bit == 0) {
        big_nullify_bit(result, i);
        dop_bit = 1;  // 1
      } else if (dop_bit == 1) {
        big_set_bit(result, i);
        dop_bit = 1;  // 1
      }
    } else if (bit_1 != bit_2) {
      if (dop_bit == 0) {
        big_set_bit(result, i);
        dop_bit = 0;  // 0
      } else if (dop_bit == 1) {
        big_nullify_bit(result, i);
        dop_bit = 1;  // 1
      }
    } else {
      if (dop_bit == 0) {
        big_nullify_bit(result, i);
        dop_bit = 0;  // 0
      } else if (dop_bit == 1) {
        big_set_bit(result, i);
        dop_bit = 0;  // 0
      }
    }
    if ((big_get_bit(result, 223) == 1) && dop_bit == 1) {
      res = S21_INF;
    }
  }

  return res;
}
void convertToBig(s21_decimal *src, s21_bigdecimal *dst) {
  memset(dst, 0, sizeof(s21_bigdecimal));
  dst->bits[0] = src->bits[0];
  dst->bits[1] = src->bits[1];
  dst->bits[2] = src->bits[2];
  dst->bits[7] = src->bits[3];
}
void convertToDec(s21_decimal *src, s21_bigdecimal *dst) {
  src->bits[0] = dst->bits[0];
  src->bits[1] = dst->bits[1];
  src->bits[2] = dst->bits[2];
  src->bits[3] = dst->bits[7];
}
void big_inverse_Bit(s21_bigdecimal *d, int position) {
  d->bits[position / 32] ^= 1U << (position % 32);
}
void big_set_bit(s21_bigdecimal *src, int position) {
  src->bits[position / 32] |= 1U << (position % 32);
}
void big_set_scale(s21_bigdecimal *src, int scalevalue) {
  src->bits[7] = (big_get_bit(src, 255) << 31) | (scalevalue << 16);
}
void big_nullify_bit(s21_bigdecimal *src, int position) {
  src->bits[position / 32] &= ~(1U << (position % 32));
}
int big_get_scale(s21_bigdecimal *src) {
  return (0b11111111 << 16 & src->bits[7]) >> 16;
}
void big_nullify_scale(s21_bigdecimal *src) { src->bits[7] &= (~16711680); }
int big_get_bit(s21_bigdecimal *src, int position) {
  return !!(src->bits[position / 32] & (1U << (position % 32)));
}
void big_set_sign(s21_bigdecimal *src) { big_set_bit(src, 255); }
int big_get_sign(s21_bigdecimal *src) { return big_get_bit(src, 255); }
int s21_big_add(s21_bigdecimal value_1, s21_bigdecimal value_2,
                s21_bigdecimal *result) {
  memset(result, 0, sizeof(s21_bigdecimal));
  int res = OK;
  int sign1 = big_get_sign(&value_1);
  int sign2 = big_get_sign(&value_2);
  int scale1 = big_get_scale(&value_1);
  int scale2 = big_get_scale(&value_2);
  int scale;
  if (scale1 != scale2) {
    big_equal_degree(&value_1, &value_2);
  }
  if (sign1 == 0 && sign2 == 0) {
    // big_set_sign(&value_1);
    big_set_sign(&value_2);
    s21_big_sub(value_1, value_2, result);
  } else if (sign1 == 1 && sign2 == 0) {
    big_nullify_bit(&value_1, 255);
    if (s21_big_is_greater(value_1, value_2)) {
      big_sub(value_1, value_2, result);
      big_set_sign(result);
    } else if (s21_big_is_greater(value_2, value_1)) {
      big_sub(value_2, value_1, result);
    }
  } else if (sign1 == 0 && sign2 == 1) {
    big_nullify_bit(&value_2, 255);
    if (s21_big_is_greater(value_1, value_2)) {
      big_sub(value_1, value_2, result);
    } else if (s21_big_is_greater(value_2, value_1)) {
      big_sub(value_2, value_1, result);
      big_set_sign(result);
    }
  } else {
    big_nullify_bit(&value_2, 255);
    big_nullify_bit(&value_1, 255);
    big_add(value_1, value_2, result);
    big_set_sign(result);
  }

  if ((scale1 > scale2) || (scale1 == scale2)) {
    scale = scale1;
  } else {
    scale = scale2;
  }
  big_set_scale(result, scale);
  if (scale1 + scale2 > 28) {
    res = S21_NEG_INF;
  }
  if (res == S21_INF) {
    if (big_get_sign(&value_1) == 1 || big_get_sign(&value_2) == 1) {
      res = S21_NEG_INF;
    } else {
      res = S21_INF;
    }
  }
  return res;
}
void big_equal_degree(s21_bigdecimal *a, s21_bigdecimal *b) {
  int a_scale = big_get_scale(a), b_scale = big_get_scale(b);
  if (a_scale > b_scale) {
    a_scale -= b_scale;
    b_scale = 0;
  } else {
    b_scale -= a_scale;
    a_scale = 0;
  }
  int scale1 = big_get_scale(a);
  int scale2 = big_get_scale(b);
  while (a_scale != 0 || b_scale != 0) {
    if (a_scale != 0) {
      big_mul_by_10(*b, b);
      a_scale--;
    } else {
      big_mul_by_10(*a, a);
      b_scale--;
    }
  }
  if (scale1 != 0) {
    big_set_scale(b, scale1);
  } else {
    big_set_scale(a, scale2);
  }
}
void big_mul_by_10(s21_bigdecimal a, s21_bigdecimal *result) {
  memset(result, 0, sizeof(s21_bigdecimal));
  int sign = big_get_sign(&a), scale = big_get_scale(&a);
  s21_bigdecimal a2 = {{a.bits[0], a.bits[1], a.bits[2], a.bits[3], a.bits[4],
                        a.bits[5], a.bits[6], 0}};
  big_l_shift(&a2, 1);
  s21_big_add(*result, a2, result);
  big_l_shift(&a2, 2);
  s21_big_add(*result, a2, result);
  if (sign == 1) {
    big_set_sign(result);
  }
  big_set_scale(result, scale);
}
int s21_big_is_greater(s21_bigdecimal a, s21_bigdecimal b) {
  big_equal_degree(&a, &b);
  int result = 0;
  if (big_get_sign(&a) < big_get_sign(&b)) {
    result = 1;
  } else if (big_get_sign(&a) > big_get_sign(&b)) {
    result = 0;
  } else {
    if (a.bits[6] > b.bits[6] || a.bits[5] > b.bits[5] ||
        a.bits[4] > b.bits[4] || a.bits[3] > b.bits[3] ||
        a.bits[2] > b.bits[2] || a.bits[1] > b.bits[1] ||
        a.bits[0] > b.bits[0]) {
      result = 1;
    } else {
      result = 0;
    }
    if (big_get_sign(&a) == 1) {
      result = !result;
    }
  }
  return result;
}
void big_l_shift(s21_bigdecimal *src, int shift) {
  for (int i = 0; i < shift; i++) {
    int c1 = 0;
    int c2 = 0;
    int c3 = 0;
    int c4 = 0;
    int c5 = 0;
    int c6 = 0;
    if (big_get_bit(src, 31) == 1) {
      c1 = 1;
    }
    if (big_get_bit(src, 63) == 1) {
      c2 = 1;
    }
    if (big_get_bit(src, 95) == 1) {
      c3 = 1;
    }
    if (big_get_bit(src, 127) == 1) {
      c4 = 1;
    }
    if (big_get_bit(src, 159) == 1) {
      c5 = 1;
    }
    if (big_get_bit(src, 191) == 1) {
      c6 = 1;
    }
    src->bits[0] <<= 1;
    src->bits[1] <<= 1;
    if (c1 == 1) {
      big_set_bit(src, 32);
    }
    src->bits[2] <<= 1;
    if (c2 == 1) {
      big_set_bit(src, 64);
    }
    src->bits[3] <<= 1;
    if (c3 == 1) {
      big_set_bit(src, 96);
    }
    src->bits[4] <<= 1;
    if (c4 == 1) {
      big_set_bit(src, 128);
    }
    src->bits[5] <<= 1;
    if (c5 == 1) {
      big_set_bit(src, 160);
    }
    src->bits[6] <<= 1;
    if (c6 == 1) {
      big_set_bit(src, 192);
    }
  }
}
int big_sub(s21_bigdecimal value_1, s21_bigdecimal value_2,
            s21_bigdecimal *result) {
  int res = OK;
  s21_bigdecimal one = {{1, 0, 0, 0, 0, 0, 0, 0}};
  memset(result, 0, sizeof(s21_bigdecimal));

  if (s21_big_is_greater(value_1, value_2)) {
    for (int i = 0; i < 224; i++) {
      big_inverse_Bit(&value_2, i);
    }
    res = big_add(value_2, one, result);
    res = big_add(value_1, *result, result);
  }
  return res;
}
int s21_big_sub(s21_bigdecimal value_1, s21_bigdecimal value_2,
                s21_bigdecimal *result) {
  memset(result, 0, sizeof(s21_bigdecimal));
  int res = OK;
  int sign1 = big_get_sign(&value_1);
  int sign2 = big_get_sign(&value_2);
  int scale1 = big_get_scale(&value_1);
  int scale2 = big_get_scale(&value_2);
  int scale;
  if (scale1 != scale2) {
    big_equal_degree(&value_1, &value_2);
  }
  if (sign1 > sign2) {
    res = big_add(value_1, value_2, result);
    big_set_sign(result);
  } else if (sign1 < sign2) {
    big_nullify_bit(&value_2, 255);
    res = big_add(value_1, value_2, result);
  } else if (sign1 == 1 && sign2 == 1) {
    if (s21_big_is_greater(value_1, value_2)) {
      big_nullify_bit(&value_2, 255);
      big_nullify_bit(&value_1, 255);
      res = big_sub(value_2, value_1, result);
    } else if (s21_big_is_greater(value_2, value_1)) {
      big_nullify_bit(&value_1, 255);
      res = s21_big_add(value_2, value_1, result);
      big_set_sign(result);
    }
  } else {
    if (s21_big_is_greater(value_1, value_2)) {
      res = big_sub(value_1, value_2, result);
    } else if (s21_big_is_greater(value_2, value_1)) {
      res = big_sub(value_2, value_1, result);
      big_set_sign(result);
    }
  }
  if ((scale1 > scale2) || (scale1 == scale2)) {
    scale = scale1;
  } else if (scale2 > scale1) {
    scale = scale2;
  }
  big_set_scale(result, scale);
  return res;
}
int s21_big_mul(s21_bigdecimal value_1, s21_bigdecimal value_2,
                s21_bigdecimal *result) {
  memset(result, 0, sizeof(s21_bigdecimal));
  int scale1 = big_get_scale(&value_1);
  int scale2 = big_get_scale(&value_2);
  int sign1 = big_get_sign(&value_1);
  int sign2 = big_get_sign(&value_2);
  big_set_scale(&value_1, 0);
  big_set_scale(&value_2, 0);
  big_nullify_bit(&value_1, 255);
  big_nullify_bit(&value_2, 255);
  for (int i = 0; i < 224; i++) {
    if (big_get_bit(&value_2, i)) {
      s21_bigdecimal temp = {{value_1.bits[0], value_1.bits[1], value_1.bits[2],
                              value_1.bits[3], value_1.bits[4], value_1.bits[5],
                              value_1.bits[6], value_1.bits[7]}};
      big_l_shift(&temp, i);
      s21_big_add(*result, temp, result);
    }
  }
  big_set_scale(result, scale1 + scale2);
  if (!((sign1 && sign2) || (!sign1 && !sign2))) {
    big_set_sign(result);
  }
  return 0;
}
int s21_big_mod(s21_bigdecimal value_1, s21_bigdecimal value_2,
                s21_bigdecimal *result) {
  int res = OK;
  s21_bigdecimal tmp;
  int sign = big_get_sign(&value_1);
  big_nullify_bit(&value_1, BIG_BITS - 1);
  big_nullify_bit(&value_2, BIG_BITS - 1);
  memset(result, 0, sizeof(s21_bigdecimal));
  if (is_zero_big_decimal(&value_2)) {
    res = S21_NAN;
  } else {
    if ((res = normalize(&value_1, &value_2)) == OK) {
      int scale1 = big_get_scale(&value_1);
      int scale2 = big_get_scale(&value_2);
      int scale = (scale1 > scale2) ? scale1 : scale2;
      simple_big_div(value_1, value_2, &tmp);
      s21_big_mul(value_2, tmp, result);
      s21_big_sub(value_1, *result, result);
      big_set_scale(result, scale);
      if (sign) {
        big_set_bit(result, BIG_BITS - 1);
      }
    }
  }
  return res;
}
int int_part_big(s21_bigdecimal big_1, s21_bigdecimal big_2,
                 s21_bigdecimal *result, s21_bigdecimal *rest) {
  int res = OK;
  memset(rest, 0, sizeof(s21_bigdecimal));
  memset(result, 0, sizeof(s21_bigdecimal));
  big_nullify_scale(&big_1);
  big_nullify_scale(&big_2);
  for (int i = BIG_MANTISSA_BITS - 1; i >= 0; i--) {
    big_l_shift(rest, 1);
    if (big_get_bit(&big_1, i)) {
      big_set_bit(rest, 0);
    }
    if (s21_is_big_less_or_equal(big_2, *rest)) {
      big_set_bit(result, i);
      s21_big_sub(*rest, big_2, rest);
    }
  }
  return res;
}
int is_zero_big_decimal(s21_bigdecimal *src) {
  int res = 1;
  for (size_t i = 0; i < 7; ++i) {
    if (src->bits[i] != 0) {
      res = 0;
    }
  }
  return res;
}
void fract_part_big(s21_bigdecimal div, s21_bigdecimal den,
                    s21_bigdecimal *result) {
  size_t shift = 0;
  s21_bigdecimal temp_ten = create_big(10, 0, 0, 0, false);
  s21_bigdecimal temp_div = create_big(0, 0, 0, 0, false);
  while (!is_zero_big_decimal(&div) && shift < 28) {
    s21_big_mul(*result, temp_ten, result);
    s21_big_mul(div, temp_ten, &div);
    int_part_big(div, den, &temp_div, &div);
    s21_big_add(*result, temp_div, result);
    shift++;
  }
  big_set_scale(result, shift);
}
s21_bigdecimal create_big(const int l_b, const int m1_b, const int m2_b,
                          const int scale, const bool sign) {
  s21_bigdecimal temp = {
      {l_b, m1_b, m2_b, 0, 0, 0, 0, ((scale << 16) | (sign << 31))}};
  return temp;
}
int normalize(s21_bigdecimal *a, s21_bigdecimal *b) {
  int res = OK;
  int scale = big_get_scale(a) - big_get_scale(b);
  s21_bigdecimal temp = create_big(10, 0, 0, 1, false);
  if (scale < 0) {
    for (size_t i = 0; i < (size_t)abs(scale) && res == OK; i++) {
      res = s21_big_mul(*a, temp, a);
    }
  } else if (scale > 0) {
    for (size_t i = 0; i < (size_t)abs(scale) && res == OK; i++) {
      res = s21_big_mul(*b, temp, b);
    }
  }
  return res;
}
int divide(s21_bigdecimal big_1, s21_bigdecimal big_2, s21_bigdecimal *result) {
  int res = OK;
  memset(result, 0, sizeof(s21_bigdecimal));
  s21_bigdecimal rest = create_big(0, 0, 0, 0, false);
  if ((res = int_part_big(big_1, big_2, result, &rest)) == OK) {
    if (!is_zero_big_decimal(&rest)) {
      fract_part_big(rest, big_2, result);
    }
  }
  return res;
}
int s21_big_div(s21_bigdecimal big_1, s21_bigdecimal big_2,
                s21_bigdecimal *result) {
  int res = OK;
  bool sign = big_get_sign(&big_1) ^ big_get_sign(&big_2);
  big_nullify_bit(&big_1, BIG_BITS - 1);
  big_nullify_bit(&big_2, BIG_BITS - 1);
  memset(result, 0, sizeof(s21_bigdecimal));
  if (is_zero_big_decimal(&big_2)) {
    res = S21_NAN;
  } else {
    if ((res = normalize(&big_1, &big_2)) == OK) {
      res = divide(big_1, big_2, result);
      if (sign) {
        big_set_bit(result, BIG_BITS - 1);
      }
    }
  }
  return res;
}
int s21_is_big_less_or_equal(s21_bigdecimal a, s21_bigdecimal b) {
  s21_bigdecimal t_less = create_big(0, 0, 0, 0, 0);
  s21_big_sub(a, b, &t_less);
  return big_get_sign(&t_less) || is_zero_big_decimal(&t_less);
}
void simple_big_div(s21_bigdecimal dec_1, s21_bigdecimal dec_2,
                    s21_bigdecimal *result) {
  s21_bigdecimal tmp = {{0, 0, 0, 0, 0, 0, 0, 0}};
  memset(result, 0, sizeof(s21_bigdecimal));
  for (int i = 255; i >= 0; i--) {
    big_l_shift(&tmp, 1);
    if (big_get_bit(&dec_1, i)) {
      big_set_bit(&tmp, 0);
    }
    if (s21_big_is_greater(tmp, dec_2)) {
      big_set_bit(result, i);
      s21_big_sub(tmp, dec_2, &tmp);
    }
  }
}
