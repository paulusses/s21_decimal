#include "s21_decimal.h"

int get_bit(s21_decimal *src, int position) {
  return !!(src->bits[position / 32] & (1U << (position % 32)));
}
void set_bit(s21_decimal *src, int position) {
  src->bits[position / 32] |= 1U << (position % 32);
}
void nullify_bit(s21_decimal *src, int position) {
  src->bits[position / 32] &= ~(1U << (position % 32));
}
int get_scale(s21_decimal *src) {
  return (0b11111111 << 16 & src->bits[3]) >> 16;
}
int get_sign(s21_decimal *src) { return get_bit(src, 127); }
void set_sign(s21_decimal *src) { set_bit(src, 127); }
void set_scale(s21_decimal *src, int scalevalue) {
  src->bits[3] = (get_bit(src, 127) << 31) | (scalevalue << 16);
}
void nullify_scale(s21_decimal *src) { src->bits[3] &= (~16711680); }
void l_shift(s21_decimal *src, int shift) {
  for (int i = 0; i < shift; i++) {
    int c1 = 0;
    int c2 = 0;
    if (get_bit(src, 31) == 1) {
      c1 = 1;
    }
    if (get_bit(src, 63) == 1) {
      c2 = 1;
    }
    src->bits[0] <<= 1;
    src->bits[1] <<= 1;
    if (c1 == 1) {
      set_bit(src, 32);
    }
    src->bits[2] <<= 1;
    if (c2 == 1) {
      set_bit(src, 64);
    }
  }
}
void print_decimal(s21_decimal src) {
  for (int i = 127; i >= 0; i--) {
    printf("%d", get_bit(&src, i));
    if (i == 96 || i == 64 || i == 32) {
      printf(" ");
    }
  }
}
void ftd(float src, char *str, int *scale) {
  sprintf(str, "%e", src);
  char *e = strchr(str, 'e');
  *scale += (*(e + 2) - '0') * 10;
  *scale += *(e + 3) - '0';
  if (*(e + 1) == '-') {
    *scale *= -1;
  }
  *e = '\0';
  int temp = 6;
  char *end = str + strlen(str) - 1;
  while (*end != '.' && *end == '0') {
    temp -= 1;
    end--;
  }
  *scale -= temp;
  if (*end == '.') {
    *end = '\0';
  } else {
    *(end + 1) = '\0';
    memmove(str + 1, str + 2, strlen(str + 2));
    *end = '\0';
  }
  while (*scale > 0) {
    end = str + strlen(str);
    *end = '0';
    *(end + 1) = '\0';
    (*scale)--;
  }
  if (*scale < 0) {
    *scale *= -1;
  }
}
void mul_by_10(s21_decimal a, s21_decimal *result) {
  memset(result, 0, sizeof(s21_decimal));
  int sign = get_sign(&a), scale = get_scale(&a);
  s21_decimal a2 = {{a.bits[0], a.bits[1], a.bits[2], 0}};
  l_shift(&a2, 1);
  s21_add(*result, a2, result);
  l_shift(&a2, 2);
  s21_add(*result, a2, result);
  if (sign == 1) {
    set_sign(result);
  }
  set_scale(result, scale);
}
void equal_degree(s21_decimal *a, s21_decimal *b) {
  int a_scale = get_scale(a), b_scale = get_scale(b);
  if (a_scale > b_scale) {
    a_scale -= b_scale;
    b_scale = 0;
  } else {
    b_scale -= a_scale;
    a_scale = 0;
  }
  int scale1 = get_scale(a);
  int scale2 = get_scale(b);
  while (a_scale != 0 || b_scale != 0) {
    if (a_scale != 0) {
      mul_by_10(*b, b);
      a_scale--;
    } else {
      mul_by_10(*a, a);
      b_scale--;
    }
  }
  if (scale1 != 0) {
    set_scale(b, scale1);
  } else {
    set_scale(a, scale2);
  }
}
int del_str_by_2(const char *str, char **new_str) {
  free(*new_str);
  *new_str = calloc(128, sizeof(char));
  char *current = *new_str;
  int ost = (*(str + strlen(str) - 1) - '0') % 2;
  int d = 0, opers = 0;
  while (*str) {
    opers++;
    d = d * 10 + (*str - '0');
    str++;
    while (d < 2 && *str) {
      if (opers != 1) {
        *current = '0';
        current++;
      }
      d *= 10;
      d += *str - '0';
      str++;
      opers++;
    }
    *current = (d / 2) + '0';
    current++;
    d -= (d / 2) * 2;
  }
  return ost;
}

void reverse_str(char **str) {
  char *new = calloc(strlen(*str) + 1, sizeof(char));
  for (size_t i = 0; i < strlen(*str); i++)
    new[i] = (*str)[strlen(*str) - i - 1];
  free(*str);
  *str = new;
}

void transferto2(char **str) {
  char *new = NULL;
  int res = del_str_by_2(*str, &new);
  char *digit = calloc(129, sizeof(char)), *c = digit;
  while ((*new != '1' && *new != '0') || strlen(*str) > 1) {
    *(c++) = res + '0';
    *str = strcpy(*str, new);
    res = del_str_by_2(*str, &new);
  }
  *(c++) = res + '0';
  *c = *new;
  reverse_str(&digit);
  free(*str);
  *str = digit;
  free(new);
}
void inverse_Bit(s21_decimal *d, int position) {
  d->bits[position / 32] ^= 1U << (position % 32);
}
int is_zero_decimal(s21_decimal *src) {
  int res = 1;
  for (size_t i = 0; i < 3; ++i) {
    if (src->bits[i] != 0) {
      res = 0;
    }
  }
  return res;
}
void simple_div(s21_decimal dec_1, s21_decimal dec_2, s21_decimal *result) {
  s21_decimal tmp = {{0, 0, 0, 0}};
  memset(result, 0, sizeof(s21_decimal));
  for (int i = 127; i >= 0; i--) {
    l_shift(&tmp, 1);
    if (get_bit(&dec_1, i)) {
      set_bit(&tmp, 0);
    }
    if (s21_is_greater_or_equal(tmp, dec_2)) {
      set_bit(result, i);
      s21_sub(tmp, dec_2, &tmp);
    }
  }
}