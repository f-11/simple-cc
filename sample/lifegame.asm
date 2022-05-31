// *(0) ~ *(7)  : board
// *(8) ~ *(15) : temporary of board
// *(16)        : tmp1
// *(17) ~ *(19): use on getaroundnum
// *(20), *(21) : store x,y at main
// *(22), *(23) : alivep
LI 0,144
ST 0,0,7
LI 0,8
ST 0,1,7
LI 0,136
ST 0,2,7
LI 0,120
ST 0,3,7
LI 0,4
ST 0,4,7
LI 0,3
ST 0,5,7
LI 0,12
ST 0,6,7
LI 0,2
ST 0,7,7
LI 0,14
LI 1,5
//BAL .get_around_num
//HLT
BAL .main
HLT

.mod // r[0] mod r[1] ; r[1] must be power of 2
ADDI 1,-1
AND 0,1
CMP 0,7
BR

// x = *(18) = r[0], y = *(19) = r[1]
// *(17) = num
.get_around_num
ST 0,18,7
ST 1,19,7
ADDI 0,-1   // x-1, y-1
ADDI 1,-1
BAL .alivep
ST 0,17,7
LD 0,18,7
LD 1,19,7
ADDI 1,-1   // x, y-1
BAL .alivep
LD 1,17,7
ADD 0,1
ST 0,17,7
LD 0,18,7
LD 1,19,7
ADDI 0,1    // x+1, y-1
ADDI 1,-1
BAL .alivep
LD 1,17,7
ADD 0,1
ST 0,17,7
LD 0,18,7
LD 1,19,7
ADDI 0,-1   // x-1, y
BAL .alivep
LD 1,17,7
ADD 0,1
ST 0,17,7
LD 0,18,7
LD 1,19,7
ADDI 0,1    // x+1, y
BAL .alivep
LD 1,17,7
ADD 0,1
ST 0,17,7
LD 0,18,7
LD 1,19,7
ADDI 0,-1   // x-1, y+1
ADDI 1,1
BAL .alivep
LD 1,17,7
ADD 0,1
ST 0,17,7
LD 0,18,7
LD 1,19,7
ADDI 1,1    // x, y+1
BAL .alivep
LD 1,17,7
ADD 0,1
ST 0,17,7
LD 0,18,7
LD 1,19,7
ADDI 0,1    // x+1, y+1
ADDI 1,1
BAL .alivep
LD 1,17,7
ADD 0,1
BR

// x = *(22) = r[0], y = *(23) = r[1]
// return 1 when cell alives
.alivep
ST 0,22,7
ST 1,23,7

// x = x mod 16
LI 1,16
BAL .mod
ST 0,22,7
// y = y mod 8
LD 0,23,7
LI 1,8
BAL .mod
MOV 1,0     // r[1] = y
LD 0,22,7   // r[0] = x

LD 2,0,1    // r[2] = board[y]
MOV 1,0     // r[1] = x
LI 0,1
BAL .shift_reg  // r[0] = 1 << (15 - x)
AND 0,2
CMPI 0,0
BE 1
LI 0,1
BR

// main loop
// x = *(20) = r[4], y = *(21) = r[5]
// alive?(x,y) = r[6]
.main
LI 5,0  //y = 0

.LcountY

// display
LD 0,0,5
BAL .out_reg

LI 4,0  //x = 0

.LcountX
MOV 0,4
MOV 1,5

ST 4,20,7
ST 5,21,7

BAL .alivep
MOV 6,0     // r[6] = alive? 1 : 0;

LD 0,20,7
LD 1,21,7
BAL .get_around_num // r[0] = around num

LD 4,20,7
LD 5,21,7

CMPI 6,0  // cell alives?
BE .Ldead
CMPI 0,1  // around num <= 1 ?
BLE 4
LI 1,4
CMP 1,0   // 4 <= arouond num ?
BLE 1
B .Ldeathend
LI 6,0      //death
B .Ldeathend
.Ldead
CMPI 0,3
BNE .Ldeathend
LI 6,1      //born
.Ldeathend

MOV 0,6
MOV 1,4
BAL .shift_reg
LD 1,8,5
OR 0,1
ST 0,8,5

ADDI 4,1
CMPI 4, 16
BNE .LcountX

ADDI 5, 1
CMPI 5, 8
BNE .LcountY

//copy
LI 3,0
LD 2,8,3
ST 2,0,3
ADDI 3,1
CMPI 3,8
BNE -5

B .main
BR

.out_reg    // out r[0],r[5]
CMPI 5,0
BNE 2
OUT 0,0
BR
CMPI 5,1
BNE 2
OUT 0,1
BR
CMPI 5,2
BNE 2
OUT 0,2
BR
CMPI 5,3
BNE 2
OUT 0,3
BR
CMPI 5,4
BNE 2
OUT 0,4
BR
CMPI 5,5
BNE 2
OUT 0,5
BR
CMPI 5,6
BNE 2
OUT 0,6
BR
CMPI 5,7
BNE 2
OUT 0,7
BR
BR

.shift_reg  //r[0] << 15 - r[1]; if r[1] > 15 then nop
CMPI 1,0
BNE 2
SLL 0,15
BR  
CMPI 1,1
BNE 2
SLL 0,14
BR
CMPI 1,2
BNE 2
SLL 0,13 
BR
CMPI 1,3
BNE 2
SLL 0,12
BR
CMPI 1,4
BNE 2
SLL 0,11
BR
CMPI 1,5
BNE 2
SLL 0,10 
BR
CMPI 1,6
BNE 2
SLL 0,9 
BR
CMPI 1,7
BNE 2
SLL 0,8 
BR
CMPI 1,8
BNE 2
SLL 0,7 
BR
CMPI 1,9
BNE 2
SLL 0,6 
BR
CMPI 1,10
BNE 2
SLL 0,5 
BR
CMPI 1,11
BNE 2
SLL 0,4 
BR
CMPI 1,12
BNE 2
SLL 0,3 
BR
CMPI 1,13
BNE 2
SLL 0,2 
BR
CMPI 1,14
BNE 2
SLL 0,1 
BR
BR

