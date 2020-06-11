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
main:
move $fp, $sp
addi $sp, $sp, 0
li $t0, 0
add $t8, $fp, 0
sw $t0, 0($t8)
lw $t0, 0($fp)
add $t8, $fp, 0
sw $t0, 0($t8)
li $t0, 1
add $t8, $fp, 0
sw $t0, 0($t8)
lw $t0, 0($fp)
add $t8, $fp, 0
sw $t0, 0($t8)
li $t0, 0
add $t8, $fp, 0
sw $t0, 0($t8)
lw $t0, 0($fp)
add $t8, $fp, 0
sw $t0, 0($t8)
addi $sp, $sp, -4
sw $ra, 0($sp)
jal read
lw $ra, 0($sp)
addi $sp, $sp, 4
add $t8, $fp, 0
sw $v0, 0($t8)
lw $t0, 0($fp)
add $t8, $fp, 0
sw $t0, 0($t8)
label1:
li $t0, 1
add $t8, $fp, 0
sw $t0, 0($t8)
lw $t0, 0($fp)
add $t8, $fp, 0
sw $t0, 0($t8)
lw $t0, 0($fp)
lw $t1, 0($fp)
blt $t0, $t1, label2
j label3
label2:
lw $t0, 0($fp)
add $t8, $fp, 0
sw $t0, 0($t8)
lw $t0, 0($fp)
add $t8, $fp, 0
sw $t0, 0($t8)
lw $t0, 0($fp)
lw $t1, 0($fp)
add $t0, $t0, $t1
add $t8, $fp, 0
sw $t0, 0($t8)
lw $t0, 0($fp)
add $t8, $fp, 0
sw $t0, 0($t8)
lw $t0, 0($fp)
add $t8, $fp, 0
sw $t0, 0($t8)
lw $a0, 0($fp)
addi $sp, $sp, -4
sw $ra, 0($sp)
jal write
lw $ra, 0($sp)
addi $sp, $sp, 4
lw $t0, 0($fp)
add $t8, $fp, 0
sw $t0, 0($t8)
lw $t0, 0($fp)
add $t8, $fp, 0
sw $t0, 0($t8)
lw $t0, 0($fp)
add $t8, $fp, 0
sw $t0, 0($t8)
lw $t0, 0($fp)
add $t8, $fp, 0
sw $t0, 0($t8)
lw $t0, 0($fp)
add $t8, $fp, 0
sw $t0, 0($t8)
li $t0, 1
add $t8, $fp, 0
sw $t0, 0($t8)
lw $t0, 0($fp)
lw $t1, 0($fp)
add $t0, $t0, $t1
add $t8, $fp, 0
sw $t0, 0($t8)
lw $t0, 0($fp)
add $t8, $fp, 0
sw $t0, 0($t8)
j label1
label3:
li $t0, 0
add $t8, $fp, 0
sw $t0, 0($t8)
lw $t0, 0($fp)
move $v0, $t0
jr $ra
