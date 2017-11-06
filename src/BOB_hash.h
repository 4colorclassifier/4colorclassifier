#ifndef COLORINGCLASSIFER_BOB_HASH_H
#define COLORINGCLASSIFER_BOB_HASH_H

#include <cstdlib>
#include <cstdint>

#define mix(a,b,c) \
{ \
  a -= b; a -= c; a ^= (c>>13); \
  b -= c; b -= a; b ^= (a<<8); \
  c -= a; c -= b; c ^= (b>>13); \
  a -= b; a -= c; a ^= (c>>12);  \
  b -= c; b -= a; b ^= (a<<16); \
  c -= a; c -= b; c ^= (b>>5); \
  a -= b; a -= c; a ^= (c>>3);  \
  b -= c; b -= a; b ^= (a<<10); \
  c -= a; c -= b; c ^= (b>>15); \
}

template<size_t len, uint32_t seed>
uint32_t BOB(const void * buf)
{
    const char * str = (const char *)buf;
    //register ub4 a,b,c,len;
    unsigned int a,b,c;
    unsigned int initval = seed;
    /* Set up the internal state */
    //len = length;
    a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
    c = initval;         /* the previous hash value */

    /*---------------------------------------- handle most of the key */
//    while (len >= 12)
//    {
//        a += (str[0] +((unsigned int)str[1]<<8) +((unsigned int)str[2]<<16) +((unsigned int)str[3]<<24));
//        b += (str[4] +((unsigned int)str[5]<<8) +((unsigned int)str[6]<<16) +((unsigned int)str[7]<<24));
//        c += (str[8] +((unsigned int)str[9]<<8) +((unsigned int)str[10]<<16)+((unsigned int)str[11]<<24));
//        mix(a,b,c);
//        str += 12; len -= 12;
//    }

    /*------------------------------------- handle the last 11 bytes */
    c += len;
    switch(len)              /* all the case statements fall through */
    {
    case 11: c+=((unsigned int)str[10]<<24);
        // fall through
    case 10: c+=((unsigned int)str[9]<<16);
        // fall through
    case 9 : c+=((unsigned int)str[8]<<8);
        // fall through
        /* the first byte of c is reserved for the length */
    case 8 : b+=((unsigned int)str[7]<<24);
        // fall through
    case 7 : b+=((unsigned int)str[6]<<16);
        // fall through
    case 6 : b+=((unsigned int)str[5]<<8);
        // fall through
    case 5 : b+=str[4];
        // fall through
    case 4 : a+=((unsigned int)str[3]<<24);
        // fall through
    case 3 : a+=((unsigned int)str[2]<<16);
        // fall through
    case 2 : a+=((unsigned int)str[1]<<8);
        // fall through
    case 1 : a+=str[0];
        // fall through
        /* case 0: nothing left to add */
        default:
            break;
    }
    mix(a,b,c);
    /*-------------------------------------------- report the result */
    return c;
}

template<uint32_t seed>
uint32_t BOB_str(const void * buf, size_t len)
{
    const char * str = (const char *)buf;
    //register ub4 a,b,c,len;
    unsigned int a,b,c;
    unsigned int initval = seed;
    /* Set up the internal state */
    //len = length;
    a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
    c = initval;         /* the previous hash value */

    /*---------------------------------------- handle most of the key */
    while (len >= 12)
    {
        a += (str[0] +((unsigned int)str[1]<<8) +((unsigned int)str[2]<<16) +((unsigned int)str[3]<<24));
        b += (str[4] +((unsigned int)str[5]<<8) +((unsigned int)str[6]<<16) +((unsigned int)str[7]<<24));
        c += (str[8] +((unsigned int)str[9]<<8) +((unsigned int)str[10]<<16)+((unsigned int)str[11]<<24));
        mix(a,b,c);
        str += 12; len -= 12;
    }

    /*------------------------------------- handle the last 11 bytes */
    c += len;
    switch(len)              /* all the case statements fall through */
    {
    case 11: c+=((unsigned int)str[10]<<24);
        // fall through
    case 10: c+=((unsigned int)str[9]<<16);
        // fall through
    case 9 : c+=((unsigned int)str[8]<<8);
        // fall through
        /* the first byte of c is reserved for the length */
    case 8 : b+=((unsigned int)str[7]<<24);
        // fall through
    case 7 : b+=((unsigned int)str[6]<<16);
        // fall through
    case 6 : b+=((unsigned int)str[5]<<8);
        // fall through
    case 5 : b+=str[4];
        // fall through
    case 4 : a+=((unsigned int)str[3]<<24);
        // fall through
    case 3 : a+=((unsigned int)str[2]<<16);
        // fall through
    case 2 : a+=((unsigned int)str[1]<<8);
        // fall through
    case 1 : a+=str[0];
        // fall through
        /* case 0: nothing left to add */
    default:
        break;
    }
    mix(a,b,c);
    /*-------------------------------------------- report the result */
    return c;
}

class BOBHash
{
    uint32_t seed;
public:
    BOBHash(uint32_t _seed): seed(_seed)
    {
    }

