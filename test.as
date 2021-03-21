.entry LIST
.extern W
MAIN: add r3, LIST
 lea W, r6
 mov r3, K
 sub r1, r4
EXIT: stop 


STR: .string "abcd"
LIST: .data 6, -9
 .data -100
K: .data 31
.extern L3 