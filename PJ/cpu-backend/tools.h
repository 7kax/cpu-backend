#pragma once
// #ifndef __TOOLS_H__
// #define __TOOLS_H__
#include <iostream>
#include <vector>
#include <cstdlib>
// #endif
typedef long long ll;

short char_to_hex(char in);
bool isHex(char c);
ll ins_read_long(std::vector<short> &ins, ll dest);
void long_to_ins(std::vector<short> &ins, ll src, ll dest);

short char_to_hex(char in)
{
    if (in >= '0' && in <= '9')
        return in - '0';
    return in - 'a' + 10;
}
bool isHex(char c)
{
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
}
ll ins_read_long(std::vector<short> &ins, ll dest)
{
    ll ret = 0;
    ret += (((ll)(ins[dest])) << 4) + (ll)(ins[dest + 1]);
    ret += ((((ll)(ins[dest + 2])) << 4) + (ll)(ins[dest + 3])) << 8;
    ret += ((((ll)(ins[dest + 4])) << 4) + (ll)(ins[dest + 5])) << 16;
    ret += ((((ll)(ins[dest + 6])) << 4) + (ll)(ins[dest + 7])) << 24;
    ret += ((((ll)(ins[dest + 8])) << 4) + (ll)(ins[dest + 9])) << 32;
    ret += ((((ll)(ins[dest + 10])) << 4) + (ll)(ins[dest + 11])) << 40;
    ret += ((((ll)(ins[dest + 12])) << 4) + (ll)(ins[dest + 13])) << 48;
    ret += ((((ll)(ins[dest + 14])) << 4) + (ll)(ins[dest + 15])) << 56;
    return ret;
}
void long_to_ins(std::vector<short> &ins, ll src, ll dest)
{
    char str[16];
    sprintf(str, "%llx", src);
    ins[dest] = char_to_hex(str[14]), ins[dest + 1] = char_to_hex(str[15]);
    ins[dest + 2] = char_to_hex(str[12]), ins[dest + 3] = char_to_hex(str[13]);
    ins[dest + 4] = char_to_hex(str[10]), ins[dest + 5] = char_to_hex(str[11]);
    ins[dest + 6] = char_to_hex(str[8]), ins[dest + 7] = char_to_hex(str[9]);
    ins[dest + 8] = char_to_hex(str[6]), ins[dest + 9] = char_to_hex(str[7]);
    ins[dest + 10] = char_to_hex(str[4]), ins[dest + 11] = char_to_hex(str[5]);
    ins[dest + 12] = char_to_hex(str[2]), ins[dest + 13] = char_to_hex(str[3]);
    ins[dest + 14] = char_to_hex(str[0]), ins[dest + 15] = char_to_hex(str[1]);
}
