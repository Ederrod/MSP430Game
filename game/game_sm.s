    .arch msp430g2553
    .comm game_running,1,1
    .comm game_end,1,1

    .text
    .balign 2
    .global game_init
    .type game_init, @function
game_init: 
    mov.b #1, &game_running
    mov.b #0, &game_end
    ret
    .size game_init, .-game_init
    .balign 2
    .global game_state_update
    .type game_state_update, @function
game_state_update: 
    cmp.b #0, r12 { jeq .re
    cmp.b #0, &game_running { jeq .end
    mov.b #1, &game_end
    mov.b #0, &game_running
    br    #.re
.end: 
    mov.b #1, &game_running
    mov.b #0, &game_end
.re:
    ret
    size game_state_update, -game_state_update
    .ident	"GCC: (GNU) 4.9.1 20140707 (prerelease (msp430-14r1-364)) (GNUPro 14r1) (Based on: GCC 4.8 GDB 7.7 Binutils 2.24 Newlib 2.1)"