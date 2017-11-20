.include modexp.asm

main:
  sub r30 r30 #1                ; decr sp
  mov !r30 r31                  ; push ra

  mov r1 #30000
  mov r2 #10000
  mov r3 #20000
  mov r4 #40000

  ;; message
  mov r20 r2
  mov r21 #5
  cal bignum_init
  mov !r20,#-1 #116
  mov !r20,#-2 #117
  mov !r20,#-3 #108
  mov !r20,#-4 #97
  mov !r20,#-5 #83
  cal bignum_print

  ;; exponent
  mov r20 r3
  mo r21 #5
  cal bignum_init
  ;each hexadecimal value is converted to a decimal value and written in a register
  mov !r20,#-1 #18
  mov !r20,#-2 #171
  mov !r20,#-3 #52
  mov !r20,#-4 #205
  mov !r20,#-5 #86
  mov !r20,#-6 #239
  mov !r20,#-7 #87
  mov !r20,#-8 #7
  cal bignum_print

  ;; modulus
  mov r20 r4
  mov r21 #5
  cal bignum_init
  mov !r20,#-1 #28
  mov !r20,#-2 #131
  mov !r20,#-3 #252
  mov !r20,#-4 #47
  mov !r20,#-5 #3
  cal bignum_print

  mov r20 r1
  mov r21 r2
  mov r22 r3
  mov r24 r4
  cal modexp

  mov r20 r1
  cal bignum_print

  mov r31 !r30                  ; pop ra
  add r30 r30 #1                ; incr sp
  ret
