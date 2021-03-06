/********************************************************************************/
/*                                                                              */
/* CZ80 opcode include source file                                              */
/* C Z80 emulator version 0.91                                                  */
/* Copyright 2004-2005 Stephane Dallongeville                                   */
/*                                                                              */
/********************************************************************************/

#ifndef IS_IN_CZ80
#error Do not compile this file by itself - compile cz80.c instead
#endif

// 2015/04/28: Instruction timings updated from:
// - http://wikiti.brandonw.net/?title=Z80_Instruction_Set
// - http://www.z80.info/z80undoc.txt
// - http://www.z80.info/z80undoc3.txt

#if CZ80_USE_JUMPTABLE
    DO_JMPTBL_OP(Opcode);
#else
switch (Opcode)
{
#endif

    // 8 BITS LOAD

    OP(0x00):   // NOP

    OP(0x40):   // LD   B,B
    OP(0x49):   // LD   C,C
    OP(0x52):   // LD   D,D
    OP(0x5b):   // LD   E,E
    OP(0x64):   // LD   H,H
    OP(0x6d):   // LD   L,L
    OP(0x7f):   // LD   A,A
        RET(4);

#define OPJ_LD_R_R(JmpType, Opcode) \
    JmpType(Opcode): \
        zR8((Opcode >> 3) & 7) = zR8(Opcode & 7); \
	RET(4);
#define OP_LD_R_R(Opcode) OPJ_LD_R_R(OP, Opcode)

    OP_LD_R_R(0x41)   // LD   B,C
    OP_LD_R_R(0x42)   // LD   B,D
    OP_LD_R_R(0x43)   // LD   B,E
    OP_LD_R_R(0x44)   // LD   B,H
    OP_LD_R_R(0x45)   // LD   B,L
    OP_LD_R_R(0x47)   // LD   B,A

    OP_LD_R_R(0x48)   // LD   C,B
    OP_LD_R_R(0x4a)   // LD   C,D
    OP_LD_R_R(0x4b)   // LD   C,E
    OP_LD_R_R(0x4c)   // LD   C,H
    OP_LD_R_R(0x4d)   // LD   C,L
    OP_LD_R_R(0x4f)   // LD   C,A

    OP_LD_R_R(0x50)   // LD   D,B
    OP_LD_R_R(0x51)   // LD   D,C
    OP_LD_R_R(0x53)   // LD   D,E
    OP_LD_R_R(0x54)   // LD   D,H
    OP_LD_R_R(0x55)   // LD   D,L
    OP_LD_R_R(0x57)   // LD   D,A

    OP_LD_R_R(0x58)   // LD   E,B
    OP_LD_R_R(0x59)   // LD   E,C
    OP_LD_R_R(0x5a)   // LD   E,D
    OP_LD_R_R(0x5c)   // LD   E,H
    OP_LD_R_R(0x5d)   // LD   E,L
    OP_LD_R_R(0x5f)   // LD   E,A

    OP_LD_R_R(0x60)   // LD   H,B
    OP_LD_R_R(0x61)   // LD   H,C
    OP_LD_R_R(0x62)   // LD   H,D
    OP_LD_R_R(0x63)   // LD   H,E
    OP_LD_R_R(0x65)   // LD   H,L
    OP_LD_R_R(0x67)   // LD   H,A

    OP_LD_R_R(0x68)   // LD   L,B
    OP_LD_R_R(0x69)   // LD   L,C
    OP_LD_R_R(0x6a)   // LD   L,D
    OP_LD_R_R(0x6b)   // LD   L,E
    OP_LD_R_R(0x6c)   // LD   L,H
    OP_LD_R_R(0x6f)   // LD   L,A

    OP_LD_R_R(0x78)   // LD   A,B
    OP_LD_R_R(0x79)   // LD   A,C
    OP_LD_R_R(0x7a)   // LD   A,D
    OP_LD_R_R(0x7b)   // LD   A,E
    OP_LD_R_R(0x7c)   // LD   A,H
    OP_LD_R_R(0x7d)   // LD   A,L

#define OPJ_LD_R_n(JmpType, Opcode) \
    JmpType(Opcode): \
        zR8((Opcode >> 3) & 7) = FETCH_BYTE(); \
	RET(7);
#define OP_LD_R_n(Opcode) OPJ_LD_R_n(OP, Opcode)

    OP_LD_R_n(0x06)   // LD   B,#imm
    OP_LD_R_n(0x0e)   // LD   C,#imm
    OP_LD_R_n(0x16)   // LD   D,#imm
    OP_LD_R_n(0x1e)   // LD   E,#imm
    OP_LD_R_n(0x26)   // LD   H,#imm
    OP_LD_R_n(0x2e)   // LD   L,#imm
    OP_LD_R_n(0x3e)   // LD   A,#imm

#define OPJ_LD_R_mHL(JmpType, Opcode) \
    JmpType(Opcode): \
        PRE_IO(); \
        EXACT_ONLY(zWZ = zHL + 1); \
        READ_BYTE(zHL, zR8((Opcode >> 3) & 7)); \
        POST_IO(); \
        RET(7);
#define OP_LD_R_mHL(Opcode) OPJ_LD_R_mHL(OP, Opcode)

    OP_LD_R_mHL(0x46)   // LD   B,(HL)
    OP_LD_R_mHL(0x4e)   // LD   C,(HL)
    OP_LD_R_mHL(0x56)   // LD   D,(HL)
    OP_LD_R_mHL(0x5e)   // LD   E,(HL)
    OP_LD_R_mHL(0x66)   // LD   H,(HL)
    OP_LD_R_mHL(0x6e)   // LD   L,(HL)
    OP_LD_R_mHL(0x7e)   // LD   A,(HL)

#define OPJ_LD_mHL_R(JmpType, Opcode) \
    JmpType(Opcode): \
        PRE_IO(); \
        EXACT_ONLY(zWZ = (zA << 8) | ((zlHL + 1) & 0xFF)); \
	WRITE_BYTE(zHL, zR8(Opcode & 7)); \
        POST_IO(); \
        RET(7);
#define OP_LD_mHL_R(Opcode) OPJ_LD_mHL_R(OP, Opcode)

    OP_LD_mHL_R(0x70)   // LD   (HL),B
    OP_LD_mHL_R(0x71)   // LD   (HL),C
    OP_LD_mHL_R(0x72)   // LD   (HL),D
    OP_LD_mHL_R(0x73)   // LD   (HL),E
    OP_LD_mHL_R(0x74)   // LD   (HL),H
    OP_LD_mHL_R(0x75)   // LD   (HL),L
    OP_LD_mHL_R(0x77)   // LD   (HL),A

    OP(0x36):   // LD (HL), #imm
        PRE_IO();
        WRITE_BYTE(zHL, FETCH_BYTE());
        POST_IO();
        RET(10);

#define OPJ_LD_A_mRR(JmpType, Opcode, RR) \
    JmpType(Opcode): \
        PRE_IO(); \
	EXACT_ONLY(zWZ = z##RR + 1); \
	READ_BYTE(z##RR, zA); \
        POST_IO(); \
	RET(7);
#define OP_LD_A_mRR(Opcode, RR) OPJ_LD_A_mRR(OP, Opcode, RR)

    OP_LD_A_mRR(0x0a, BC)   // LD   A,(BC)
    OP_LD_A_mRR(0x1a, DE)   // LD   A,(DE)

    OP(0x3a): {  // LD   A,(nn)
        uint16_t adr;
        PRE_IO();
        FETCH_WORD(adr);
        READ_BYTE(adr, zA);
        POST_IO();
        RET(13);
    }

#define OPJ_LD_mRR_A(JmpType, Opcode, RR) \
    JmpType(Opcode): \
        PRE_IO(); \
        EXACT_ONLY(zWZ = (zA << 8) | ((z##RR + 1) & 0xFF)); \
	WRITE_BYTE(z##RR, zA); \
        POST_IO(); \
	RET(7);
#define OP_LD_mRR_A(Opcode, RR) OPJ_LD_mRR_A(OP, Opcode, RR)

    OP_LD_mRR_A(0x02, BC)   // LD   (BC),A
    OP_LD_mRR_A(0x12, DE)   // LD   (DE),A

    OP(0x32): {  // LD   (nn),A
        uint16_t adr;
        PRE_IO();
        FETCH_WORD(adr);
        WRITE_BYTE(adr, zA);
        POST_IO();
        RET(13);
    }

    // 16 BITS LOAD

#define OPJ_LD_RR_nn(JmpType, Opcode) \
    JmpType(Opcode): \
        FETCH_WORD(zR16(Opcode >> 4)); \
        RET(10);
#define OP_LD_RR_nn(Opcode) OPJ_LD_RR_nn(OP, Opcode)

    OP_LD_RR_nn(0x01)   // LD   BC,nn
    OP_LD_RR_nn(0x11)   // LD   DE,nn
    OP_LD_RR_nn(0x21)   // LD   HL,nn

    OP(0x31):   // LD   SP,nn
        FETCH_WORD(zSP);
        RET(10);

    OP(0xf9):   // LD   SP,HL
        zSP = data->W;
        RET(6);

#define OPJ_LD_RR_mNN(JmpType, Opcode, RR) \
    JmpType(Opcode): \
        PRE_IO(); \
        FETCH_WORD(zWZ); \
        READ_WORD_LE(zWZ, z##RR); \
	EXACT_ONLY(zWZ++); \
        POST_IO(); \
        RET(16);
#define OP_LD_RR_mNN(Opcode, RR) OPJ_LD_RR_mNN(OP, Opcode, RR)

    OP_LD_RR_mNN(0x2a, HL)   // LD   HL,(nn)

#define OPJ_LD_mNN_RR(JmpType, Opcode, RR) \
    JmpType(Opcode): \
        PRE_IO(); \
        FETCH_WORD(zWZ); \
        WRITE_WORD_LE(zWZ, z##RR); \
	EXACT_ONLY(zWZ++); \
        POST_IO(); \
        RET(16);
#define OP_LD_mNN_RR(Opcode, RR) OPJ_LD_mNN_RR(OP, Opcode, RR)

    OP_LD_mNN_RR(0x22, HL)   // LD   (nn),HL

    // PUSH / POP

    // AF is stored reversed on the stack.
#define OPJ_POP_AF(JmpType, Opcode) \
    JmpType(Opcode): { \
        uint16_t res; \
        PRE_IO(); \
        POP_16(res); \
        zA = res >> 8; \
        zF = res & 0xFF; \
        POST_IO(); \
        RET(10); \
    }
#define OP_POP_AF(Opcode) OPJ_POP_AF(OP, Opcode)

    OP_POP_AF(0xf1)   // POP  AF

#define OPJ_POP_RR(JmpType, Opcode, RR) \
    JmpType(Opcode): \
        PRE_IO(); \
        POP_16(z##RR); \
        POST_IO(); \
        RET(10);
#define OP_POP_RR(Opcode, RR) OPJ_POP_RR(OP, Opcode, RR)

    OP_POP_RR(0xc1, BC)   // POP  BC
    OP_POP_RR(0xd1, DE)   // POP  DE
    OP_POP_RR(0xe1, HL)   // POP  HL

    // AF is stored reversed on the stack.
#define OPJ_PUSH_AF(JmpType, Opcode) \
    JmpType(Opcode): \
        PRE_IO(); \
        PUSH_16((zA << 8) | zF); \
        POST_IO(); \
        RET(11);
#define OP_PUSH_AF(Opcode) OPJ_PUSH_AF(OP, Opcode)

    OP_PUSH_AF(0xf5)   // PUSH AF

#define OPJ_PUSH_RR(JmpType, Opcode, RR) \
    JmpType(Opcode): \
        PRE_IO(); \
        PUSH_16(z##RR); \
        POST_IO(); \
        RET(11);
#define OP_PUSH_RR(Opcode, RR) OPJ_PUSH_RR(OP, Opcode, RR)

    OP_PUSH_RR(0xc5, BC)   // PUSH BC
    OP_PUSH_RR(0xd5, DE)   // PUSH DE
    OP_PUSH_RR(0xe5, HL)   // PUSH HL

    // EXCHANGE & BLOCK TRANSFERT / SEARCH

    {
        uint16_t tmp;

    OP(0x08):   // EX   AF,AF'
OP_EX_AF_AF2:
        tmp = zFA;
        zFA = zFA2;
        zFA2 = tmp;
        RET(4);

    OP(0xeb):   // EX   DE,HL
OP_EX_DE_HL:
        tmp = zDE;
        zDE = zHL;
        zHL = tmp;
        RET(4);

    OP(0xd9):   // EXX
OP_EXX:
        tmp = zBC;
        zBC = zBC2;
        zBC2 = tmp;
        tmp = zDE;
        zDE = zDE2;
        zDE2 = tmp;
        tmp = zHL;
        zHL = zHL2;
        zHL2 = tmp;
        RET(4);
    }

    OP(0xe3):   // EX   HL,(SP)
    {
        uint16_t adr;
        uint16_t tmp;
        
OP_EX_xx_mSP:
        PRE_IO();
        adr = zSP;
        tmp = data->W;
        READ_WORD_LE(adr, data->W);
        WRITE_WORD_LE(adr, tmp);
        EXACT_ONLY(zWZ = data->W);
        POST_IO();
        RET(19);
    }

    // 8 BITS ARITHMETIC

    // INC

#define OPJ_INC_R(JmpType, Opcode) \
    JmpType(Opcode): \
        zR8(Opcode >> 3)++; \
        zF = (zF & CZ80_CF) | SZXYHV_inc[zR8(Opcode >> 3)]; \
        RET(4);
#define OP_INC_R(Opcode) OPJ_INC_R(OP, Opcode)

    OP_INC_R(0x04)   // INC  B
    OP_INC_R(0x0c)   // INC  C
    OP_INC_R(0x14)   // INC  D
    OP_INC_R(0x1c)   // INC  E
    OP_INC_R(0x24)   // INC  H
    OP_INC_R(0x2c)   // INC  L
    OP_INC_R(0x3c)   // INC  A

// TODO: Should 'cycles' include the base 4 cycles, or not?
// Currently, it does not; if cycles = 0, no cycles will be used.
#define OP_INC_mRR_INT(adr, cycles) do { \
        uint8_t res; \
        PRE_IO(); \
        READ_BYTE(adr, res); \
        res++; \
        WRITE_BYTE(adr, res); \
        zF = (zF & CZ80_CF) | SZXYHV_inc[res]; \
        POST_IO(); \
        RET(cycles); \
    } while (0)

    OP(0x34): {  // INC  (HL)
        const uint16_t adr = zHL;
	OP_INC_mRR_INT(adr, 11);
    }

    // DEC

#define OPJ_DEC_R(JmpType, Opcode) \
    JmpType(Opcode): \
        zR8(Opcode >> 3)--; \
        zF = (zF & CZ80_CF) | SZXYHV_dec[zR8(Opcode >> 3)]; \
        RET(4);
#define OP_DEC_R(Opcode) OPJ_DEC_R(OP, Opcode)

    OP_DEC_R(0x05)   // DEC  B
    OP_DEC_R(0x0d)   // DEC  C
    OP_DEC_R(0x15)   // DEC  D
    OP_DEC_R(0x1d)   // DEC  E
    OP_DEC_R(0x25)   // DEC  H
    OP_DEC_R(0x2d)   // DEC  L
    OP_DEC_R(0x3d)   // DEC  A

// TODO: Should 'cycles' include the base 4 cycles, or not?
// Currently, it does not; if cycles = 0, no cycles will be used.
#define OP_DEC_mRR_INT(adr, cycles) do { \
        uint8_t res; \
        PRE_IO(); \
        READ_BYTE(adr, res); \
        res--; \
        WRITE_BYTE(adr, res); \
        zF = (zF & CZ80_CF) | SZXYHV_dec[res]; \
        POST_IO(); \
        RET(cycles); \
    } while (0)

    OP(0x35): {  // DEC  (HL)
        const uint16_t adr = zHL;
	OP_DEC_mRR_INT(adr, 11);
    }

    // ADD

// TODO: Should 'cycles' include the base 4 cycles, or not?
// Currently, it does not; if cycles = 0, no cycles will be used.
#define OP_ADD_INT(val, cycles) do { \
        /* bench : maybe use src instead of zA in zF calculation */ \
        uint16_t res = zA + val; \
        zF = SZXY[res & 0xFF] |                                 /* S/Z/X/Y flag */ \
            ((zA ^ res ^ val) & CZ80_HF) |                      /* H flag */ \
            (((val ^ zA ^ 0x80) & (val ^ res) & 0x80) >> 5) |   /* V flag */ \
            ((res >> 8) & CZ80_CF);                             /* C flag */ \
        zA = (uint8_t)res; \
        RET(cycles); \
    } while (0)

#define OPJ_ADD_A_n(JmpType, Opcode) \
    JmpType(Opcode): { \
        uint8_t val = FETCH_BYTE(); \
        OP_ADD_INT(val, 7); \
    }
#define OP_ADD_A_n(Opcode) OPJ_ADD_A_n(OP, Opcode)

    OP_ADD_A_n(0xc6)   // ADD  A,n

#define OPJ_ADD_A_mRR(JmpType, Opcode, RR) \
    JmpType(Opcode): { \
        uint8_t val; \
        PRE_IO(); \
        READ_BYTE(z##RR, val); \
        POST_IO(); \
        OP_ADD_INT(val, 7); \
    }
#define OP_ADD_A_mRR(Opcode, RR) OPJ_ADD_A_mRR(OP, Opcode, RR)

    OP_ADD_A_mRR(0x86, HL)   // ADD  A,(HL)

#define OPJ_ADD_A_R(JmpType, Opcode) \
    JmpType(Opcode): { \
        uint8_t val = zR8(Opcode & 7); \
        OP_ADD_INT(val, 4); \
    }
#define OP_ADD_A_R(Opcode) OPJ_ADD_A_R(OP, Opcode)

    OP_ADD_A_R(0x80)   // ADD  A,B
    OP_ADD_A_R(0x81)   // ADD  A,C
    OP_ADD_A_R(0x82)   // ADD  A,D
    OP_ADD_A_R(0x83)   // ADD  A,E
    OP_ADD_A_R(0x84)   // ADD  A,H
    OP_ADD_A_R(0x85)   // ADD  A,L
    OP_ADD_A_R(0x87)   // ADD  A,A

    // ADC

#define OP_ADC_INT(val, cycles) do { \
        /* bench : maybe use src instead of zA in zF calculation */ \
        uint16_t res = (zA + val) + (zF & CZ80_CF); \
        zF = SZXY[res & 0xFF] |                                 /* S/Z/X/Y flag */ \
            ((zA ^ res ^ val) & CZ80_HF) |                      /* H flag */ \
            (((val ^ zA ^ 0x80) & (val ^ res) & 0x80) >> 5) |   /* V flag */ \
            ((res >> 8) & CZ80_CF);                             /* C flag */ \
        zA = (uint8_t)res; \
        RET(cycles); \
    } while (0)

#define OPJ_ADC_A_n(JmpType, Opcode) \
    JmpType(Opcode): { \
        uint8_t val = FETCH_BYTE(); \
        OP_ADC_INT(val, 7); \
    }
#define OP_ADC_A_n(Opcode) OPJ_ADC_A_n(OP, Opcode)

    OP_ADC_A_n(0xce)   // ADC  A,n

#define OPJ_ADC_A_mRR(JmpType, Opcode, RR) \
    JmpType(Opcode): { \
        uint8_t val; \
        PRE_IO(); \
        READ_BYTE(z##RR, val); \
        POST_IO(); \
        OP_ADC_INT(val, 7); \
    }
#define OP_ADC_A_mRR(Opcode, RR) OPJ_ADC_A_mRR(OP, Opcode, RR)

    OP_ADC_A_mRR(0x8e, HL)   // ADC  A,(HL)

#define OPJ_ADC_A_R(JmpType, Opcode) \
    JmpType(Opcode): { \
        uint8_t val = zR8(Opcode & 7); \
        OP_ADC_INT(val, 4); \
    }
#define OP_ADC_A_R(Opcode) OPJ_ADC_A_R(OP, Opcode)

    OP_ADC_A_R(0x88)   // ADC  A,B
    OP_ADC_A_R(0x89)   // ADC  A,C
    OP_ADC_A_R(0x8a)   // ADC  A,D
    OP_ADC_A_R(0x8b)   // ADC  A,E
    OP_ADC_A_R(0x8c)   // ADC  A,H
    OP_ADC_A_R(0x8d)   // ADC  A,L
    OP_ADC_A_R(0x8f)   // ADC  A,A

    // SUB

#define OP_SUB_INT(val, cycles) do { \
        /* bench : maybe use src instead of zA in zF calculation */ \
        uint16_t res = zA - val; \
        zF = SZXY[res & 0xFF] |                             /* S/Z/X/Y flag */ \
            ((zA ^ res ^ val) & CZ80_HF) |                  /* H flag */ \
            (((val ^ zA) & (zA ^ res) & 0x80) >> 5) |       /* V flag */ \
            ((res >> 8) & CZ80_CF) | CZ80_NF;               /* C/N flag */ \
        zA = (uint8_t)res; \
        RET(cycles); \
    } while (0)

#define OPJ_SUB_A_n(JmpType, Opcode) \
    JmpType(Opcode): { \
        uint8_t val = FETCH_BYTE(); \
        OP_SUB_INT(val, 7); \
    }
#define OP_SUB_A_n(Opcode) OPJ_SUB_A_n(OP, Opcode)

    OP_SUB_A_n(0xd6)   // SUB  A,n

#define OPJ_SUB_A_mRR(JmpType, Opcode, RR) \
    JmpType(Opcode): { \
        uint8_t val; \
        PRE_IO(); \
        READ_BYTE(z##RR, val); \
        POST_IO(); \
        OP_SUB_INT(val, 7); \
    }
#define OP_SUB_A_mRR(Opcode, RR) OPJ_SUB_A_mRR(OP, Opcode, RR)

    OP_SUB_A_mRR(0x96, HL)   // SUB  A,(HL)

#define OPJ_SUB_A_R(JmpType, Opcode) \
    JmpType(Opcode): { \
        uint8_t val = zR8(Opcode & 7); \
        OP_SUB_INT(val, 4); \
    }
#define OP_SUB_A_R(Opcode) OPJ_SUB_A_R(OP, Opcode)

    OP_SUB_A_R(0x90)   // SUB  A,B
    OP_SUB_A_R(0x91)   // SUB  A,C
    OP_SUB_A_R(0x92)   // SUB  A,D
    OP_SUB_A_R(0x93)   // SUB  A,E
    OP_SUB_A_R(0x94)   // SUB  A,H
    OP_SUB_A_R(0x95)   // SUB  A,L
    OP_SUB_A_R(0x97)   // SUB  A,A

    // SBC

#define OP_SBC_INT(val, cycles) do { \
        /* bench : maybe use src instead of zA in zF calculation */ \
        uint16_t res = zA - (val + (zF & CZ80_CF)); \
        zF = SZXY[res & 0xFF] |                             /* S/Z/X/Y flag */ \
            ((zA ^ res ^ val) & CZ80_HF) |                  /* H flag */ \
            (((val ^ zA) & (zA ^ res) & 0x80) >> 5) |       /* V flag */ \
            ((res >> 8) & CZ80_CF) | CZ80_NF;               /* C/N flag */ \
        zA = (uint8_t)res; \
        RET(cycles); \
    } while (0)

#define OPJ_SBC_A_n(JmpType, Opcode) \
    JmpType(Opcode): { \
        uint8_t val = FETCH_BYTE(); \
        OP_SBC_INT(val, 7); \
    }
#define OP_SBC_A_n(Opcode) OPJ_SBC_A_n(OP, Opcode)

    OP_SBC_A_n(0xde)   // SBC  A,n

#define OPJ_SBC_A_mRR(JmpType, Opcode, RR) \
    JmpType(Opcode): { \
        uint8_t val; \
        PRE_IO(); \
        READ_BYTE(z##RR, val); \
        POST_IO(); \
        OP_SBC_INT(val, 7); \
    }
#define OP_SBC_A_mRR(Opcode, RR) OPJ_SBC_A_mRR(OP, Opcode, RR)

    OP_SBC_A_mRR(0x9e, HL)   // SBC  A,(HL)

#define OPJ_SBC_A_R(JmpType, Opcode) \
    JmpType(Opcode): { \
        uint8_t val = zR8(Opcode & 7); \
        OP_SBC_INT(val, 4); \
    }
#define OP_SBC_A_R(Opcode) OPJ_SBC_A_R(OP, Opcode)

    OP_SBC_A_R(0x98)   // SBC  A,B
    OP_SBC_A_R(0x99)   // SBC  A,C
    OP_SBC_A_R(0x9a)   // SBC  A,D
    OP_SBC_A_R(0x9b)   // SBC  A,E
    OP_SBC_A_R(0x9c)   // SBC  A,H
    OP_SBC_A_R(0x9d)   // SBC  A,L
    OP_SBC_A_R(0x9f)   // SBC  A,A

    // CP

#if CZ80_DEBUG
#define OP_CP_INT(val, cycles) do { \
        /* bench : maybe use src instead of zA in zF calculation */ \
        uint16_t res = zA - val; \
        zF = SZXY[res & 0xFF] |                             /* S/Z/X/Y flag */ \
            ((zA ^ res ^ val) & CZ80_HF) |                  /* H flag */ \
            (((val ^ zA) & (zA ^ res) & 0x80) >> 5) |       /* V flag */ \
            ((res >> 8) & CZ80_CF) | CZ80_NF;               /* C/N flag */ \
        RET(cycles); \
    } while (0)
#else /* CZ80_DEBUG */
#define OP_CP_INT(val, cycles) do { \
        /* bench : maybe use src instead of zA in zF calculation */ \
        uint16_t res = zA - val; \
        zF = (SZXY[res & 0xFF] & ~(CZ80_XF | CZ80_YF)) |    /* S/Z flag */ \
            (val & (CZ80_XF | CZ80_YF)) |                   /* X/Y flag */ \
            ((zA ^ res ^ val) & CZ80_HF) |                  /* H flag */ \
            (((val ^ zA) & (zA ^ res) & 0x80) >> 5) |       /* V flag */ \
            ((res >> 8) & CZ80_CF) | CZ80_NF;               /* C/N flag */ \
        RET(cycles); \
    } while (0)
#endif /* CZ80_DEBUG */

#define OPJ_CP_A_n(JmpType, Opcode) \
    JmpType(Opcode): { \
        uint8_t val = FETCH_BYTE(); \
        OP_CP_INT(val, 7); \
    }
#define OP_CP_A_n(Opcode) OPJ_CP_A_n(OP, Opcode)

    OP_CP_A_n(0xfe)   // CP  A,n

#define OPJ_CP_A_mRR(JmpType, Opcode, RR) \
    JmpType(Opcode): { \
        uint8_t val; \
        PRE_IO(); \
        READ_BYTE(z##RR, val); \
        POST_IO(); \
        OP_CP_INT(val, 7); \
    }
#define OP_CP_A_mRR(Opcode, RR) OPJ_CP_A_mRR(OP, Opcode, RR)

    OP_CP_A_mRR(0xbe, HL)   // CP  A,(HL)

#define OPJ_CP_A_R(JmpType, Opcode) \
    JmpType(Opcode): { \
        uint8_t val = zR8(Opcode & 7); \
        OP_CP_INT(val, 4); \
    }
#define OP_CP_A_R(Opcode) OPJ_CP_A_R(OP, Opcode)

    OP_CP_A_R(0xb8)   // CP   A,B
    OP_CP_A_R(0xb9)   // CP   A,C
    OP_CP_A_R(0xba)   // CP   A,D
    OP_CP_A_R(0xbb)   // CP   A,E
    OP_CP_A_R(0xbc)   // CP   A,H
    OP_CP_A_R(0xbd)   // CP   A,L
    OP_CP_A_R(0xbf)   // CP   A,A

    // AND

#define OP_AND_INT(val, cycles) do { \
        zA &= val; \
        zF = SZXYP[zA] | CZ80_HF; \
        RET(cycles); \
    } while (0)

#define OPJ_AND_A_mHL(JmpType, Opcode) \
    JmpType(Opcode): { \
        uint8_t val; \
        PRE_IO(); \
        READ_BYTE(zHL, val); \
        POST_IO(); \
        OP_AND_INT(val, 7); \
    }
#define OP_AND_A_mHL(Opcode) OPJ_AND_A_mHL(OP, Opcode)

    OP_AND_A_mHL(0xa6)  // AND  A,(HL)

#define OPJ_AND_A_n(JmpType, Opcode) \
    JmpType(Opcode): { \
        uint8_t val = FETCH_BYTE(); \
        OP_AND_INT(val, 7); \
    }
#define OP_AND_A_n(Opcode) OPJ_AND_A_n(OP, Opcode)

    OP_AND_A_n(0xe6)   // AND  A,n

#define OPJ_AND_A_R(JmpType, Opcode) \
    JmpType(Opcode): { \
        uint8_t val = zR8(Opcode & 7); \
        OP_AND_INT(val, 4); \
    }
#define OP_AND_A_R(Opcode) OPJ_AND_A_R(OP, Opcode)

    OP_AND_A_R(0xa0)   // AND  A,B
    OP_AND_A_R(0xa1)   // AND  A,C
    OP_AND_A_R(0xa2)   // AND  A,D
    OP_AND_A_R(0xa3)   // AND  A,E
    OP_AND_A_R(0xa4)   // AND  A,H
    OP_AND_A_R(0xa5)   // AND  A,L

    // Special case for AND A,A, which is
    // effectively a NOP.
#define OPJ_AND_A_A(JmpType, Opcode) \
    JmpType(Opcode): { \
        zF = SZXYP[zA] | CZ80_HF; \
        RET(4); \
    }
#define OP_AND_A_A(Opcode) OPJ_AND_A_A(OP, Opcode)

    OP_AND_A_A(0xa7)   // AND  A,A

    // XOR

#define OP_XOR_INT(val, cycles) do { \
        zA ^= val; \
        zF = SZXYP[zA]; \
        RET(cycles); \
    } while (0)

#define OPJ_XOR_A_mHL(JmpType, Opcode) \
    JmpType(Opcode): { \
        uint8_t val; \
        PRE_IO(); \
        READ_BYTE(zHL, val); \
        POST_IO(); \
        OP_XOR_INT(val, 7); \
    }
#define OP_XOR_A_mHL(Opcode) OPJ_XOR_A_mHL(OP, Opcode)

    OP_XOR_A_mHL(0xae)  // XOR  A,(HL)

#define OPJ_XOR_A_n(JmpType, Opcode) \
    JmpType(Opcode): { \
        uint8_t val = FETCH_BYTE(); \
        OP_XOR_INT(val, 7); \
    }
#define OP_XOR_A_n(Opcode) OPJ_XOR_A_n(OP, Opcode)

    OP_XOR_A_n(0xee)   // XOR  A,n

#define OPJ_XOR_A_R(JmpType, Opcode) \
    JmpType(Opcode): { \
        uint8_t val = zR8(Opcode & 7); \
        OP_XOR_INT(val, 4); \
    }
#define OP_XOR_A_R(Opcode) OPJ_XOR_A_R(OP, Opcode)

    OP_XOR_A_R(0xa8)   // XOR  A,B
    OP_XOR_A_R(0xa9)   // XOR  A,C
    OP_XOR_A_R(0xaa)   // XOR  A,D
    OP_XOR_A_R(0xab)   // XOR  A,E
    OP_XOR_A_R(0xac)   // XOR  A,H
    OP_XOR_A_R(0xad)   // XOR  A,L

    // Special case for XOR A,A, which
    // clears the register.
    // TODO: Assign the full zFA in one shot?
#define OPJ_XOR_A_A(JmpType, Opcode) \
    JmpType(Opcode): { \
        zA = 0; \
        zF = SZXYP[0]; \
        RET(4); \
    }
#define OP_XOR_A_A(Opcode) OPJ_XOR_A_A(OP, Opcode)

    OP_XOR_A_A(0xaf)   // XOR  A,A

    // OR

#define OP_OR_INT(val, cycles) do { \
        zA |= val; \
        zF = SZXYP[zA]; \
        RET(cycles); \
    } while (0)

#define OPJ_OR_A_mHL(JmpType, Opcode) \
    JmpType(Opcode): { \
        uint8_t val; \
        PRE_IO(); \
        READ_BYTE(zHL, val); \
        POST_IO(); \
        OP_OR_INT(val, 7); \
    }
#define OP_OR_A_mHL(Opcode) OPJ_OR_A_mHL(OP, Opcode)

    OP_OR_A_mHL(0xb6)  // OR   A,(HL)

#define OPJ_OR_A_n(JmpType, Opcode) \
    JmpType(Opcode): { \
        uint8_t val = FETCH_BYTE(); \
        OP_OR_INT(val, 7); \
    }
#define OP_OR_A_n(Opcode) OPJ_OR_A_n(OP, Opcode)

    OP_OR_A_n(0xf6)   // OR   A,n

#define OPJ_OR_A_R(JmpType, Opcode) \
    JmpType(Opcode): { \
        uint8_t val = zR8(Opcode & 7); \
        OP_OR_INT(val, 4); \
    }
#define OP_OR_A_R(Opcode) OPJ_OR_A_R(OP, Opcode)

    OP_OR_A_R(0xb0)   // OR   A,B
    OP_OR_A_R(0xb1)   // OR   A,C
    OP_OR_A_R(0xb2)   // OR   A,D
    OP_OR_A_R(0xb3)   // OR   A,E
    OP_OR_A_R(0xb4)   // OR   A,H
    OP_OR_A_R(0xb5)   // OR   A,L

    // Special case for XOR A,A, which is
    // effectively a NOP.
    // TODO: Assign the full zFA in one shot?
#define OPJ_OR_A_A(JmpType, Opcode) \
    JmpType(Opcode): { \
        zF = SZXYP[zA]; \
        RET(4); \
    }
#define OP_OR_A_A(Opcode) OPJ_OR_A_A(OP, Opcode)

    OP_OR_A_A(0xb7)   // OR  A,A

    // MISC ARITHMETIC & CPU CONTROL

    OP(0x27):   // DAA
OP_DAA:
    {
        // New and improved DAA with fewer branches.
        // Reference: http://wikiti.brandonw.net/?title=Z80_Instruction_Set
        // NOTE: Using bitwise OR to improve performance.
        uint8_t tmp = zA;
        if (zF & CZ80_NF) {
            // Previous operation was a subtraction.
            if ((zF & CZ80_HF) | ((zA & 0x0F) > 9)) tmp -= 0x06;
            if ((zF & CZ80_CF) | (zA > 0x99)) tmp -= 0x60;
        } else {
            // Previous operation was an addition.
            if ((zF & CZ80_HF) | ((zA & 0x0F) > 9)) tmp += 0x06;
            if ((zF & CZ80_CF) | (zA > 0x99)) tmp += 0x60;
        }

        zF = SZXYP[tmp] |                /* Standard flags for the adjusted A */
            (zF & (CZ80_CF | CZ80_NF)) | /* Keep existing CF/NF flags */
            (zA > 0x99) |                /* Set CF if A > 0x99 (BCD carry) */
            ((zA ^ tmp) & CZ80_HF);      /* Set HF if A and tmp have differing HF */
        zA = tmp;
        RET(4);
    }

    OP(0x2f):   // CPL
OP_CPL:
        zA ^= 0xFF;
        zF = (zF & (CZ80_SF | CZ80_ZF | CZ80_PF | CZ80_CF)) |
            CZ80_HF | CZ80_NF |
            (zA & (CZ80_XF | CZ80_YF));
        RET(4);

    OP(0x37):   // SCF
OP_SCF:
        zF = (zF & (CZ80_SF | CZ80_ZF | CZ80_PF)) |
            (zA & (CZ80_XF | CZ80_YF)) |
            CZ80_CF;
        RET(4);

    OP(0x3f):   // CCF
OP_CCF:
        zF = ((zF & (CZ80_SF | CZ80_ZF | CZ80_PF | CZ80_CF)) |
            ((zF & CZ80_CF) << 4) |
            (zA & (CZ80_XF | CZ80_YF))) ^
            CZ80_CF;
        RET(4);

    OP(0x76):   // HALT
OP_HALT:
        // HALTED state
        CPU->Status |= CZ80_HALTED;
        // release remaining cycles...
        CCnt = 0;
        goto Cz80_Exec_Really_End;

    OP(0xf3):   // DI
OP_DI:
        zIFF = 0;
#if CZ80_DEBUG
        RET(4);
#else
        CCnt -= 4;
        // can't take interrupt after DI so we force next instruction execution
        goto Cz80_Exec;
#endif

    OP(0xfb):   // EI
    OP_EI:
        zIFF = CZ80_IFF | (CZ80_IFF << 8);
#if CZ80_DEBUG
        RET(4);
#else
        // release remaining cycles...
        CPU->CycleSup += CCnt - 4;
        CCnt = 0;
        // can't take interrupt after EI so we force next instruction execution
        goto Cz80_Exec;
#endif

    // 16 BITS ARITHMETIC

    OP(0x03):   // INC  BC
        zBC++;
        RET(6);

    OP(0x13):   // INC  DE
        zDE++;
        RET(6);

    OP(0x23):   // INC  HL
        data->W++;
        RET(6);

    OP(0x33):   // INC  SP
        zSP++;
        RET(6);

    OP(0x0b):   // DEC  BC
        zBC--;
        RET(6);

    OP(0x1b):   // DEC  DE
        zDE--;
        RET(6);

    OP(0x2b):   // DEC  HL
        data->W--;
        RET(6);

    OP(0x3b):   // DEC  SP
        zSP--;
        RET(6);

    // ADD16

// NOTE: Destination is always HL, IX, or IY.
#if CZ80_DEBUG
#define OPJ_ADD_RR_RR(JmpType, Opcode, RRsrc) \
    JmpType(Opcode): { \
        uint16_t src = RRsrc; \
        uint32_t res = src + data->W; \
        EXACT_ONLY(zWZ = data->W + 1); \
        zF = (zF & (CZ80_SF | CZ80_ZF | CZ80_VF)) |     /* S/Z/V flag */ \
            (((src ^ data->W ^ res) >> 8) & CZ80_HF) |  /* H flag */ \
            ((res >> 16) & CZ80_CF);                    /* C flag */ \
        data->W = (uint16_t)res; \
        RET(11); \
    }
#else
#define OPJ_ADD_RR_RR(JmpType, Opcode, RRsrc) \
    JmpType(Opcode): { \
        uint16_t src = RRsrc; \
        uint32_t res = src + data->W; \
        EXACT_ONLY(zWZ = data->W + 1); \
        zF = (zF & (CZ80_SF | CZ80_ZF | CZ80_VF)) |     /* S/Z/V flag */ \
            (((src ^ data->W ^ res) >> 8) & CZ80_HF) |  /* H flag */ \
            ((res >> 8) & (CZ80_XF | CZ80_YF)) |        /* X/Y flag */ \
            ((res >> 16) & CZ80_CF);                    /* C flag */ \
        data->W = (uint16_t)res; \
        RET(11); \
    }
#endif

#define OP_ADD_RR_RR(Opcode, RRsrc) OPJ_ADD_RR_RR(OP, Opcode, RRsrc)

    OP_ADD_RR_RR(0x39, zSP)       // ADD  HL,SP
    OP_ADD_RR_RR(0x29, data->W)   // ADD  HL,HL (TODO: Optimization?)
    OP_ADD_RR_RR(0x09, zBC)       // ADD  HL,BC
    OP_ADD_RR_RR(0x19, zDE)       // ADD  HL,DE

    // ROTATE

    {
        uint8_t A;
        uint8_t F;

        // NOTE: These opcodes have slightly different flag semantics
        // compared to the CB-prefix rotate instructions, so they
        // can't be combined.

    OP(0x07):   // RLCA
OP_RLCA:
        A = zA;
        zA = (A << 1) | (A >> 7);
        zF = (zF & (CZ80_SF | CZ80_ZF | CZ80_PF)) |     // S/Z/P flag
            (zA & (CZ80_XF | CZ80_YF | CZ80_CF));       // X/Y/C
        RET(4);

    OP(0x0f):   // RRCA
OP_RRCA:
        A = zA;
        zA = (A >> 1) | (A << 7);
        zF = (zF & (CZ80_SF | CZ80_ZF | CZ80_PF)) |     // S/Z/P flag
            (zA & (CZ80_XF | CZ80_YF)) |                // X/Y flag
            (A & CZ80_CF);                              // C flag
        RET(4);

    OP(0x17):   // RLA
OP_RLA:
        A = zA;
        F = zF;
        zA = (A << 1) | (F & CZ80_CF);
        zF = (F & (CZ80_SF | CZ80_ZF | CZ80_PF)) |      // S/Z/P flag
            (zA & (CZ80_XF | CZ80_YF)) |                // X/Y flag
            (A >> 7);                                   // C flag
        RET(4);

    OP(0x1f):   // RRA
OP_RRA:
        A = zA;
        F = zF;
        zA = (A >> 1) | (F << 7);
        zF = (F & (CZ80_SF | CZ80_ZF | CZ80_PF)) |      // S/Z/P flag
            (zA & (CZ80_XF | CZ80_YF)) |                // X/Y flag
            (A & CZ80_CF);                              // C flag
        RET(4);
    }

    // JUMP

// TODO: Don't use zWZ if !CZ80_EXACT?
#define DO_JP() do { \
    zWZ = GET_WORD(); \
    SET_PC(zWZ); \
    RET(10); \
} while (0)

// TODO: FETCH_WORD_WZ() macro?
#define OPJ_JP_NCOND(JmpType, Opcode, Cond) \
    JmpType(Opcode): \
        if (!(zF & Cond)) DO_JP(); \
        EXACT_ONLY(FETCH_WORD(zWZ)); \
        INEXACT_ONLY(PC += 2); \
        RET(10);
#define OP_JP_NCOND(Opcode, Cond) OPJ_JP_NCOND(OP, Opcode, Cond)

#define OPJ_JP_COND(JmpType, Opcode, Cond) \
    JmpType(Opcode): \
        if (zF & Cond) DO_JP(); \
        EXACT_ONLY(FETCH_WORD(zWZ)); \
        INEXACT_ONLY(PC += 2); \
        RET(10);
#define OP_JP_COND(Opcode, Cond) OPJ_JP_COND(OP, Opcode, Cond)

    OP_JP_NCOND(0xd2, CZ80_CF)   // JP   NC,nn
    OP_JP_COND( 0xda, CZ80_CF)   // JP   C,nn
    OP_JP_NCOND(0xe2, CZ80_VF)   // JP   PO,nn
    OP_JP_COND( 0xea, CZ80_VF)   // JP   PE,nn
    OP_JP_NCOND(0xf2, CZ80_SF)   // JP   P,nn
    OP_JP_COND( 0xfa, CZ80_SF)   // JP   M,nn
    OP_JP_NCOND(0xc2, CZ80_ZF)   // JP   NZ,nn
    OP_JP_COND( 0xca, CZ80_ZF)   // JP   Z,nn

#define OPJ_JP(JmpType, Opcode) \
    JmpType(Opcode): \
        DO_JP();
#define OP_JP(Opcode) OPJ_JP(OP, Opcode)

    OP_JP(0xc3)   // JP   nn

#if CZ80_EXACT
#define OPJ_JP_mHL(JmpType, Opcode) \
    JmpType(Opcode): \
        zWZ = data->W; \
        SET_PC(zWZ); \
        RET(4);
#else
#define OPJ_JP_mHL(JmpType, Opcode) \
    JmpType(Opcode): \
        SET_PC(data->W); \
        RET(4);
#endif
#define OP_JP_mHL(Opcode) OPJ_JP_mHL(OP, Opcode)

    OP_JP_mHL(0xe9)   // JP   (xx)

#define DO_JR() do { \
    int8_t val = FETCH_BYTE_S(); \
    /* no rebase needed here... */ \
    PC += val; \
    EXACT_ONLY(zWZ = PC); \
    RET(12); \
} while (0)

#define OPJ_JR_NCOND(JmpType, Opcode, Cond) \
    JmpType(Opcode): \
        if (!(zF & Cond)) DO_JR(); \
        PC++; \
        RET(7);
#define OP_JR_NCOND(Opcode, Cond) OPJ_JR_NCOND(OP, Opcode, Cond)

#define OPJ_JR_COND(JmpType, Opcode, Cond) \
    JmpType(Opcode): \
        if (zF & Cond) DO_JR(); \
        PC++; \
        RET(7);
#define OP_JR_COND(Opcode, Cond) OPJ_JR_COND(OP, Opcode, Cond)

    OP_JR_NCOND(0x30, CZ80_CF)   // JR   NC,n
    OP_JR_COND( 0x38, CZ80_CF)   // JR   C,n
    OP_JR_NCOND(0x20, CZ80_ZF)   // JR   NZ,n
    OP_JR_COND( 0x28, CZ80_ZF)   // JR   Z,n

#define OPJ_DJNZ(JmpType, Opcode) \
    JmpType(Opcode): \
        if (--zB) { \
            int8_t val = FETCH_BYTE_S(); \
            /* no rebase needed here... */ \
            PC += val; \
            EXACT_ONLY(zWZ = PC); \
            RET(13); \
        } \
        PC++; \
        RET(8);
#define OP_DJNZ(Opcode) OPJ_DJNZ(OP, Opcode)

    OP_DJNZ(0x10)   // DJNZ n

#define OPJ_JR(JmpType, Opcode) \
    JmpType(Opcode): \
        DO_JR();
#define OP_JR(Opcode) OPJ_JR(OP, Opcode)

    OP_JR(0x18)   // JR   n

    // CALL & RETURN

// TODO: Don't use zWZ if !CZ80_EXACT?
#define DO_CALL() do { \
    uint16_t oldRPC; \
    PRE_IO(); \
    FETCH_WORD(zWZ); \
    oldRPC = PC; \
    PUSH_16(oldRPC); \
    SET_PC(zWZ); \
    POST_IO(); \
    RET(17); \
} while (0)

#define OPJ_CALL_NCOND(JmpType, Opcode, Cond) \
    JmpType(Opcode): \
        if (!(zF & Cond)) DO_CALL(); \
        EXACT_ONLY(FETCH_WORD(zWZ)); \
        INEXACT_ONLY(PC += 2); \
        RET(10);
#define OP_CALL_NCOND(Opcode, Cond) OPJ_CALL_NCOND(OP, Opcode, Cond)

#define OPJ_CALL_COND(JmpType, Opcode, Cond) \
    JmpType(Opcode): \
        if (zF & Cond) DO_CALL(); \
        EXACT_ONLY(FETCH_WORD(zWZ)); \
        INEXACT_ONLY(PC += 2); \
        RET(10);
#define OP_CALL_COND(Opcode, Cond) OPJ_CALL_COND(OP, Opcode, Cond)

    OP_CALL_NCOND(0xd4, CZ80_CF)   // CALL NC,nn
    OP_CALL_COND( 0xdc, CZ80_CF)   // CALL C,nn
    OP_CALL_NCOND(0xe4, CZ80_VF)   // CALL PO,nn
    OP_CALL_COND( 0xec, CZ80_VF)   // CALL PE,nn
    OP_CALL_NCOND(0xf4, CZ80_SF)   // CALL P,nn
    OP_CALL_COND( 0xfc, CZ80_SF)   // CALL M,nn
    OP_CALL_NCOND(0xc4, CZ80_ZF)   // CALL NZ,nn
    OP_CALL_COND( 0xcc, CZ80_ZF)   // CALL Z,nn

#define OPJ_CALL(JmpType, Opcode) \
    JmpType(Opcode): \
        DO_CALL();
#define OP_CALL(Opcode) OPJ_CALL(OP, Opcode)

    OP_CALL(0xcd)   // CALL nn

// TODO: Don't use zWZ if !CZ80_EXACT?
#define DO_RET() do { \
    PRE_IO(); \
    POP_16(zWZ); \
    SET_PC(zWZ); \
    POST_IO(); \
    RET(10); \
} while (0)

#define OPJ_RET_NCOND(JmpType, Opcode, Cond) \
    JmpType(Opcode): \
        if (!(zF & Cond)) { \
            CCnt--; /* 11 cycles for RET cc if cc is true */ \
            DO_RET(); \
        } \
        RET(5);
#define OP_RET_NCOND(Opcode, Cond) OPJ_RET_NCOND(OP, Opcode, Cond)

#define OPJ_RET_COND(JmpType, Opcode, Cond) \
    JmpType(Opcode): \
        if (zF & Cond) { \
            CCnt--; /* 11 cycles for RET cc if cc is true */ \
            DO_RET(); \
        } \
        RET(5);
#define OP_RET_COND(Opcode, Cond) OPJ_RET_COND(OP, Opcode, Cond)

    OP_RET_NCOND(0xd0, CZ80_CF)   // RET  NC
    OP_RET_COND( 0xd8, CZ80_CF)   // RET  C
    OP_RET_NCOND(0xe0, CZ80_VF)   // RET  PO
    OP_RET_COND( 0xe8, CZ80_VF)   // RET  PE
    OP_RET_NCOND(0xf0, CZ80_SF)   // RET  P
    OP_RET_COND( 0xf8, CZ80_SF)   // RET  M
    OP_RET_NCOND(0xc0, CZ80_ZF)   // RET  NZ
    OP_RET_COND( 0xc8, CZ80_ZF)   // RET  Z

#define OPJ_RET(JmpType, Opcode) \
    JmpType(Opcode): \
        DO_RET();
#define OP_RET(Opcode) OPJ_RET(OP, Opcode)

    OP_RET(0xc9)   // RET

#if CZ80_EXACT
#define OPJ_RST(JmpType, Opcode) \
    JmpType(Opcode): \
        PUSH_16(PC); \
        zWZ = Opcode & 0x38; \
        SET_PC(zWZ); \
        RET(11);
#else
#define OPJ_RST(JmpType, Opcode) \
    JmpType(Opcode): \
        PUSH_16(PC); \
        SET_PC(Opcode & 0x38); \
        RET(11);
#endif
#define OP_RST(Opcode) OPJ_RST(OP, Opcode)

    OP_RST(0xc7)   // RST  0
    OP_RST(0xcf)   // RST  1
    OP_RST(0xd7)   // RST  2
    OP_RST(0xdf)   // RST  3
    OP_RST(0xe7)   // RST  4
    OP_RST(0xef)   // RST  5
    OP_RST(0xf7)   // RST  6
    OP_RST(0xff)   // RST  7

    // INPUT & OUTPUT

    {
        uint16_t adr;
        
    OP(0xd3):   // OUT  (n),A
OP_OUT_mN_A:
        adr = (zA << 8) | FETCH_BYTE();
        EXACT_ONLY(zWZ = (zA << 8) | ((adr + 1) & 0xFF));
        OUT(adr, zA);
        RET(11);

    OP(0xdb):   // IN   A,(n)
OP_IN_A_mN:
        adr = (zA << 8) | FETCH_BYTE();
        EXACT_ONLY(zWZ = adr + 1);
        IN(adr, zA);
        RET(11);
    }
    
    // PREFIXE
    
    OP(0xcb):   // CB PREFIXE (BIT & SHIFT INSTRUCTIONS)
/* CB_PREFIXE: */
        Opcode = FETCH_BYTE();
        INC_R_EXACT();
        #include "cz80_opcb.inc.c"

    OP(0xed):   // ED PREFIXE
ED_PREFIXE:
        CCnt -= 4;
        Opcode = FETCH_BYTE();
        INC_R_EXACT();
        #include "cz80_oped.inc.c"

    OP(0xdd):   // DD PREFIXE (IX)
DD_PREFIXE:
        data = pzIX;
        goto XY_PREFIXE;

    OP(0xfd):   // FD PREFIXE (IY)
FD_PREFIXE:
        data = pzIY;
        
XY_PREFIXE:
        CCnt -= 4;
        Opcode = FETCH_BYTE();
        INC_R_EXACT();
        #include "cz80_opxy.inc.c"

#if CZ80_USE_JUMPTABLE
#else
}
#endif
