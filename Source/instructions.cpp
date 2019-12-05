/* Assembly Instruction Implementation */

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>

// Project Headers
#include "instructions.hpp"
#include "mcu.hpp"
#include "sys.hpp"

#define bit(value, i) (((0x01 << i) & value) >> i)

using namespace std;

namespace {

    int extract(int opcode, int from, int to, int offs) {

        int res = 0;

        for(int i = from; i < to; i++) {

            int bit = (((0x01 << i) & opcode) >> i);
            res += (bit << (i - from) + offs);
        }

        return res;
    }
};

void nop(Sys *sys, int opcode) {

    return;
}

void movw(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 8, 0) * 2;
    int src = extract(opcode, 0, 4, 0) * 2;

    sys->write_gpr(dest, sys->read_gpr(src));
    sys->write_gpr(dest + 1, sys->read_gpr(src + 1));
}

void muls(Sys *sys, int opcode) {

    /* in progress */
}

void mulsu(Sys *sys, int opcode) {

    /* in progress */
}

void fmul(Sys *sys, int opcode) {

    /* in progress */
}

void ldi(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 8, 0);
    int src = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    sys->write_gpr(dest + 16, src);
}

void rjmp(Sys *sys, int opcode) {

    int offs = extract(opcode, 0, 12, 0);
    int prog_counter = sys->get_pc();

    if(((0x01 << 11) & offs) != 0x00) {

        offs ^= ((0x01 << 12) - 1);
        offs += 0x01;

        sys->set_pc(prog_counter - offs + 1);

        return;
    }

    sys->set_pc(prog_counter + offs + 1);
} 

void mov(Sys *sys, int opcode) {

    int	dest = extract(opcode, 4, 9, 0);
    int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    sys->write_gpr(dest, sys->read_gpr(src));
}

void dec(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);
    int8_t value = sys->read_gpr(dest);

    int8_t result = value - 0x01;

    int8_t vf_res = ~bit(result, 7) * bit(result, 6) * bit(result, 5) * bit(result, 4);
    vf_res *= bit(result, 3) * bit(result, 2) * bit(result, 1) * bit(result, 0);

    int8_t nf_res = bit(result, 7);

    sys->write_sreg(VF, vf_res);
    sys->write_sreg(NF, nf_res);
    sys->write_sreg(SF, vf_res ^ nf_res);
    sys->write_sreg(ZF, (result == 0x00));

    sys->write_gpr(dest, result);
}

void push(Sys *sys, int opcode) {

    int src = extract(opcode, 4, 9, 0);
    int8_t val = sys->read_gpr(src);

    sys->push_stack(val);
}

void pop(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);
    int8_t val = sys->pop_stack();

    sys->write_gpr(dest, val);
}

void out(Sys *sys, int opcode) {

    int dest = extract(opcode, 0, 4, 0) + extract(opcode, 9, 11, 4);
    int src = extract(opcode, 4, 9, 0);

    int8_t val = sys->read_gpr(src);

    sys->write_data(dest, val);
}

void clr(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);
    int src = extract(opcode, 0, 4, 0) + extract(opcode, 9, 10, 4);

    int8_t val_dest = sys->read_gpr(dest);
    int8_t val_src = sys->read_gpr(src);

    sys->write_gpr(dest, (val_dest ^ val_src));

    sys->write_sreg(SF, 0x00);
    sys->write_sreg(VF, 0x00);
    sys->write_sreg(NF, 0x00);
    sys->write_sreg(ZF, 0x01);
}

void ld_x(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);

    int8_t xl = sys->read_gpr(XL);
    int8_t xh = sys->read_gpr(XH);

    int8_t data = sys->read_data((xh << 8) + xl);
    sys->write_gpr(dest, data);
}

void ld_y(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);

    int8_t yl = sys->read_gpr(YL);
    int8_t yh = sys->read_gpr(YH);

    int8_t data = sys->read_data((yh << 8) + yl);
    sys->write_gpr(dest, data);
}

void ld_z(Sys *sys, int opcode) {

    int dest = extract(opcode, 4, 9, 0);

    int8_t zl = sys->read_gpr(ZL);
    int8_t zh = sys->read_gpr(ZH);

    int8_t data = sys->read_data((zh << 8) + zl);
    sys->write_gpr(dest, data);
}

