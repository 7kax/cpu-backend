#pragma once
// #ifndef __SYSTEM_Y86_H__
// #define __SYSTEM_Y86_H__
#include <map>
#include <string>
#include <array>
#include "tools.h"
// CC
#define CC_ZF 0
#define CC_SF 1
#define CC_OF 2
// REG
#define REG_rax 0
#define REG_rcx 1
#define REG_rdx 2
#define REG_rbx 3
#define REG_rsp 4
#define REG_rbp 5
#define REG_rsi 6
#define REG_rdi 7
#define REG_r8 8
#define REG_r9 9
#define REG_r10 10
#define REG_r11 11
#define REG_r12 12
#define REG_r13 13
#define REG_r14 14
#define REG_no_reg 15
// STAT
#define STAT_AOK 1
#define STAT_HLT 2
#define STAT_ADR 3
#define STAT_INS 4
// #endif
typedef long long ll;
const std::string ins_loc = "0x";

ll PC;
std::array<ll, 16> RF;
std::array<bool, 3> CC;
int STAT;
std::vector<short> instructions;
std::map<ll, std::string> locate_ins;
std::map<ll, ll> DMEM;
bool Cnd;
int MEM_start;

void initializer();

void initializer()
{
    PC = 0;
    RF.fill(0);
    CC = {true, false, false};
    STAT = STAT_AOK;
    instructions.clear();
    DMEM.clear();
    std::string temp;
    while (std::getline(std::cin, temp))
    {
        if (temp.substr(0, 2) == ins_loc && isHex(temp[7]))
        {
            int loc = std::stoi(temp, 0, 16);
            loc *= 2;
            instructions.resize(loc);
            for (int i = 7; isHex(temp[i]); i++)
                instructions.push_back(char_to_hex(temp[i]));
            locate_ins[loc / 2] = temp.substr(31, temp.size() - 31);
        }
    }
    MEM_start = instructions.size() + 1;
    MEM_start /= 2;
    instructions.resize(instructions.size() + 16);
}
