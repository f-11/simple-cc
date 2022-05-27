// *(0) ~ *(7)  : board
// *(8) ~ *(15) : temporary of board
// *(16)        : tmp1
// *(17) ~ *(19): use on getaroundnum
// *(20), *(21) : store x,y at main
// *(22), *(23) : alivep

.display:
LD 0,0,7
OUT 0,0
LD 0,1,7
OUT 0,1
LD 0,2,7
OUT 0,2
LD 0,3,7
OUT 0,3
LD 0,4,7
OUT 0,4
LD 0,5,7
OUT 0,5
LD 0,6,7
OUT 0,6
LD 0,7,7
OUT 0,7
BR

.shift_reg:  //r[0] << 15 - r[1]
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
CMPI 1,15
BNE 2
SLL 0,0 
BR
BR

.mod: // r[0] mod r[1]
CMPI 0,0
BLT 5
CMP 0,1
BLT 2
SUB 0,1
B -4
B 2
MOV 0,1
ADDI 0,1
BR

.get_around_num: //x = r[0], y = r[1]
ST 7,17,7 
ST 0,18,7
ST 1,19,7
ADDI 0,-1   // x-1, y-1
ADDI 1,-1
call .alivep
ST 0,17,7
LD 0,18,7
LD 1,19.7
ADDI 1,-1
call .alivep
LD 1,17,7
ADD 0,1
ST 0,17,7
LD 0,18,7
LD 1,19.7
ADDI 0,1
ADDI 1,-1
call .alivep
LD 1,17,7
ADD 0,1
ST 0,17,7
LD 0,18,7
LD 1,19.7
ADDI 0,-1
call .alivep
LD 1,17,7
ADD 0,1
ST 0,17,7
LD 0,18,7
LD 1,19.7
ADDI 0,1
call .alivep
LD 1,17,7
ADD 0,1
ST 0,17,7
LD 0,18,7
LD 1,19.7
ADDI 0,-1
ADDI 0,1
call .alivep
LD 1,17,7
ADD 0,1
ST 0,17,7
LD 0,18,7
LD 1,19.7
ADDI 1,1
call .alivep
LD 1,17,7
ADD 0,1
ST 0,17,7
LD 0,18,7
LD 1,19.7
ADDI 0,1
ADDI 1,1
call .alivep
LD 1,17,7
ADD 0,1
BR

.alivep:  //x=r[0],y=r[1]; return 1 when cell alives
ST 0,22,7
ST 1,23,7

LI 1,16
call .mod
ST 0,22,7
LD 0,23,7
LI 1,8
call .mod
MOV 1,0
LD 0,22,7

LD 2,0,1
MOV 1,0
LI 0,1
call .shift_reg
OR 0,2
CMPI 0,0
BE 1
LI 0,1
BR

main://x = r[4], y=r[5], alive?(x,y) = r[6]
call .display

LI 5,0

.LcountY
LI 4,0
ST 7,16,7 //clear tmp1

.LcountX
MOV 0,4
MOV 1,4

ST 4,20,7
ST 5,21,7

call .alivep
MOV 6,0

LD 4,20,7
LD 5,21,7

MOV 0,4
MOV 1,4
call .get_around_num

LD 4,20,7
LD 5,21,7

CMPI 6,0  // if cell alives
BE .Ldeath:
CMPI 0,1
BLE 4
LI 1,4
CMP 1,0 //arouond num >= 4 ?
BLE 1
B .Ldeathend
LI 6,0  //death
B .Ldeathend
.Ldeath:
CMPI 0,3
LI 6,1 //born
.Ldeathend

//ここまででr[6]に次世代の生死があるはず
MOV 0,6
MOV 1,4
call .shift_reg
LD 1,16,7
OR 0,1
ST 0,16,7

ADDI 4,1
CMPI 4, 16
BE 2
B .LcountX

//tmp1 -> temporary
LD 0,16,7
ST 0,8,5

ADDI 5, 1
CMPI 5, 8
BE 2
B .LcountY

//copy
LI 3,0
LD 2,8,3
ST 2,0,3
ADDI 3,1
CMPI 3,8
BE 2
B -7

B .main
HLT
