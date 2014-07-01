
#include "cartridge.h"

// PORT C is [R1, R2, R3, R4, L1, L2, L3, L4] (Multiplexer)
// PORT L is Left (MOSFET Drivers)
// PORT A is Right (MOSFET Drivers)

// Put in to a printhead class

/*

.global _Z8fireHeadhhhh
    .type   _Z8fireHeadhhhh, @function
_Z8fireHeadhhhh:
    push r29
    push r28
    in r28,__SP_L__
    in r29,__SP_H__
    sbiw r28,8
    in __tmp_reg__,__SREG__
    cli
    out __SP_H__,r29
    out __SREG__,__tmp_reg__
    out __SP_L__,r28

*/

void fire_head(uint8_t rPrim, uint8_t rAddr, uint8_t lPrim, uint8_t lAddr) {
    int x1, x2;
    /*

        // prologue: function
        // frame size = 8
        std Y+5,r24
        std Y+6,r22
        std Y+7,r20
        std Y+8,r18
        ldd r24,Y+5
        tst r24
        brne .L25
        ldd r24,Y+6
        tst r24
        brne .L25
        ldd r24,Y+7
        tst r24
        brne .L25
        ldd r24,Y+8
        tst r24
        brne .+2
        rjmp .L27

    */
    if (rPrim || rAddr || lPrim || lAddr) {
        /*

            .L25:
            ldi r26,lo8(40)
            ldi r27,hi8(40)
            ldi r30,lo8(40)
            ldi r31,hi8(40) // 40 is PORTC
            ld r25,Z
            ldd r24,Y+8 // Y+8 is lAddr
            or r24,r25 // PORTC | lAddr
            st X,r24
            ldi r26,lo8(40)
            ldi r27,hi8(40)
            ldi r30,lo8(40)
            ldi r31,hi8(40) // 40 is PORTC
            ld r24,Z
            mov r18,r24
            ldd r24,Y+6 // Y+6 is rAddr
            mov r24,r24 // ?
            ldi r25,lo8(0)
            swap r24
            swap r25
            andi r25,0xf0
            eor r25,r24
            andi r24,0xf0
            eor r25,r24
            or r24,r18
            st X,r24

        */
        PORTC = (PORTC | int(lAddr)); //Assign this to port C to load it into the cannon.

        //x1 = int(rAddr) << 4 | int(rAddr) >4; //Reverse
        //x2 = (x1 & 0x33) << 2 | (x1 & 0xcc) >2;
        //PORTC = (PORTC | ((x2 & 0x55) << 1 | (x2 & 0xaa) >1);

        // 2.6µS outer (t_h)
        // 2.5µS inner (t_pw)

        PORTC = (PORTC | (int(rAddr) << 4));

        asm volatile("nop\n\t"
                     "nop\n\t"
                     "nop\n\t"
                     "nop\n\t"
                     "nop\n\t"
                     "nop\n\t"
                     "nop\n\t"
        );

        /*

            ldi r30,lo8(267)
            ldi r31,hi8(267)
            ldd r24,Y+7
            st Z,r24

            ldi r30,lo8(34)
            ldi r31,hi8(34)
            ldd r24,Y+5
            st Z,r24

            ldi r24,lo8(6)
            ldi r25,hi8(6)
            call delayMicroseconds

        */

        PORTL = int(lPrim);
        PORTA = int(rPrim);

        delayMicroseconds(6);

        asm volatile("nop\n\t"
                     "nop\n\t"
                     "nop\n\t"
                     "nop\n\t"
                     "nop\n\t"
                     "nop\n\t"
                     "nop\n\t"
                     "nop\n\t"
                     "nop\n\t"
                     "nop\n\t"
                     "nop\n\t"
                     "nop\n\t"
                     "nop\n\t"
                     "nop\n\t"
                     "nop\n\t"
        );

        /*

            ldi r30,lo8(34)
            ldi r31,hi8(34)
            st Z,__zero_reg__

            ldi r30,lo8(267)
            ldi r31,hi8(267)
            st Z,__zero_reg__

        */
        PORTA = 0;
        PORTL = 0;

        asm volatile("nop\n\t"
                     "nop\n\t"
                     "nop\n\t"
                     "nop\n\t"
                     "nop\n\t"
                     "nop\n\t"
                     "nop\n\t");

        /*

            ldi r30,lo8(40)
            ldi r31,hi8(40)
            st Z,__zero_reg__

        */
        PORTC = 0;
        //delayMicroseconds(500);
    }
}
