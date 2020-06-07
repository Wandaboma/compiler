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
process:
move $fp, $sp
mod:
move $fp, $sp
main:
move $fp, $sp
label1:
j label3
label2:
j label1
label3:
label4:
j label6
label5:
j label8
label7:
label8:
j label10
label9:
label10:
j label4
label6:
