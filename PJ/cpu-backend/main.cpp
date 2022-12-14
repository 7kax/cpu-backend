#include "./headers/system_y86.h"
#include "./headers/run.h"
#include "./headers/json.hpp"
using json = nlohmann::json;
const json src = {
    {"PC", 0},
    {"REG", {{"rax", 0}, {"rcx", 0}, {"rdx", 0}, {"rbx", 0}, {"rsp", 0}, {"rbp", 0}, {"rsi", 0}, {"rdi", 0}, {"r8", 0}, {"r9", 0}, {"r10", 0}, {"r11", 0}, {"r12", 0}, {"r13", 0}, {"r14", 0}}},
    {"CC", {{"ZF", 0}, {"SF", 0}, {"OF", 0}}},
    {"STAT", 1},
    {"MEM", {}}};

void add_json();

json dest;
json output;

int main()
{
    std::cout << "WARNING:Make sure you put files in correct catalog!" << std::endl;
    std::string read_file_seed = "./test_file/";
    std::string write_file_seed = "./output/";
    std::string read_file, write_file;
    std::cout << "Enter test file name:" << std::endl;
    std::cin >> read_file;
    std::cout << "Enter output file name:" << std::endl;
    std::cin >> write_file;
    freopen((read_file_seed + read_file).c_str(), "r", stdin);
    freopen((write_file_seed + write_file).c_str(), "w", stdout);
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
    freopen("CON", "w", stdout);
    freopen("CON", "r", stdin);
    std::cout << "Work is done." << std::endl;
    return 0;
}

void add_json()
{
    // init
    dest = src;
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
    // set STAT
    dest["STAT"] = STAT;
    // set CC
    dest["CC"]["ZF"] = (int)CC[CC_ZF];
    dest["CC"]["SF"] = (int)CC[CC_SF];
    dest["CC"]["OF"] = (int)CC[CC_OF];
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
    // add to info
    output.push_back(dest);
}