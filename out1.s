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
#FUNCTION main :
main:
move $fp, $sp
addi $sp, $sp, -224
#t1 := #0
li $t0, 0
add $t8, $fp, -8
sw $t0, 0($t8)
#x := t1
lw $t0, -8($fp)
add $t8, $fp, -4
sw $t0, 0($t8)
#t3 := #0
li $t0, 0
add $t8, $fp, -20
sw $t0, 0($t8)
#y := t3
lw $t0, -20($fp)
add $t8, $fp, -16
sw $t0, 0($t8)
#t5 := #0
li $t0, 0
add $t8, $fp, -32
sw $t0, 0($t8)
#i := t5
lw $t0, -32($fp)
add $t8, $fp, -28
sw $t0, 0($t8)
#t7 := #0
li $t0, 0
add $t8, $fp, -44
sw $t0, 0($t8)
#j := t7
lw $t0, -44($fp)
add $t8, $fp, -40
sw $t0, 0($t8)
#
#LABEL label1 :
label1:
#t9 := i
lw $t0, -28($fp)
add $t8, $fp, -92
sw $t0, 0($t8)
#t10 := #10
li $t0, 10
add $t8, $fp, -96
sw $t0, 0($t8)
#IF t9 < t10 GOTO label2
lw $t0, -92($fp)
lw $t1, -96($fp)
blt $t0, $t1, label2
#GOTO label3
j label3
#LABEL label2 :
label2:
#t15 := &a
addi $t0, $fp, -88
add $t8, $fp, -116
sw $t0, 0($t8)
#t16 := i
lw $t0, -28($fp)
add $t8, $fp, -120
sw $t0, 0($t8)
#t18 := t16 * #4
lw $t0, -120($fp)
li $t1, 4
mul $t0, $t0, $t1
add $t8, $fp, -128
sw $t0, 0($t8)
#t13 := t15 + t18
lw $t0, -116($fp)
lw $t1, -128($fp)
add $t0, $t0, $t1
add $t8, $fp, -108
sw $t0, 0($t8)
#t14 := i
lw $t0, -28($fp)
add $t8, $fp, -112
sw $t0, 0($t8)
#*t13 := t14
lw $t0, -112($fp)
add $t8, $fp, -108
lw $t8, 0($t8)
sw $t0, 0($t8)
#t21 := i
lw $t0, -28($fp)
add $t8, $fp, -140
sw $t0, 0($t8)
#t22 := #1
li $t0, 1
add $t8, $fp, -144
sw $t0, 0($t8)
#t20 := t21 + t22
lw $t0, -140($fp)
lw $t1, -144($fp)
add $t0, $t0, $t1
add $t8, $fp, -136
sw $t0, 0($t8)
#i := t20
lw $t0, -136($fp)
add $t8, $fp, -28
sw $t0, 0($t8)
#GOTO label1
j label1
#LABEL label3 :
label3:
#LABEL label4 :
label4:
#t25 := j
lw $t0, -40($fp)
add $t8, $fp, -156
sw $t0, 0($t8)
#t26 := #10
li $t0, 10
add $t8, $fp, -160
sw $t0, 0($t8)
#IF t25 < t26 GOTO label5
lw $t0, -156($fp)
lw $t1, -160($fp)
blt $t0, $t1, label5
#GOTO label6
j label6
#LABEL label5 :
label5:
#t31 := &a
addi $t0, $fp, -88
add $t8, $fp, -180
sw $t0, 0($t8)
#t32 := j
lw $t0, -40($fp)
add $t8, $fp, -184
sw $t0, 0($t8)
#t34 := t32 * #4
lw $t0, -184($fp)
li $t1, 4
mul $t0, $t0, $t1
add $t8, $fp, -192
sw $t0, 0($t8)
#t30 := t31 + t34
lw $t0, -180($fp)
lw $t1, -192($fp)
add $t0, $t0, $t1
add $t8, $fp, -176
sw $t0, 0($t8)
#t29 := *t30
lw $t8, -176($fp)
lw $t0, 0($t8)
add $t8, $fp, -172
sw $t0, 0($t8)
#WRITE t29
lw $a0, -172($fp)
addi $sp, $sp, -4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addi $sp, $sp, 4
#t37 := j
lw $t0, -40($fp)
add $t8, $fp, -204
sw $t0, 0($t8)
#t38 := #1
li $t0, 1
add $t8, $fp, -208
sw $t0, 0($t8)
#t36 := t37 + t38
lw $t0, -204($fp)
lw $t1, -208($fp)
add $t0, $t0, $t1
add $t8, $fp, -200
sw $t0, 0($t8)
#j := t36
lw $t0, -200($fp)
add $t8, $fp, -40
sw $t0, 0($t8)
#GOTO label4
j label4
#LABEL label6 :
label6:
#t41 := #0
li $t0, 0
add $t8, $fp, -220
sw $t0, 0($t8)
#RETURN t41
lw $t0, -220($fp)
move $v0, $t0
jr $ra
