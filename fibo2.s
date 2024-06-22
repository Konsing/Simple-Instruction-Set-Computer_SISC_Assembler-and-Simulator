start:  
        not r0 r1       ; negate r1
        and r0 r0 r1    ; r0 has 0000 0000
        not r1 r0       ; r1 is 1111 1111
        add r1 r1 r1    ; r1 is 1111 1110 
        not r1 r1       ; r1 has 0000 0001
        and r3 r0 r0    ; print r0 (0)
        add r2 r1 r1    ; r2 0000 0010
        add r2 r2 r2    ; r2 0000 0100
        add r2 r2 r2    ; r2 0000 1000
        add r2 r1 r2    ; r2 0000 1001
        add r2 r1 r2    ; r2 0000 1010
        add r2 r1 r2    ; r2 0000 1011
        add r2 r1 r2    ; r2 0000 1100
        not r2 r2       ; r2 is 1111 0011 (-12)
        add r2 r2 r1    ; add 1 to r2 because we printed r0
        bnz update      ; jump to update
loop:
        and r1 r3 r3    ; reset r1 to it's previous value
        add r0 r1 r0    ; r0 is r1 + r0 first fib
        add r1 r0 r1    ; r3 is r0 + r1 2nd fib
        and r3 r0 r0    ; print r0     
update: 
        and r3 r1 r1    ; print r1 
        not r1 r0       ; negate r1
        and r1 r0 r1    ; r1 has 0000 0000
        not r1 r1       ; r1 is 1111 1111
        add r1 r1 r1    ; r1 is 1111 1110 
        not r1 r1       ; r1 has 0000 0001
        add r2 r2 r1    ; add 1 to r2 because we printed r0
        add r2 r2 r1    ; add 1 to r2 because we printed r1
        bnz loop        ; go back to loop if not r3 not 0
        and r3 r3 r3    ; resets z flag to be NOT 0
end:    
        bnz end         ; loop over and over again