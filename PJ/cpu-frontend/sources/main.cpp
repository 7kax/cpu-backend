#include "../headers/system_y86.h"
#include "../headers/run.h"
#include "../headers/json.hpp"
#include "../headers/fifo_map.hpp"
using namespace nlohmann;

// A workaround to give to use fifo_map as map, we are just ignoring the 'less' compare
template <class K, class V, class dummy_compare, class A>
using my_workaround_fifo_map = fifo_map<K, V, fifo_map_compare<K>, A>;
using my_json = basic_json<my_workaround_fifo_map>;

void add_json();

my_json output;

int main()
{
    // init
    initializer();
    output.clear();
    // start processing
    while (true)
    {
        fetch();
        if (STAT != STAT_AOK)
            break;
        decode();
        execute();
        if (STAT != STAT_AOK)
            break;
        memory();
        if (STAT != STAT_AOK)
            break;
        wirte_back();
        PC_update();
        add_json();
    }
    add_json();
    // print info
    std::cout << std::setw(4) << output << std::endl;
    return 0;
}

void add_json()
{
    // init
    my_json dest;
    // set PC
    dest["PC"] = PC;
    // set REG
    dest["REG"]["rax"] = RF[REG_rax];
    dest["REG"]["rcx"] = RF[REG_rcx];
    dest["REG"]["rdx"] = RF[REG_rdx];
    dest["REG"]["rbx"] = RF[REG_rbx];
    dest["REG"]["rsp"] = RF[REG_rsp];
    dest["REG"]["rbp"] = RF[REG_rbp];
    dest["REG"]["rsi"] = RF[REG_rsi];
    dest["REG"]["rdi"] = RF[REG_rdi];
    dest["REG"]["r8"] = RF[REG_r8];
    dest["REG"]["r9"] = RF[REG_r9];
    dest["REG"]["r10"] = RF[REG_r10];
    dest["REG"]["r11"] = RF[REG_r11];
    dest["REG"]["r12"] = RF[REG_r12];
    dest["REG"]["r13"] = RF[REG_r13];
    dest["REG"]["r14"] = RF[REG_r14];
    // set MEM
    ll ptr;
    for (ptr = 0; ptr < MEM_start; ptr += 8)
    {
        ll temp = ins_read_long(instructions, ptr * 2);
        if (temp != 0)
            dest["MEM"][std::to_string(ptr)] = temp;
    }
    for (auto &cell : DMEM)
        if (cell.second != 0)
            dest["MEM"][std::to_string(cell.first)] = cell.second;
    // set CC
    dest["CC"]["ZF"] = (int)CC[CC_ZF];
    dest["CC"]["SF"] = (int)CC[CC_SF];
    dest["CC"]["OF"] = (int)CC[CC_OF];
    // set STAT
    dest["STAT"] = STAT;
    // add to info
    output.push_back(dest);
}