void brne(Sys *sys, int opcode) {

    if(sys->read_sreg(ZF) == 0x01)
        return;

    int offs = extract(opcode, 3, 10, 0);
    int prog_counter = sys->get_pc();

    if(((0x01 << 6) & offs) != 0x00) {

        offs ^= ((0x01 << 7) - 1);
        offs += 0x01;

        sys->set_pc(prog_counter - offs + 1);

        return;
    }

    sys->set_pc(prog_counter + offs + 1);
}

void rcall(Sys *sys, int opcode) {

    int offs = extract(opcode, 0, 12, 0);
    int pc = sys->get_pc();

    switch(PC_BIT) {

        case 16:

            sys->push_stack((pc + 1) & 0x00ff);
            sys->push_stack((pc + 1) & 0xff00);

        break;

        case 22: /* currently not supported */ break;

        default: return;
    }

    if(((0x01 << 11) & offs) != 0x00) {

        offs ^= ((0x01 << 12) - 1);
        offs += 0x01;

        sys->set_pc(pc - offs + 1);

        return;
    }

    sys->set_pc(pc + offs + 1);
}

void ret(Sys *sys, int opcode) {

    int8_t pcl, pch, pcm;

    switch(PC_BIT) {

        case 16:
            
            pch = sys->pop_stack();
            pcl = sys->pop_stack();

            sys->set_pc((pch << 8) + pcl);

        break;

        case 22: /* currently not supported */ break;

        default: return;
    }
}

void cpi(Sys *sys, int opcode) {

    int reg = extract(opcode, 4, 8, 0);
    int8_t comp = extract(opcode, 0, 4, 0) + extract(opcode, 8, 12, 4);

    int8_t value = sys->read_gpr(16 + reg);
    int8_t result = value - comp;

    int8_t cf_res = (~(bit(value, 7)) * bit(comp, 7)) + (bit(comp, 7) * bit(result, 7));
    cf_res += (bit(result, 7) * ~bit(value, 7));

    int8_t vf_res = (bit(value, 7) * ~bit(comp, 7) * ~bit(result, 7));
    vf_res += (~bit(value, 7) * bit(comp, 7) * bit(result, 7));

    int8_t hf_res = (~bit(value, 3) * bit(comp, 3)) + (bit(comp, 3) * bit(result, 3));
    hf_res += (bit(result, 3) * ~bit(value, 3));

    int8_t nf_res = bit(result, 7);

    sys->write_sreg(CF, cf_res);
    sys->write_sreg(VF, vf_res);
    sys->write_sreg(HF, hf_res);
    sys->write_sreg(NF, nf_res);
    sys->write_sreg(SF, nf_res ^ vf_res);
    sys->write_sreg(ZF, (result == 0x00));
}

void ses(Sys *sys, int opcode) {

    sys->write_sreg(SF, 0x01);
}

void set(Sys *sys, int opcode) {

    sys->write_sreg(TF, 0x01);
}

void sev(Sys *sys, int opcode) {

    sys->write_sreg(VF, 0x01);
}

void sez(Sys *sys, int opcode) {

    sys->write_sreg(ZF, 0x01);
}

void seh(Sys *sys, int opcode) {

    sys->write_sreg(HF, 0x01);
}

void sec(Sys *sys, int opcode) {

    sys->write_sreg(CF, 0x01);
}

void sei(Sys *sys, int opcode) {

    sys->write_sreg(IF, 0x01);
}

void sen(Sys *sys, int opcode) {

    sys->write_sreg(NF, 0x01);
}

void bclr(Sys *sys, int opcode) {

    int s_bit = extract(opcode, 4, 7, 0);

    sys->write_sreg(s_bit, 0x00);
}

void (*instructions[INSTR_MAX]) (Sys *sys, int opcode) = { nop, movw, muls, mulsu, fmul, ldi, rjmp, mov, 
                                                           dec, push, pop, out, clr, ld_x, ld_y, ld_z, brne,
                                                           rcall, ret, cpi, ses, set, sev, sez, seh, sec, sei, 
                                                           sen, bclr };




