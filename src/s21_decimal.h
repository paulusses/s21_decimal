#ifndef SRC_S21_DECIMAL_H_
#define SRC_S21_DECIMAL_H_

#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int bits[4];
} s21_decimal;

typedef struct {
  int bits[8];
} s21_bigdecimal;

#define S21_calc_error 1

#define S21_INF 1
#define S21_NEG_INF 2
#define S21_NAN 3

#define BIG_BITS (64 * sizeof(int))
#define BIG_MANTISSA_BITS (56 * sizeof(int))
#define DECIMAL_BITS (32 * sizeof(int))
enum { OK, CONVERTATION_FAULT };

// converters
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);

// arithmetics
int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mul(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_div(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);
int s21_mod(s21_decimal value_1, s21_decimal value_2, s21_decimal *result);

// comparsion
int s21_is_less(s21_decimal a, s21_decimal b);
int s21_is_less_or_equal(s21_decimal a, s21_decimal b);
int s21_is_greater(s21_decimal a, s21_decimal b);
int s21_is_greater_or_equal(s21_decimal a, s21_decimal b);
int s21_is_equal(s21_decimal a, s21_decimal b);
int s21_is_not_equal(s21_decimal a, s21_decimal b);

// other functions
int s21_floor(s21_decimal value, s21_decimal *result);
int s21_round(s21_decimal value, s21_decimal *result);
int s21_truncate(s21_decimal value, s21_decimal *result);
int s21_negate(s21_decimal value, s21_decimal *result);

// extra functions
void print_decimal(s21_decimal src);
void set_bit(s21_decimal *src, int position);
void set_sign(s21_decimal *src);
int get_bit(s21_decimal *src, int position);
int get_scale(s21_decimal *src);
int get_sign(s21_decimal *src);
void s21_decimal_default(s21_decimal *d);
void nullify_bit(s21_decimal *src, int position);
void set_scale(s21_decimal *src, int scalevalue);
void ftd(float src, char *str, int *scale);
int del_str_by_2(const char *str, char **new_str);
void mul_by_10(s21_decimal a, s21_decimal *result);
void l_shift(s21_decimal *src, int shift);
void equal_degree(s21_decimal *a, s21_decimal *b);
void reverse_str(char **str);
void transferto2(char **str);
void inverse_Bit(s21_decimal *d, int position);
void s21_Normalize(s21_decimal *src, s21_decimal *result);
int is_zero_decimal(s21_decimal *src);
void simple_div(s21_decimal dec_1, s21_decimal dec_2, s21_decimal *result);
void nullify_scale(s21_decimal *src);

// big decimal additional functions
void big_nullify_bit(s21_bigdecimal *src, int position);
void big_set_bit(s21_bigdecimal *src, int position);
void big_inverse_Bit(s21_bigdecimal *d, int position);
int big_add(s21_bigdecimal value_1, s21_bigdecimal value_2,
            s21_bigdecimal *result);
void convertToBig(s21_decimal *src, s21_bigdecimal *dst);
int big_get_scale(s21_bigdecimal *src);
void big_set_scale(s21_bigdecimal *src, int scalevalue);
int big_get_bit(s21_bigdecimal *src, int position);
int s21_big_add(s21_bigdecimal value_1, s21_bigdecimal value_2,
                s21_bigdecimal *result);
void big_equal_degree(s21_bigdecimal *a, s21_bigdecimal *b);
int big_sub(s21_bigdecimal value_1, s21_bigdecimal value_2,
            s21_bigdecimal *result);
int s21_big_is_greater(s21_bigdecimal a, s21_bigdecimal b);
void big_mul_by_10(s21_bigdecimal a, s21_bigdecimal *result);
void big_l_shift(s21_bigdecimal *src, int shift);
int s21_big_sub(s21_bigdecimal value_1, s21_bigdecimal value_2,
                s21_bigdecimal *result);
void convertToDec(s21_decimal *src, s21_bigdecimal *dst);
int big_get_sign(s21_bigdecimal *src);
void convertToDec(s21_decimal *src, s21_bigdecimal *dst);
void big_set_sign(s21_bigdecimal *src);
int s21_big_mul(s21_bigdecimal value_1, s21_bigdecimal value_2,
                s21_bigdecimal *result);
int s21_big_is_equal(s21_bigdecimal a, s21_bigdecimal b);
void big_nullify_scale(s21_bigdecimal *src);
int greater_for_mul(s21_bigdecimal a, s21_bigdecimal b);
void print_big_decimal(s21_bigdecimal src);
int s21_big_mod(s21_bigdecimal value_1, s21_bigdecimal value_2,
                s21_bigdecimal *result);
int s21_is_big_less_or_equal(s21_bigdecimal a, s21_bigdecimal b);
int s21_big_div(s21_bigdecimal big_1, s21_bigdecimal big_2,
                s21_bigdecimal *result);
int divide(s21_bigdecimal big_1, s21_bigdecimal big_2, s21_bigdecimal *result);
int normalize(s21_bigdecimal *a, s21_bigdecimal *b);
s21_bigdecimal create_big(const int l_b, const int m1_b, const int m2_b,
                          const int scale, const bool sign);
void fract_part_big(s21_bigdecimal div, s21_bigdecimal den,
                    s21_bigdecimal *result);
int is_zero_big_decimal(s21_bigdecimal *src);
int int_part_big(s21_bigdecimal big_1, s21_bigdecimal big_2,
                 s21_bigdecimal *result, s21_bigdecimal *rest);
bool is_empty_big_part(s21_bigdecimal *src);
void s21_NormalizeBig(s21_bigdecimal src, s21_bigdecimal *result);
void simple_big_div(s21_bigdecimal dec_1, s21_bigdecimal dec_2,
                    s21_bigdecimal *result);

#endif  // SRC_S21_DECIMAL_H_
