#pragma once

// the depth of the hashing
#define HASH3_DEPTH 64

// randomly generated constants.  The bottom half has to be FFFF or
// else the entire hash loses some strength
static const size_t HASH3_CONSTANTS[HASH3_DEPTH + 1] =
{
	0x6157FFFF, 0x5387FFFF, 0x8ECBFFFF, 0xB3DBFFFF, 0x1AFDFFFF, 0xD1FDFFFF, 0x19B3FFFF, 0xE6C7FFFF,
	0x53BDFFFF, 0xCDAFFFFF, 0xE543FFFF, 0x369DFFFF, 0x8135FFFF, 0x50E1FFFF, 0x115BFFFF, 0x07D1FFFF,
	0x9AA1FFFF, 0x4D87FFFF, 0x442BFFFF, 0xEAA5FFFF, 0xAEDBFFFF, 0xB6A5FFFF, 0xAFE9FFFF, 0xE895FFFF,
	0x4E05FFFF, 0xF8BFFFFF, 0xCB5DFFFF, 0x2F85FFFF, 0xF1DFFFFF, 0xD5ADFFFF, 0x438DFFFF, 0x6073FFFF,
	0xA99FFFFF, 0x2E0BFFFF, 0xF729FFFF, 0x5D01FFFF, 0x1ACDFFFF, 0xFAD1FFFF, 0xD86BFFFF, 0xE909FFFF,
	0xD3BDFFFF, 0xF35BFFFF, 0xD53DFFFF, 0x4DC1FFFF, 0x6897FFFF, 0x6E4DFFFF, 0x305BFFFF, 0x6F0DFFFF,
	0x33C9FFFF, 0xC955FFFF, 0xC1EDFFFF, 0x48D5FFFF, 0x0CF5FFFF, 0x356BFFFF, 0x5F65FFFF, 0x71C1FFFF,
	0x3F13FFFF, 0x489DFFFF, 0xEBA3FFFF, 0x340DFFFF, 0xF537FFFF, 0xD5E7FFFF, 0x6D27FFFF, 0x89D7FFFF,
	0xA93FFFFF,
};

// multiplication constants, this allows an abstract use
// of the string length
static const size_t HASH3_MULTS[HASH3_DEPTH + 1] =
{
	33,  34,  35,  36,  37,  38,  39,  40,
	41,  42,  43,  44,  45,  46,  47,  48,
	49,  50,  51,  52,  53,  54,  55,  56,
	57,  58,  59,  60,  61,  62,  63,  64,
	65,  66,  67,  68,  69,  70,  71,  72,
	73,  74,  75,  76,  77,  78,  79,  80,
	81,  82,  83,  84,  85,  86,  87,  88,
	89,  90,  91,  92,  93,  94,  95,  96,
	97,
};

