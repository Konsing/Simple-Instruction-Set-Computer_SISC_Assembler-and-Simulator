start:  
        not r0 r1       ; negate r1
        and r0 r0 r1    ; r0 is 0000 0000
        not r1 r0       ; r1 is 1111 1111
        add r1 r1 r1    ; r1 is 1111 1110 
        not r1 r1       ; r1 is 0000 0001
        and r3 r0 r0    ; display r0 
loop:   
        add r2 r0 r1    ; add r0 and r1 and store in r2
        and r3 r2 r2    ; print r2
        add r0 r1 r2    ; add r1 and r2 and store in r0
        and r3 r0 r0    ; print r0
        add r1 r2 r0    ; add r2 and r0 and store in r1
        and r3 r1 r1    ; print r1
        add r2 r0 r1    ; add r0 and r1 and store in r2
        bnz loop        ; jump back to loop