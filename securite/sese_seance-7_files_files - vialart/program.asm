number:
  ;prn r10

  mov r10 #5
  prn r10

  not r10 #5
  prn r10

  mov r1 #1
  mov r2 #2

  ;r1 > r2 : cpm writes 1 in r3
  cmp r3 r1 r2
  prn r3

  ;r1 < r2 : cpm write -1 in r3
  cmp r3 r2 r1
  prn r3
ret

helloWorld:
  mov r5 #72
  mov r6 #101
  mov r7 #108
  mov r8 #108
  mov r9 #111
  mov r10 #32
  mov r11 #33
ret


main:
  ;cal number
  cal helloWorld
