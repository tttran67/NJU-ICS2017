#include "cpu/exec.h"

make_EHelper(mov);

make_EHelper(operand_size);

make_EHelper(inv);
make_EHelper(nemu_trap);

// control.c
make_EHelper(jmp);
make_EHelper(call);
make_EHelper(ret);

// data-mov.c
make_EHelper(push);
make_EHelper(pop);

// arith.c
make_EHelper(add);
make_EHelper(adc);
make_EHelper(cmp);
make_EHelper(sub);
make_EHelper(sbb);
make_EHelper(mul);
make_EHelper(sub);

// logic.c
make_EHelper(test);
make_EHelper(and);
make_EHelper(or);
make_EHelper(xor);


