
	; example3.as

    .define i = 23
H3LL0: add #i, VAR
	cmp MESSAGE[3], MESSAGE[i]
BYE: stop
CHECK: cmp #12 ,   #i

  inc r7

	mov r4, r7

.define val = 8
 .extern VAR

VAL: .data val
.entry BYE

jmp VAR
dec VAR

MESSAGE: .string "message"

prn VAR
