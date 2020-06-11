.data
_prompt: .asciiz "Enter an integer:"
_ret: .asciiz "\n"
.globl main
.text
read:
li $v0, 4
la $a0, _prompt
syscall
li $v0, 5
syscall
jr $ra
write:
li $v0, 1
syscall
li $v0, 4
la $a0, _ret
syscall
move $v0, $0
jr $ra
#FUNCTION Swap :
Swap:
move $fp, $sp
addi $sp, $sp, -116
#PARAM a
#PARAM l
#PARAM h
#t3 := a
lw $t0, 8($fp)
add $t8, $fp, -16
sw $t0, 0($t8)
#t4 := l
lw $t0, 12($fp)
add $t8, $fp, -20
sw $t0, 0($t8)
#t6 := t4 * #4
lw $t0, -20($fp)
li $t1, 4
mul $t0, $t0, $t1
add $t8, $fp, -28
sw $t0, 0($t8)
#t2 := t3 + t6
lw $t0, -16($fp)
lw $t1, -28($fp)
add $t0, $t0, $t1
add $t8, $fp, -12
sw $t0, 0($t8)
#t1 := *t2
lw $t8, -12($fp)
lw $t0, 0($t8)
add $t8, $fp, -8
sw $t0, 0($t8)
#temp := t1
lw $t0, -8($fp)
add $t8, $fp, -4
sw $t0, 0($t8)
#t10 := a
lw $t0, 8($fp)
add $t8, $fp, -44
sw $t0, 0($t8)
#t11 := l
lw $t0, 12($fp)
add $t8, $fp, -48
sw $t0, 0($t8)
#t13 := t11 * #4
lw $t0, -48($fp)
li $t1, 4
mul $t0, $t0, $t1
add $t8, $fp, -56
sw $t0, 0($t8)
#t8 := t10 + t13
lw $t0, -44($fp)
lw $t1, -56($fp)
add $t0, $t0, $t1
add $t8, $fp, -36
sw $t0, 0($t8)
#t15 := a
lw $t0, 8($fp)
add $t8, $fp, -64
sw $t0, 0($t8)
#t16 := h
lw $t0, 16($fp)
add $t8, $fp, -68
sw $t0, 0($t8)
#t18 := t16 * #4
lw $t0, -68($fp)
li $t1, 4
mul $t0, $t0, $t1
add $t8, $fp, -76
sw $t0, 0($t8)
#t14 := t15 + t18
lw $t0, -64($fp)
lw $t1, -76($fp)
add $t0, $t0, $t1
add $t8, $fp, -60
sw $t0, 0($t8)
#t9 := *t14
lw $t8, -60($fp)
lw $t0, 0($t8)
add $t8, $fp, -40
sw $t0, 0($t8)
#*t8 := t9
lw $t0, -40($fp)
add $t8, $fp, -36
lw $t8, 0($t8)
sw $t0, 0($t8)
#t22 := a
lw $t0, 8($fp)
add $t8, $fp, -92
sw $t0, 0($t8)
#t23 := h
lw $t0, 16($fp)
add $t8, $fp, -96
sw $t0, 0($t8)
#t25 := t23 * #4
lw $t0, -96($fp)
li $t1, 4
mul $t0, $t0, $t1
add $t8, $fp, -104
sw $t0, 0($t8)
#t20 := t22 + t25
lw $t0, -92($fp)
lw $t1, -104($fp)
add $t0, $t0, $t1
add $t8, $fp, -84
sw $t0, 0($t8)
#t21 := temp
lw $t0, -4($fp)
add $t8, $fp, -88
sw $t0, 0($t8)
#*t20 := t21
lw $t0, -88($fp)
add $t8, $fp, -84
lw $t8, 0($t8)
sw $t0, 0($t8)
#t27 := #0
li $t0, 0
add $t8, $fp, -112
sw $t0, 0($t8)
#RETURN t27
lw $t0, -112($fp)
move $v0, $t0
jr $ra
#FUNCTION Partition :
Partition:
move $fp, $sp
addi $sp, $sp, -248
#PARAM b
#PARAM low
#PARAM high
#t31 := b
lw $t0, 8($fp)
add $t8, $fp, -16
sw $t0, 0($t8)
#t32 := low
lw $t0, 12($fp)
add $t8, $fp, -20
sw $t0, 0($t8)
#t34 := t32 * #4
lw $t0, -20($fp)
li $t1, 4
mul $t0, $t0, $t1
add $t8, $fp, -28
sw $t0, 0($t8)
#t30 := t31 + t34
lw $t0, -16($fp)
lw $t1, -28($fp)
add $t0, $t0, $t1
add $t8, $fp, -12
sw $t0, 0($t8)
#t29 := *t30
lw $t8, -12($fp)
lw $t0, 0($t8)
add $t8, $fp, -8
sw $t0, 0($t8)
#base := t29
lw $t0, -8($fp)
add $t8, $fp, -4
sw $t0, 0($t8)
#LABEL label1 :
label1:
#t36 := low
lw $t0, 12($fp)
add $t8, $fp, -36
sw $t0, 0($t8)
#t37 := high
lw $t0, 16($fp)
add $t8, $fp, -40
sw $t0, 0($t8)
#IF t36 < t37 GOTO label2
lw $t0, -36($fp)
lw $t1, -40($fp)
blt $t0, $t1, label2
#GOTO label3
j label3
#LABEL label2 :
label2:
#LABEL label4 :
label4:
#t40 := low
lw $t0, 12($fp)
add $t8, $fp, -52
sw $t0, 0($t8)
#t41 := high
lw $t0, 16($fp)
add $t8, $fp, -56
sw $t0, 0($t8)
#IF t40 < t41 GOTO label7
lw $t0, -52($fp)
lw $t1, -56($fp)
blt $t0, $t1, label7
#GOTO label6
j label6
#LABEL label7 :
label7:
#t47 := b
lw $t0, 8($fp)
add $t8, $fp, -80
sw $t0, 0($t8)
#t48 := high
lw $t0, 16($fp)
add $t8, $fp, -84
sw $t0, 0($t8)
#t50 := t48 * #4
lw $t0, -84($fp)
li $t1, 4
mul $t0, $t0, $t1
add $t8, $fp, -92
sw $t0, 0($t8)
#t46 := t47 + t50
lw $t0, -80($fp)
lw $t1, -92($fp)
add $t0, $t0, $t1
add $t8, $fp, -76
sw $t0, 0($t8)
#t44 := *t46
lw $t8, -76($fp)
lw $t0, 0($t8)
add $t8, $fp, -68
sw $t0, 0($t8)
#t45 := base
lw $t0, -4($fp)
add $t8, $fp, -72
sw $t0, 0($t8)
#IF t44 >= t45 GOTO label5
lw $t0, -68($fp)
lw $t1, -72($fp)
bge $t0, $t1, label5
#GOTO label6
j label6
#LABEL label5 :
label5:
#t54 := high
lw $t0, 16($fp)
add $t8, $fp, -108
sw $t0, 0($t8)
#t55 := #1
li $t0, 1
add $t8, $fp, -112
sw $t0, 0($t8)
#t53 := t54 - t55
lw $t0, -108($fp)
lw $t1, -112($fp)
sub $t0, $t0, $t1
add $t8, $fp, -104
sw $t0, 0($t8)
#high := t53
lw $t0, -104($fp)
add $t8, $fp, 16
sw $t0, 0($t8)
#GOTO label4
j label4
#LABEL label6 :
label6:
#t58 := b
lw $t0, 8($fp)
add $t8, $fp, -124
sw $t0, 0($t8)
#t59 := low
lw $t0, 12($fp)
add $t8, $fp, -128
sw $t0, 0($t8)
#t61 := high
lw $t0, 16($fp)
add $t8, $fp, -136
sw $t0, 0($t8)
#ARG t61
addi $sp , $sp, -4
lw $t0, -136($fp)
sw $t0, 0($sp)
#ARG t59
addi $sp , $sp, -4
lw $t0, -128($fp)
sw $t0, 0($sp)
#ARG t58
addi $sp , $sp, -4
lw $t0, -124($fp)
sw $t0, 0($sp)
#t63 := CALL Swap
addi $sp, $sp, -8
sw $ra, 4($sp)
sw $fp, 0($sp)
jal Swap
move $sp, $fp
lw $fp, 0($sp)
lw $ra, 4($sp)
add $t8, $fp, -144
sw $v0, 0($t8)
#LABEL label8 :
label8:
#t64 := low
lw $t0, 12($fp)
add $t8, $fp, -148
sw $t0, 0($t8)
#t65 := high
lw $t0, 16($fp)
add $t8, $fp, -152
sw $t0, 0($t8)
#IF t64 < t65 GOTO label11
lw $t0, -148($fp)
lw $t1, -152($fp)
blt $t0, $t1, label11
#GOTO label10
j label10
#LABEL label11 :
label11:
#t71 := b
lw $t0, 8($fp)
add $t8, $fp, -176
sw $t0, 0($t8)
#t72 := low
lw $t0, 12($fp)
add $t8, $fp, -180
sw $t0, 0($t8)
#t74 := t72 * #4
lw $t0, -180($fp)
li $t1, 4
mul $t0, $t0, $t1
add $t8, $fp, -188
sw $t0, 0($t8)
#t70 := t71 + t74
lw $t0, -176($fp)
lw $t1, -188($fp)
add $t0, $t0, $t1
add $t8, $fp, -172
sw $t0, 0($t8)
#t68 := *t70
lw $t8, -172($fp)
lw $t0, 0($t8)
add $t8, $fp, -164
sw $t0, 0($t8)
#t69 := base
lw $t0, -4($fp)
add $t8, $fp, -168
sw $t0, 0($t8)
#IF t68 <= t69 GOTO label9
lw $t0, -164($fp)
lw $t1, -168($fp)
ble $t0, $t1, label9
#GOTO label10
j label10
#LABEL label9 :
label9:
#t78 := low
lw $t0, 12($fp)
add $t8, $fp, -204
sw $t0, 0($t8)
#t79 := #1
li $t0, 1
add $t8, $fp, -208
sw $t0, 0($t8)
#t77 := t78 + t79
lw $t0, -204($fp)
lw $t1, -208($fp)
add $t0, $t0, $t1
add $t8, $fp, -200
sw $t0, 0($t8)
#low := t77
lw $t0, -200($fp)
add $t8, $fp, 12
sw $t0, 0($t8)
#GOTO label8
j label8
#LABEL label10 :
label10:
#t82 := b
lw $t0, 8($fp)
add $t8, $fp, -220
sw $t0, 0($t8)
#t83 := low
lw $t0, 12($fp)
add $t8, $fp, -224
sw $t0, 0($t8)
#t85 := high
lw $t0, 16($fp)
add $t8, $fp, -232
sw $t0, 0($t8)
#ARG t85
addi $sp , $sp, -4
lw $t0, -232($fp)
sw $t0, 0($sp)
#ARG t83
addi $sp , $sp, -4
lw $t0, -224($fp)
sw $t0, 0($sp)
#ARG t82
addi $sp , $sp, -4
lw $t0, -220($fp)
sw $t0, 0($sp)
#t87 := CALL Swap
addi $sp, $sp, -8
sw $ra, 4($sp)
sw $fp, 0($sp)
jal Swap
move $sp, $fp
lw $fp, 0($sp)
lw $ra, 4($sp)
add $t8, $fp, -240
sw $v0, 0($t8)
#GOTO label1
j label1
#LABEL label3 :
label3:
#t88 := low
lw $t0, 12($fp)
add $t8, $fp, -244
sw $t0, 0($t8)
#RETURN t88
lw $t0, -244($fp)
move $v0, $t0
jr $ra
#FUNCTION QuickSort :
QuickSort:
move $fp, $sp
addi $sp, $sp, -128
#PARAM c
#PARAM low1
#PARAM high1
#t90 := low1
lw $t0, 12($fp)
add $t8, $fp, -4
sw $t0, 0($t8)
#t91 := high1
lw $t0, 16($fp)
add $t8, $fp, -8
sw $t0, 0($t8)
#IF t90 < t91 GOTO label12
lw $t0, -4($fp)
lw $t1, -8($fp)
blt $t0, $t1, label12
#GOTO label13
j label13
#LABEL label12 :
label12:
#t95 := c
lw $t0, 8($fp)
add $t8, $fp, -28
sw $t0, 0($t8)
#t96 := low1
lw $t0, 12($fp)
add $t8, $fp, -32
sw $t0, 0($t8)
#t98 := high1
lw $t0, 16($fp)
add $t8, $fp, -40
sw $t0, 0($t8)
#ARG t98
addi $sp , $sp, -4
lw $t0, -40($fp)
sw $t0, 0($sp)
#ARG t96
addi $sp , $sp, -4
lw $t0, -32($fp)
sw $t0, 0($sp)
#ARG t95
addi $sp , $sp, -4
lw $t0, -28($fp)
sw $t0, 0($sp)
#t94 := CALL Partition
addi $sp, $sp, -8
sw $ra, 4($sp)
sw $fp, 0($sp)
jal Partition
move $sp, $fp
lw $fp, 0($sp)
lw $ra, 4($sp)
add $t8, $fp, -24
sw $v0, 0($t8)
#base1 := t94
lw $t0, -24($fp)
add $t8, $fp, -20
sw $t0, 0($t8)
#t101 := c
lw $t0, 8($fp)
add $t8, $fp, -52
sw $t0, 0($t8)
#t102 := low1
lw $t0, 12($fp)
add $t8, $fp, -56
sw $t0, 0($t8)
#t105 := base1
lw $t0, -20($fp)
add $t8, $fp, -68
sw $t0, 0($t8)
#t106 := #1
li $t0, 1
add $t8, $fp, -72
sw $t0, 0($t8)
#t104 := t105 - t106
lw $t0, -68($fp)
lw $t1, -72($fp)
sub $t0, $t0, $t1
add $t8, $fp, -64
sw $t0, 0($t8)
#ARG t104
addi $sp , $sp, -4
lw $t0, -64($fp)
sw $t0, 0($sp)
#ARG t102
addi $sp , $sp, -4
lw $t0, -56($fp)
sw $t0, 0($sp)
#ARG t101
addi $sp , $sp, -4
lw $t0, -52($fp)
sw $t0, 0($sp)
#t109 := CALL QuickSort
addi $sp, $sp, -8
sw $ra, 4($sp)
sw $fp, 0($sp)
jal QuickSort
move $sp, $fp
lw $fp, 0($sp)
lw $ra, 4($sp)
add $t8, $fp, -84
sw $v0, 0($t8)
#t110 := c
lw $t0, 8($fp)
add $t8, $fp, -88
sw $t0, 0($t8)
#t112 := base1
lw $t0, -20($fp)
add $t8, $fp, -96
sw $t0, 0($t8)
#t113 := #1
li $t0, 1
add $t8, $fp, -100
sw $t0, 0($t8)
#t111 := t112 + t113
lw $t0, -96($fp)
lw $t1, -100($fp)
add $t0, $t0, $t1
add $t8, $fp, -92
sw $t0, 0($t8)
#t116 := high1
lw $t0, 16($fp)
add $t8, $fp, -112
sw $t0, 0($t8)
#ARG t116
addi $sp , $sp, -4
lw $t0, -112($fp)
sw $t0, 0($sp)
#ARG t111
addi $sp , $sp, -4
lw $t0, -92($fp)
sw $t0, 0($sp)
#ARG t110
addi $sp , $sp, -4
lw $t0, -88($fp)
sw $t0, 0($sp)
#t118 := CALL QuickSort
addi $sp, $sp, -8
sw $ra, 4($sp)
sw $fp, 0($sp)
jal QuickSort
move $sp, $fp
lw $fp, 0($sp)
lw $ra, 4($sp)
add $t8, $fp, -120
sw $v0, 0($t8)
#LABEL label13 :
label13:
#t119 := #0
li $t0, 0
add $t8, $fp, -124
sw $t0, 0($t8)
#RETURN t119
lw $t0, -124($fp)
move $v0, $t0
jr $ra
#FUNCTION main :
main:
move $fp, $sp
addi $sp, $sp, -236
#t121 := #8
li $t0, 8
add $t8, $fp, -8
sw $t0, 0($t8)
#n := t121
lw $t0, -8($fp)
add $t8, $fp, -4
sw $t0, 0($t8)
#
#t123 := #0
li $t0, 0
add $t8, $fp, -52
sw $t0, 0($t8)
#i := t123
lw $t0, -52($fp)
add $t8, $fp, -48
sw $t0, 0($t8)
#LABEL label14 :
label14:
#t125 := i
lw $t0, -48($fp)
add $t8, $fp, -60
sw $t0, 0($t8)
#t126 := n
lw $t0, -4($fp)
add $t8, $fp, -64
sw $t0, 0($t8)
#IF t125 < t126 GOTO label15
lw $t0, -60($fp)
lw $t1, -64($fp)
blt $t0, $t1, label15
#GOTO label16
j label16
#LABEL label15 :
label15:
#t131 := &arr
addi $t0, $fp, -44
add $t8, $fp, -84
sw $t0, 0($t8)
#t132 := i
lw $t0, -48($fp)
add $t8, $fp, -88
sw $t0, 0($t8)
#t134 := t132 * #4
lw $t0, -88($fp)
li $t1, 4
mul $t0, $t0, $t1
add $t8, $fp, -96
sw $t0, 0($t8)
#t129 := t131 + t134
lw $t0, -84($fp)
lw $t1, -96($fp)
add $t0, $t0, $t1
add $t8, $fp, -76
sw $t0, 0($t8)
#READ t130
addi $sp, $sp, -4
sw $ra, 0($sp)
jal read
lw $ra, 0($sp)
addi $sp, $sp, 4
add $t8, $fp, -80
sw $v0, 0($t8)
#*t129 := t130
lw $t0, -80($fp)
add $t8, $fp, -76
lw $t8, 0($t8)
sw $t0, 0($t8)
#t137 := i
lw $t0, -48($fp)
add $t8, $fp, -108
sw $t0, 0($t8)
#t138 := #1
li $t0, 1
add $t8, $fp, -112
sw $t0, 0($t8)
#t136 := t137 + t138
lw $t0, -108($fp)
lw $t1, -112($fp)
add $t0, $t0, $t1
add $t8, $fp, -104
sw $t0, 0($t8)
#i := t136
lw $t0, -104($fp)
add $t8, $fp, -48
sw $t0, 0($t8)
#GOTO label14
j label14
#LABEL label16 :
label16:
#t141 := &arr
addi $t0, $fp, -44
add $t8, $fp, -124
sw $t0, 0($t8)
#t142 := #0
li $t0, 0
add $t8, $fp, -128
sw $t0, 0($t8)
#t145 := n
lw $t0, -4($fp)
add $t8, $fp, -140
sw $t0, 0($t8)
#t146 := #1
li $t0, 1
add $t8, $fp, -144
sw $t0, 0($t8)
#t144 := t145 - t146
lw $t0, -140($fp)
lw $t1, -144($fp)
sub $t0, $t0, $t1
add $t8, $fp, -136
sw $t0, 0($t8)
#ARG t144
addi $sp , $sp, -4
lw $t0, -136($fp)
sw $t0, 0($sp)
#ARG t142
addi $sp , $sp, -4
lw $t0, -128($fp)
sw $t0, 0($sp)
#ARG t141
addi $sp , $sp, -4
lw $t0, -124($fp)
sw $t0, 0($sp)
#t149 := CALL QuickSort
addi $sp, $sp, -8
sw $ra, 4($sp)
sw $fp, 0($sp)
jal QuickSort
move $sp, $fp
lw $fp, 0($sp)
lw $ra, 4($sp)
add $t8, $fp, -156
sw $v0, 0($t8)
#t150 := #0
li $t0, 0
add $t8, $fp, -160
sw $t0, 0($t8)
#i := t150
lw $t0, -160($fp)
add $t8, $fp, -48
sw $t0, 0($t8)
#LABEL label17 :
label17:
#t152 := i
lw $t0, -48($fp)
add $t8, $fp, -168
sw $t0, 0($t8)
#t153 := n
lw $t0, -4($fp)
add $t8, $fp, -172
sw $t0, 0($t8)
#IF t152 < t153 GOTO label18
lw $t0, -168($fp)
lw $t1, -172($fp)
blt $t0, $t1, label18
#GOTO label19
j label19
#LABEL label18 :
label18:
#t158 := &arr
addi $t0, $fp, -44
add $t8, $fp, -192
sw $t0, 0($t8)
#t159 := i
lw $t0, -48($fp)
add $t8, $fp, -196
sw $t0, 0($t8)
#t161 := t159 * #4
lw $t0, -196($fp)
li $t1, 4
mul $t0, $t0, $t1
add $t8, $fp, -204
sw $t0, 0($t8)
#t157 := t158 + t161
lw $t0, -192($fp)
lw $t1, -204($fp)
add $t0, $t0, $t1
add $t8, $fp, -188
sw $t0, 0($t8)
#t156 := *t157
lw $t8, -188($fp)
lw $t0, 0($t8)
add $t8, $fp, -184
sw $t0, 0($t8)
#WRITE t156
lw $a0, -184($fp)
addi $sp, $sp, -4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addi $sp, $sp, 4
#t164 := i
lw $t0, -48($fp)
add $t8, $fp, -216
sw $t0, 0($t8)
#t165 := #1
li $t0, 1
add $t8, $fp, -220
sw $t0, 0($t8)
#t163 := t164 + t165
lw $t0, -216($fp)
lw $t1, -220($fp)
add $t0, $t0, $t1
add $t8, $fp, -212
sw $t0, 0($t8)
#i := t163
lw $t0, -212($fp)
add $t8, $fp, -48
sw $t0, 0($t8)
#GOTO label17
j label17
#LABEL label19 :
label19:
#t168 := #0
li $t0, 0
add $t8, $fp, -232
sw $t0, 0($t8)
#RETURN t168
lw $t0, -232($fp)
move $v0, $t0
jr $ra
