
;file src.as

.entry ARR
.extern W

.define sz=4
MAIN:		mov r5, ARR[sz]
LOOP:		jmp L2
		prn #-5
		mov STR[5], STR[3]
		sub r1, r4
		cmp r5, #sz
		bne END
L2:		inc K 
		bne LOOP 
END:		stop
.define len = 4
STR:		.string "abcdef"
LIST: .data	6, -9, len
K:		.data 22

