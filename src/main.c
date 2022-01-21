#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include "include/terminal.h"

static mpz_t stirling_res;

void stirling_recursion(uint32_t n, uint32_t k, mpz_t res);
void stirling_iterative(uint32_t n, uint32_t k, mpz_t res);
void cleanup(void);

int main(int argc, char const *argv[])
{
	uint32_t n = 0;
	uint32_t k = 0;
	atexit(cleanup);
	mpz_init_set_si(stirling_res, 0);
	terminal_clear();
	printf("n: ");
	scanf("%" SCNu32, &n);
	terminal_clear();
	printf("k: ");
	scanf("%" SCNu32, &k);
	printf("n=%" PRIu32 ", k=%" PRIu32 "\n", n, k);
	stirling_iterative(n, k, stirling_res);
	gmp_printf("Stirling(n=%" PRIu32 ", k=%" PRIu32 ")=%Zd\n", n, k, stirling_res, stirling_res);
	mpz_clear(stirling_res);
	while (terminal_kbhit() != 1)
	{
		asm("nop\n");
	}
}

void stirling_iterative(uint32_t n, uint32_t k, mpz_t res)
{
	mpz_t loop_res_div;
	mpz_t loop_res;
	mpz_t tmp_loop_res;
	mpz_t bin;
	mpz_t neg;
	mpz_t tmp_neg;
	mpz_t po;
	mpz_t tmp_po;
	mpz_init(loop_res_div);
	mpz_init(loop_res);
	mpz_init(tmp_loop_res);
	mpz_fac_ui(loop_res_div, k);
	mpz_init(bin);
	mpz_init(neg);
	mpz_init(tmp_neg);
	mpz_init(po);
	mpz_init(tmp_po);
	for (unsigned long i = 0; i <= k; i++)
	{
		mpz_set_si(loop_res, 0);
		mpz_set_si(tmp_loop_res, 0);
		mpz_set_si(bin, 0);
		mpz_set_si(neg, 0);
		mpz_set_si(po, 0);
		mpz_set_si(tmp_po, 0);
		mpz_set_si(tmp_neg, -1);
		mpz_pow_ui(neg, tmp_neg, k - i);
		mpz_bin_uiui(bin, k, i);
		mpz_set_si(tmp_po, i);
		mpz_pow_ui(po, tmp_po, n);
		mpz_mul(tmp_loop_res, neg, bin);
		mpz_mul(loop_res, tmp_loop_res, po);
	}
	if (mpz_divisible_p(loop_res, loop_res_div))
	{
		mpz_divexact(res, loop_res, loop_res_div);
	}
	else
	{
		mpz_fdiv_q(res, loop_res, loop_res_div);
	}
	mpz_clear(loop_res_div);
	mpz_clear(loop_res);
	mpz_clear(tmp_loop_res);
	mpz_clear(bin);
	mpz_clear(neg);
	mpz_clear(po);
	mpz_clear(tmp_po);
	mpz_clear(tmp_neg);
}

void stirling_recursion(uint32_t n, uint32_t k, mpz_t res)
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
		mpz_fdiv_q_ui(res, res, 3);

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
		mpz_fdiv_q_ui(res, res, 24);
		return;
	}
	if ((n - 1) == k)
	{
		mpz_bin_uiui(res, n, 2);
		return;
	}
	stirling_recursion(n - 1, k - 1, res);
	mpz_t tmp;
	mpz_init_set(tmp, res);
	stirling_recursion(n - 1, k, tmp);
	mpz_mul_ui(tmp, tmp, k);
	mpz_add(res, res, tmp);
	mpz_clear(tmp);
}

void cleanup(void)
{
	mpz_clear(stirling_res);
}