#define HASH3_RECURSE_00(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[0] : HASH3_MULTS[0] * HASH3_RECURSE_01(string+1, *(string+1))), value)
#define HASH3_RECURSE_01(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[1] : HASH3_MULTS[1] * HASH3_RECURSE_02(string+1, *(string+1))), value)
#define HASH3_RECURSE_02(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[2] : HASH3_MULTS[2] * HASH3_RECURSE_03(string+1, *(string+1))), value)
#define HASH3_RECURSE_03(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[3] : HASH3_MULTS[3] * HASH3_RECURSE_04(string+1, *(string+1))), value)
#define HASH3_RECURSE_04(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[4] : HASH3_MULTS[4] * HASH3_RECURSE_05(string+1, *(string+1))), value)
#define HASH3_RECURSE_05(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[5] : HASH3_MULTS[5] * HASH3_RECURSE_06(string+1, *(string+1))), value)
#define HASH3_RECURSE_06(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[6] : HASH3_MULTS[6] * HASH3_RECURSE_07(string+1, *(string+1))), value)
#define HASH3_RECURSE_07(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[7] : HASH3_MULTS[7] * HASH3_RECURSE_08(string+1, *(string+1))), value)
#define HASH3_RECURSE_08(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[8] : HASH3_MULTS[8] * HASH3_RECURSE_09(string+1, *(string+1))), value)
#define HASH3_RECURSE_09(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[9] : HASH3_MULTS[9] * HASH3_RECURSE_10(string+1, *(string+1))), value)
#define HASH3_RECURSE_10(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[10] : HASH3_MULTS[10] * HASH3_RECURSE_11(string+1, *(string+1))), value)
#define HASH3_RECURSE_11(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[11] : HASH3_MULTS[11] * HASH3_RECURSE_12(string+1, *(string+1))), value)
#define HASH3_RECURSE_12(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[12] : HASH3_MULTS[12] * HASH3_RECURSE_13(string+1, *(string+1))), value)
#define HASH3_RECURSE_13(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[13] : HASH3_MULTS[13] * HASH3_RECURSE_14(string+1, *(string+1))), value)
#define HASH3_RECURSE_14(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[14] : HASH3_MULTS[14] * HASH3_RECURSE_15(string+1, *(string+1))), value)
#define HASH3_RECURSE_15(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[15] : HASH3_MULTS[15] * HASH3_RECURSE_16(string+1, *(string+1))), value)
#define HASH3_RECURSE_16(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[16] : HASH3_MULTS[16] * HASH3_RECURSE_17(string+1, *(string+1))), value)
#define HASH3_RECURSE_17(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[17] : HASH3_MULTS[17] * HASH3_RECURSE_18(string+1, *(string+1))), value)
#define HASH3_RECURSE_18(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[18] : HASH3_MULTS[18] * HASH3_RECURSE_19(string+1, *(string+1))), value)
#define HASH3_RECURSE_19(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[19] : HASH3_MULTS[19] * HASH3_RECURSE_20(string+1, *(string+1))), value)
#define HASH3_RECURSE_20(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[20] : HASH3_MULTS[20] * HASH3_RECURSE_21(string+1, *(string+1))), value)
#define HASH3_RECURSE_21(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[21] : HASH3_MULTS[21] * HASH3_RECURSE_22(string+1, *(string+1))), value)
#define HASH3_RECURSE_22(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[22] : HASH3_MULTS[22] * HASH3_RECURSE_23(string+1, *(string+1))), value)
#define HASH3_RECURSE_23(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[23] : HASH3_MULTS[23] * HASH3_RECURSE_24(string+1, *(string+1))), value)
#define HASH3_RECURSE_24(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[24] : HASH3_MULTS[24] * HASH3_RECURSE_25(string+1, *(string+1))), value)
#define HASH3_RECURSE_25(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[25] : HASH3_MULTS[25] * HASH3_RECURSE_26(string+1, *(string+1))), value)
#define HASH3_RECURSE_26(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[26] : HASH3_MULTS[26] * HASH3_RECURSE_27(string+1, *(string+1))), value)
#define HASH3_RECURSE_27(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[27] : HASH3_MULTS[27] * HASH3_RECURSE_28(string+1, *(string+1))), value)
#define HASH3_RECURSE_28(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[28] : HASH3_MULTS[28] * HASH3_RECURSE_29(string+1, *(string+1))), value)
#define HASH3_RECURSE_29(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[29] : HASH3_MULTS[29] * HASH3_RECURSE_30(string+1, *(string+1))), value)
#define HASH3_RECURSE_30(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[30] : HASH3_MULTS[30] * HASH3_RECURSE_31(string+1, *(string+1))), value)
#define HASH3_RECURSE_31(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[31] : HASH3_MULTS[31] * HASH3_RECURSE_32(string+1, *(string+1))), value)
#define HASH3_RECURSE_32(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[32] : HASH3_MULTS[32] * HASH3_RECURSE_33(string+1, *(string+1))), value)
#define HASH3_RECURSE_33(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[33] : HASH3_MULTS[33] * HASH3_RECURSE_34(string+1, *(string+1))), value)
#define HASH3_RECURSE_34(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[34] : HASH3_MULTS[34] * HASH3_RECURSE_35(string+1, *(string+1))), value)
#define HASH3_RECURSE_35(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[35] : HASH3_MULTS[35] * HASH3_RECURSE_36(string+1, *(string+1))), value)
#define HASH3_RECURSE_36(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[36] : HASH3_MULTS[36] * HASH3_RECURSE_37(string+1, *(string+1))), value)
#define HASH3_RECURSE_37(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[37] : HASH3_MULTS[37] * HASH3_RECURSE_38(string+1, *(string+1))), value)
#define HASH3_RECURSE_38(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[38] : HASH3_MULTS[38] * HASH3_RECURSE_39(string+1, *(string+1))), value)
#define HASH3_RECURSE_39(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[39] : HASH3_MULTS[39] * HASH3_RECURSE_40(string+1, *(string+1))), value)
#define HASH3_RECURSE_40(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[40] : HASH3_MULTS[40] * HASH3_RECURSE_41(string+1, *(string+1))), value)
#define HASH3_RECURSE_41(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[41] : HASH3_MULTS[41] * HASH3_RECURSE_42(string+1, *(string+1))), value)
#define HASH3_RECURSE_42(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[42] : HASH3_MULTS[42] * HASH3_RECURSE_43(string+1, *(string+1))), value)
#define HASH3_RECURSE_43(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[43] : HASH3_MULTS[43] * HASH3_RECURSE_44(string+1, *(string+1))), value)
#define HASH3_RECURSE_44(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[44] : HASH3_MULTS[44] * HASH3_RECURSE_45(string+1, *(string+1))), value)
#define HASH3_RECURSE_45(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[45] : HASH3_MULTS[45] * HASH3_RECURSE_46(string+1, *(string+1))), value)
#define HASH3_RECURSE_46(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[46] : HASH3_MULTS[46] * HASH3_RECURSE_47(string+1, *(string+1))), value)
#define HASH3_RECURSE_47(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[47] : HASH3_MULTS[47] * HASH3_RECURSE_48(string+1, *(string+1))), value)
#define HASH3_RECURSE_48(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[48] : HASH3_MULTS[48] * HASH3_RECURSE_49(string+1, *(string+1))), value)
#define HASH3_RECURSE_49(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[49] : HASH3_MULTS[49] * HASH3_RECURSE_50(string+1, *(string+1))), value)
#define HASH3_RECURSE_50(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[50] : HASH3_MULTS[50] * HASH3_RECURSE_51(string+1, *(string+1))), value)
#define HASH3_RECURSE_51(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[51] : HASH3_MULTS[51] * HASH3_RECURSE_52(string+1, *(string+1))), value)
#define HASH3_RECURSE_52(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[52] : HASH3_MULTS[52] * HASH3_RECURSE_53(string+1, *(string+1))), value)
#define HASH3_RECURSE_53(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[53] : HASH3_MULTS[53] * HASH3_RECURSE_54(string+1, *(string+1))), value)
#define HASH3_RECURSE_54(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[54] : HASH3_MULTS[54] * HASH3_RECURSE_55(string+1, *(string+1))), value)
#define HASH3_RECURSE_55(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[55] : HASH3_MULTS[55] * HASH3_RECURSE_56(string+1, *(string+1))), value)
#define HASH3_RECURSE_56(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[56] : HASH3_MULTS[56] * HASH3_RECURSE_57(string+1, *(string+1))), value)
#define HASH3_RECURSE_57(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[57] : HASH3_MULTS[57] * HASH3_RECURSE_58(string+1, *(string+1))), value)
#define HASH3_RECURSE_58(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[58] : HASH3_MULTS[58] * HASH3_RECURSE_59(string+1, *(string+1))), value)
#define HASH3_RECURSE_59(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[59] : HASH3_MULTS[59] * HASH3_RECURSE_60(string+1, *(string+1))), value)
#define HASH3_RECURSE_60(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[60] : HASH3_MULTS[60] * HASH3_RECURSE_61(string+1, *(string+1))), value)
#define HASH3_RECURSE_61(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[61] : HASH3_MULTS[61] * HASH3_RECURSE_62(string+1, *(string+1))), value)
#define HASH3_RECURSE_62(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[62] : HASH3_MULTS[62] * HASH3_RECURSE_63(string+1, *(string+1))), value)
#define HASH3_RECURSE_63(string, value) HASH3_FUNCTION((*(string+1) == 0 ? HASH3_CONSTANTS[63] : HASH3_MULTS[63] * HASH3_RECURSE_64(string+1, *(string+1))), value)
#define HASH3_RECURSE_64(string, value) HASH3_CONSTANTS[64]

// The following is the function used for hashing
// Do NOT use NEXTHASH more than once, it will cause
// N-Squared expansion and make compilation very slow
// If not impossible
#define HASH3_FUNCTION(next, value) ((value << 15) | value | 33) + ((11 * value * value * 257) ^ HASH3_CONSTANTS[value & 0x3f] ^ (next))

// finally the macro used to generate the hash
#define HASH3(string) HASH3_RECURSE_00(string, *string)