    uint32_t run(const void * buf, size_t len)
    {
        const char * str = (const char *)buf;
        //register ub4 a,b,c,len;
        unsigned int a,b,c;
        unsigned int initval = seed;
        /* Set up the internal state */
        //len = length;
        a = b = 0x9e3779b9;  /* the golden ratio; an arbitrary value */
        c = initval;         /* the previous hash value */

        /*---------------------------------------- handle most of the key */
        while (len >= 12)
        {
            a += (str[0] +((unsigned int)str[1]<<8) +((unsigned int)str[2]<<16) +((unsigned int)str[3]<<24));
            b += (str[4] +((unsigned int)str[5]<<8) +((unsigned int)str[6]<<16) +((unsigned int)str[7]<<24));
            c += (str[8] +((unsigned int)str[9]<<8) +((unsigned int)str[10]<<16)+((unsigned int)str[11]<<24));
            mix(a,b,c);
            str += 12; len -= 12;
        }

        /*------------------------------------- handle the last 11 bytes */
        c += len;
        switch(len)              /* all the case statements fall through */
        {
        case 11: c+=((unsigned int)str[10]<<24);
            // fall through
        case 10: c+=((unsigned int)str[9]<<16);
            // fall through
        case 9 : c+=((unsigned int)str[8]<<8);
            // fall through
            /* the first byte of c is reserved for the length */
        case 8 : b+=((unsigned int)str[7]<<24);
            // fall through
        case 7 : b+=((unsigned int)str[6]<<16);
            // fall through
        case 6 : b+=((unsigned int)str[5]<<8);
            // fall through
        case 5 : b+=str[4];
            // fall through
        case 4 : a+=((unsigned int)str[3]<<24);
            // fall through
        case 3 : a+=((unsigned int)str[2]<<16);
            // fall through
        case 2 : a+=((unsigned int)str[1]<<8);
            // fall through
        case 1 : a+=str[0];
            // fall through
            /* case 0: nothing left to add */
        default:
            break;
        }
        mix(a,b,c);
        /*-------------------------------------------- report the result */
        return c;
    }
};

uint32_t (*BOB1_str)(const void * buf, size_t len) = BOB_str<0x01a725e4>;
uint32_t (*BOB2_str)(const void * buf, size_t len) = BOB_str<0xb7a2fb64>;

uint32_t (*BOB1)(const void * buf) = BOB<8, 0x01a725e4>;
uint32_t (*BOB2)(const void * buf) = BOB<8, 0xb7a2fb64>;
uint32_t (*BOB3)(const void * buf) = BOB<8, 0x9574726c>;
uint32_t (*BOB4)(const void * buf) = BOB<8, 0xe1b60a3c>;
uint32_t (*BOB5)(const void * buf) = BOB<8, 0x251738e4>;
uint32_t (*BOB6)(const void * buf) = BOB<8, 0x7940f935>;
uint32_t (*BOB7)(const void * buf) = BOB<8, 0xf47b3921>;
uint32_t (*BOB8)(const void * buf) = BOB<8, 0x79aaa783>;
uint32_t (*BOB9)(const void * buf) = BOB<8, 0x8aa403ae>;
uint32_t (*BOB10)(const void * buf) = BOB<8, 0x899fb734>;
uint32_t (*BOB11)(const void * buf) = BOB<8, 0xd612d7e0>;
uint32_t (*BOB12)(const void * buf) = BOB<8, 0xa966dba1>;
uint32_t (*BOB13)(const void * buf) = BOB<8, 0xc4745645>;
uint32_t (*BOB14)(const void * buf) = BOB<8, 0x07fd50b7>;
uint32_t (*BOB15)(const void * buf) = BOB<8, 0x34b31b48>;
uint32_t (*BOB16)(const void * buf) = BOB<8, 0xe3b734e8>;

uint32_t (*BOB17)(const void * buf) = BOB<8, 0x5ccb3998>;
uint32_t (*BOB18)(const void * buf) = BOB<8, 0x399665fd>;
uint32_t (*BOB19)(const void * buf) = BOB<8, 0x7c5831bb>;
uint32_t (*BOB20)(const void * buf) = BOB<8, 0x3f5fb74e>;
uint32_t (*BOB21)(const void * buf) = BOB<8, 0x55149568>;
uint32_t (*BOB22)(const void * buf) = BOB<8, 0x2f0329f4>;
uint32_t (*BOB23)(const void * buf) = BOB<8, 0x09e1eb15>;
uint32_t (*BOB24)(const void * buf) = BOB<8, 0x40937258>;
uint32_t (*BOB25)(const void * buf) = BOB<8, 0x028b72f2>;
uint32_t (*BOB26)(const void * buf) = BOB<8, 0x649f472a>;
uint32_t (*BOB27)(const void * buf) = BOB<8, 0x4dd337fc>;
uint32_t (*BOB28)(const void * buf) = BOB<8, 0x6a5ea1c5>;
uint32_t (*BOB29)(const void * buf) = BOB<8, 0x4759ca2b>;
uint32_t (*BOB30)(const void * buf) = BOB<8, 0x33fecdec>;
uint32_t (*BOB31)(const void * buf) = BOB<8, 0x65dc2ced>;
uint32_t (*BOB32)(const void * buf) = BOB<8, 0x552ee399>;

uint32_t (*BOB_hashs[])(const void * buf) = {
        BOB1, BOB2, BOB3, BOB4, BOB5, BOB6, BOB7, BOB8,
        BOB9, BOB10, BOB11, BOB12, BOB13, BOB14, BOB15, BOB16,
        BOB17,
        BOB18,
        BOB19,
        BOB20,
        BOB21,
        BOB22,
        BOB23,
        BOB24,
        BOB25,
        BOB26,
        BOB27,
        BOB28,
        BOB29,
        BOB30,
        BOB31,
        BOB32,
};

#endif //COLORINGCLASSIFER_BOB_HASH_H
