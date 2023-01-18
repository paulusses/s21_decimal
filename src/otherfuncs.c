#include "s21_decimal.h"

int s21_truncate(s21_decimal src, s21_decimal *result) {
  int res = OK;
  memset(result, 0, sizeof(s21_decimal));
  s21_decimal tenDec;
  memset(&tenDec, 0, sizeof(s21_decimal));
  tenDec.bits[0] = 10;
  int srcScale = get_scale(&src);
  int srcSign = get_sign(&src);
  set_scale(&src, 0);
  nullify_bit(&src, 127);
  while (srcScale > 0) {
    simple_div(src, tenDec, &src);
    srcScale--;
  }
  if (srcSign) {
    set_sign(&src);
  }
  *result = src;
  return res;
}

int s21_floor(s21_decimal value, s21_decimal *result) {
  memset(result, 0, sizeof(s21_decimal));
  int res = OK;
  int scale = get_scale(&value);
  s21_decimal one = {{1, 0, 0, 0}};
  if (get_scale(&value) > 28) {
    res = S21_calc_error;
  } else {
    if (value.bits[0] == 0 && value.bits[1] == 0 && value.bits[2] == 0) {
      s21_truncate(value, result);
    } else if (get_sign(&value) == 1 && scale > 0) {
      s21_truncate(value, result);
      s21_sub(*result, one, result);
    } else {
      s21_truncate(value, result);
    }
  }
  return res;
}

// int s21_round(s21_decimal value, s21_decimal *result) {
//   memset(result, 0, sizeof(s21_decimal));
//   int res = OK;
//   int sign = get_sign(&value);
//   int scale = get_scale(&value);
//   s21_decimal tmp = {{0}};
//   s21_decimal one = {{1, 0, 0, 0}};
//   s21_decimal ten = {{10, 0, 0, 0}};
//   s21_decimal five = {{5, 0, 0, 0}};
//   if (get_scale(&value) > 28) {
//     res = S21_calc_error;
//   } else if (scale == 0) {
//     *result = value;
//   } else {
//     tmp = value;
//     nullify_bit(&tmp, 127);
//     if (scale) {
//       mul_by_10(tmp, &tmp);
//       s21_truncate(tmp, &tmp);
//     }
//     while (s21_is_greater_or_equal(tmp, ten)) {
//       s21_mod(tmp, ten, &tmp);
//     }
//     if (sign == 0 && s21_is_greater_or_equal(tmp, five)) {
//       s21_truncate(value, result);
//       s21_add(*result, one, result);
//     } else if (sign == 1 && s21_is_greater_or_equal(tmp, five)) {
//       s21_truncate(value, result);
//       s21_sub(*result, one, result);
//     } else if (s21_is_less(tmp, five)) {
//       s21_truncate(value, result);
//     }
//   }
//   return res;
// }
int s21_round(s21_decimal value, s21_decimal *result) {
  int res = OK;
  if (result != NULL) {
    memset(result, 0, sizeof(s21_decimal));
    int scale = get_scale(&value);
    int sign = get_sign(&value);
    if (sign) {
      nullify_bit(&value, 127);
    }
    s21_truncate(value, result);
    s21_decimal fract_part;
    memset(&fract_part, 0, sizeof(s21_decimal));
    s21_sub(value, *result, &fract_part);
    nullify_scale(&fract_part);
    s21_decimal ten = {{10, 0, 0, 0}};
    while ((scale - 1 > 0) && !is_zero_decimal(&fract_part)) {
      simple_div(fract_part, ten, &fract_part);
      scale--;
    }
    s21_decimal five = {{5, 0, 0, 0}};
    int comp_res = s21_is_greater_or_equal(fract_part, five);
    if (comp_res) {
      s21_decimal one = {{1, 0, 0, 0}};
      s21_add(*result, one, result);
    }
    if (sign) {
      set_sign(result);
    }
  } else {
    res = S21_calc_error;
  }
  return res;
}
int s21_negate(s21_decimal value, s21_decimal *result) {
  int res = OK;
  memset(result, 0, sizeof(s21_decimal));
  *result = value;
  inverse_Bit(result, 127);
  return res;
}
