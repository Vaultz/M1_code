  .include bignum.asm

modexp:
  ;; r1 = r2 ^ r3 mod r4
  ;; r  = b  ^ e

  ;; backup arguments and ra
  sub r30 r30 #1                ; decr sp
  mov !r30 r31                  ; push ra

  ;; init r5 (res)
  mov r5 r1                     ; r5 points on res (r1)
  mov r20 r5                    ; r20 is res
  mov r21 #1                    ; r21 is 1 (initial len(res))
  cal bignum_init               ; res = 0
  mov !r5,#-1 #1                ; res = 1

  ;; init r6 (base)
  mov r6 #500000                ; base is stored at @500000
  mov r20 r6                    ; r20 is base
  mov r21 r2                    ; r21 is b
  cal bignum_copy               ; base = b

  ;; init r7 (exp)
  mov r7 #510000                ; exp is stored at @510000
  mov r20 r7                    ; r20 is exp
  mov r21 r3                    ; r21 is e
  cal bignum_copy               ; exp = e

  ;; init r8 (tmp)
  mov r8 #520000                ; tmp is stored at @520000
  mov r20 r8                    ; r20 is tmp
  mov r21 #1                    ; r21 is 1 (initial len(tmp))
  cal bignum_init               ; tmp = 0

  dbg 240

_modexp_loop:
  and r1 @510000 #1                  ; r1 = (e AND 1)
  cmp r2 @510000 r1                  ; IF e == r1: r2 = 0
  beq _modexp_loop_mul r2 #0         ; IF r2 == 0: goto _modexp_loop_mul

; TODO : check if the exponent's lsb is 0
_modexp_loop_mul:
  


_modexp_loop_sqr:

_modexp_loop_shift:

  jmp _modexp_loop              ; loop

_modexp_end:
  ;; restore ra
  mov r31 !r30                  ; pop ra
  add r30 r30 #1                ; incr sp
  ret
