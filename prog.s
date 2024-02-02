.data
str_2: .asciz "\n"
str_1: .asciz "Your Number Is: "
str_0: .asciz "Enter a number: "
.align 8

.text
init: 
pushq %rbp
movq %rsp, %rbp
addq $16, %rbp
subq $0, %rsp
#Fn body <init>
#Fn epilogue <init>
lbl_0: 
addq $0, %rsp
popq %rbp
retq


.globl fact
fun_fact: 
pushq %rbp
movq %rsp, %rbp
addq $16, %rbp
subq $48, %rsp
#Fn body fact
#            getarg 1 [num]
#            [tmp0] := [num] EQ64 0
movq 0(%rbp), %rax
movq $0, %rbx
cmpq %rbx, %rax
movq $0, %rax
sete %al
movq %rax, -24(%rbp)
#            IFZ [tmp0] GOTO lbl_2
movq -24(%rbp), %rax
cmpq $1, %rax
jne lbl_2
#            setret 1
movq $1, %rax
#            goto lbl_1
jmp lbl_1
lbl_2: 
#lbl_2:      nop
nop
#            [tmp1] := [num] SUB64 1
movq 0(%rbp), %rax
movq $1, %rbx
subq %rbx, %rax
movq %rax, -32(%rbp)
#            setarg 1 [tmp1]
movq -32(%rbp), %rax
pushq %rax
#            call fact
callq fun_fact
addq $8, %rsp
#            getret [tmp2]
movq %rax, -40(%rbp)
#            [tmp3] := [tmp2] MULT64 [num]
movq -40(%rbp), %rax
movq 0(%rbp), %rbx
imulq %rbx
movq %rax, -48(%rbp)
#            setret [tmp3]
movq -48(%rbp), %rax
#            goto lbl_1
jmp lbl_1
#Fn epilogue fact
lbl_1: 
addq $48, %rsp
popq %rbp
retq

.globl main
main: 
callq init

pushq %rbp
movq %rsp, %rbp
addq $16, %rbp
subq $16, %rsp
#Fn body main
#            WRITE str_0
movq $str_0, %rax
movq %rax, %rdi
callq printString
#            READ [a]
callq getInt
movq %rax, -24(%rbp)
#            WRITE str_1
movq $str_1, %rax
movq %rax, %rdi
callq printString
#            setarg 1 [a]
movq -24(%rbp), %rax
pushq %rax
#            call fact
callq fun_fact
addq $8, %rsp
#            getret [tmp0]
movq %rax, -32(%rbp)
#            WRITE [tmp0]
movq -32(%rbp), %rax
movq %rax, %rdi
callq printInt
#            WRITE str_2
movq $str_2, %rax
movq %rax, %rdi
callq printString
#            setret 0
movq $0, %rax
#            goto lbl_3
jmp lbl_3
#Fn epilogue main
lbl_3: 
addq $16, %rsp
popq %rbp
retq

