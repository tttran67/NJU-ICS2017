#include "cpu/exec.h"

// data-mov.c
make_EHelper(mov);
make_EHelper(movzx);
make_EHelper(movsx);
make_EHelper(push);
make_EHelper(pop);
make_EHelper(leave);

// prefix.c
make_EHelper(operand_size);

// special.c
make_EHelper(inv);
make_EHelper(nemu_trap);

// control.c
make_EHelper(jmp);
make_EHelper(jmp_rm);
make_EHelper(call);
make_EHelper(call_rm);
make_EHelper(ret);

// arith.c
make_EHelper(add);
make_EHelper(adc);
make_EHelper(cmp);
make_EHelper(sub);
make_EHelper(sbb);
make_EHelper(mul);
make_EHelper(inc);
make_EHelper(dec);
make_EHelper(neg);

// logic.c
make_EHelper(test);
make_EHelper(and);
make_EHelper(or);
make_EHelper(xor);