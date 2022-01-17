#include <stdio.h>
#include <gmp.h>
#include <stdint.h>
#include <inttypes.h>
#include "include/terminal.h"

void sterling(uint32_t n, uint32_t k, mpz_t res);
int main(int argc, char const *argv[])
{
	uint32_t n = 0;
	uint32_t k = 0;
	mpz_t sterling_res;
	mpz_init_set_si(sterling_res, 0);
	terminal_clear();
	printf("n: ");
	scanf("%" SCNu32, &n);
	terminal_clear();
	printf("k: ");
	scanf("%" SCNu32, &k);
	sterling(n, k, sterling_res);
	gmp_printf("Sterling(n=%" PRIu32 ", k=%" PRIu32 ")=%Zd\n", n, k, sterling_res);
	mpz_clear(sterling_res);
}

void sterling(uint32_t n, uint32_t k, mpz_t res)
{
	if (n == k)
	{
		mpz_set_si(res, 1);
		return;
	}
	if (n > 0 && k == 0)
	{
		mpz_set_si(res, 0);
		return;
	}
	if (k > n)
	{
		mpz_set_si(res, 0);
		return;
	}
	if (k == 2)
	{
		mpz_set_si(res, 2);
		mpz_pow_ui(res, res, n - 1);
		mpz_sub_ui(res, res, 1);
		return;
	}
	if (k == 3)
	{
		mpz_set_si(res, 3);
		mpz_pow_ui(res, res, n - 1);
		mpz_t tmp;
		mpz_init_set_ui(tmp, 2);
		mpz_pow_ui(tmp, tmp, n);
		mpz_sub(res, res, tmp);
		mpz_clear(tmp);
		mpz_add_ui(res, res, 1);
		mpz_div_ui(res, res, 3);

		return;
	}
	if ((n - 3) == k)
	{
		mpz_t tmp;
		mpz_init(tmp);
		mpz_bin_uiui(res, n, 4);
		mpz_bin_uiui(tmp, n - 2, 2);
		mpz_mul(res, res, tmp);
		mpz_clear(tmp);
		return;
	}
	if ((n - 2) == k)
	{ //(n(n-1)(n-2)(3n-5))/24
		mpz_set_ui(res, n);
		mpz_mul_ui(res, res, 3);
		mpz_sub_ui(res, res, 5);
		mpz_sub_ui(res, res, n - 2);
		mpz_sub_ui(res, res, n - 1);
		mpz_sub_ui(res, res, n);
		mpz_div_ui(res, res, 24);
		return;
	}
	if ((n - 1) == k)
	{
		mpz_bin_uiui(res, n, 2);
		return;
	}
	sterling(n - 1, k - 1, res);
	mpz_t tmp;
	mpz_init_set(tmp, res);
	sterling(n - 1, k, tmp);
	mpz_mul_ui(tmp, tmp, k);
	mpz_add(res, res, tmp);
	mpz_clear(tmp);
}