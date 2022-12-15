#pragma once
#include "system_y86.h"
#include "tools.h"
typedef long long ll;

short icode, ifun;
short rA, rB;
ll valA, valB, valP, valC, valE, valM;

void fetch();
void decode();
void execute();
void memory();
void wirte_back();
void PC_update();

void set_CC();
void set_Cnd();
ll operate();

void fetch()
{
    icode = instructions[PC * 2];
    ifun = instructions[PC * 2 + 1];
    switch (icode)
    {
    case 0:
    case 1:
    case 9:
        valP = PC + 1;
        break;
    case 2:
    case 6:
    case 10:
    case 11:
        rA = instructions[PC * 2 + 2];
        rB = instructions[PC * 2 + 3];
        valP = PC + 2;
        break;
    case 3:
    case 4:
    case 5:
        rA = instructions[PC * 2 + 2];
        rB = instructions[PC * 2 + 3];
        valC = ins_read_long(instructions, PC * 2 + 4);
        valP = PC + 10;
        break;
    case 7:
    case 8:
        valC = ins_read_long(instructions, PC * 2 + 2);
        valP = PC + 9;
        break;
    default:
        STAT = STAT_INS;
        break;
    }
}
void decode()
{
    switch (icode)
    {
    case 0:
    case 1:
    case 3:
    case 7:
        break;
    case 2:
    case 4:
    case 5:
    case 6:
        valA = RF[rA];
        valB = RF[rB];
        break;
    case 8:
    case 9:
    case 11:
        valA = RF[REG_rsp];
        valB = RF[REG_rsp];
        break;
    case 10:
        valA = RF[rA];
        valB = RF[REG_rsp];
        break;
    default:
        STAT = STAT_INS;
        break;
    }
}
void execute()
{
    switch (icode)
    {
    case 0:
        STAT = STAT_HLT;
        break;
    case 1:
        break;
    case 2:
    case 7:
        valE = valA;
        set_Cnd();
        break;
    case 3:
        valE = valC;
        break;
    case 4:
    case 5:
        valE = valB + valC;
        break;
    case 6:
        valE = operate();
        set_CC();
        break;
    case 8:
    case 10:
        valE = valB - 8;
        break;
    case 9:
    case 11:
        valE = valB + 8;
        break;
    default:
        STAT = STAT_INS;
        break;
    }
}
void memory()
{
    switch (icode)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 6:
    case 7:
        break;
    case 4:
        if (valE >= MEM_start)
            DMEM[valE] = valA;
        else
            long_to_ins(instructions, valA, valE * 2);
        break;
    case 10:
        if (valE >= MEM_start)
            DMEM[valE] = valA;
        else
            STAT = STAT_ADR;
        break;
    case 5:
        if (valE >= MEM_start)
            valM = DMEM[valE];
        else
            valM = ins_read_long(instructions, valE * 2);
        break;
    case 8:
        if (valE >= MEM_start)
            DMEM[valE] = valP;
        else
            STAT = STAT_ADR;
        break;
    case 9:
    case 11:
        if (valA >= MEM_start)
            valM = DMEM[valA];
        else
            valM = ins_read_long(instructions, valA * 2);
        break;
    default:
        STAT = STAT_INS;
        break;
    }
}
void wirte_back()
{
    switch (icode)
    {
    case 0:
    case 1:
    case 4:
    case 7:
        break;
    case 2:
        RF[rB] = Cnd ? valE : valB;
        break;
    case 3:
    case 6:
        RF[rB] = valE;
        break;
    case 5:
        RF[rA] = valM;
        break;
    case 8:
    case 9:
    case 10:
        RF[REG_rsp] = valE;
        break;
    case 11:
        RF[REG_rsp] = valE;
        RF[rA] = valM;
        break;
    default:
        STAT = STAT_INS;
        break;
    }
}
void PC_update()
{
    switch (icode)
    {
    case 0:
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 10:
    case 11:
        PC = valP;
        break;
    case 7:
        PC = Cnd ? valC : valP;
        break;
    case 8:
        PC = valC;
        break;
    case 9:
        PC = valM;
        break;
    default:
        STAT = STAT_INS;
        break;
    }
}

void set_CC()
{
    CC[CC_ZF] = (valE == 0);
    CC[CC_SF] = (valE < 0);
    switch (ifun)
    {
    case 0:
        CC[CC_OF] = ((valA < 0) == (valB < 0)) && ((valE < 0) != (valA < 0));
        break;
    case 1:
        CC[CC_OF] = ((valA < 0) != (valB < 0)) && ((valE < 0) == (valA < 0));
        break;
    case 2:
    case 3:
        CC[CC_OF] = false;
        break;
    default:
        STAT = STAT_INS;
        break;
    }
}
void set_Cnd()
{
    switch (ifun)
    {
    case 0:
        Cnd = true;
        break;
    case 1:
        Cnd = (CC[CC_SF] ^ CC[CC_OF]) | CC[CC_ZF];
        break;
    case 2:
        Cnd = CC[CC_SF] ^ CC[CC_OF];
        break;
    case 3:
        Cnd = CC[CC_ZF];
        break;
    case 4:
        Cnd = !CC[CC_ZF];
        break;
    case 5:
        Cnd = !(CC[CC_SF] ^ CC[CC_OF]);
        break;
    case 6:
        Cnd = !((CC[CC_SF] ^ CC[CC_OF]) | CC[CC_ZF]);
        break;
    default:
        STAT = STAT_INS;
        break;
    }
}
ll operate()
{
    switch (ifun)
    {
    case 0:
        return valA + valB;
    case 1:
        return valB - valA;
    case 2:
        return valA & valB;
    case 3:
        return valA ^ valB;
    default:
        STAT = STAT_INS;
        break;
    }
    return 0;
}
