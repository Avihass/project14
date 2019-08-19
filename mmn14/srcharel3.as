;file src3.as

.entry ROW
.extern R
.extern A

.define godel=5

MAIN:		add #1,ROW[godel]
LOOP:		jmp R
		mov A,r0
ROW:		bne LOOP
STR:		.string "abba"
 

