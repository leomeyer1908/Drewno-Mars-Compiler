#include <ostream>
#include "3ac.hpp"

namespace drewno_mars{

void IRProgram::allocGlobals(){
	for (auto entry : globals){
		entry.second->setMemoryLoc(entry.second->getName());
	}
}

void IRProgram::datagenX64(std::ostream& out){
	out << ".data\n";
	for (auto entry : globals){
		if (!(entry.first->getKind() == FN)) {
			out << entry.second->getMemoryLoc() << ": .quad 0\n";
		}
	}
	for (auto entry : strings){
		out << entry.first->valString() << ": " << ".asciz " << entry.second << "\n";
	}
	//Put this directive after you write out strings
	// so that everything is aligned to a quadword value
	// again
	out << ".align 8\n\n";
}

void IRProgram::toX64(std::ostream& out){
	allocGlobals();
	datagenX64(out);
	out << ".text\n";
	getInitProc()->toX64(out);
	out << "\n";
	for (auto proc : *procs) {
		out << ".globl " << proc->getName() << "\n";
		proc->toX64(out); 
	}
}

void Procedure::allocLocals(){
	size_t last_var = 16;
	for (auto local : locals){
		last_var += local.second->getWidth();
		local.second->setMemoryLoc("-" + to_string(last_var) + "(%rbp)");
	}
	for (auto tmp : temps){
		last_var += tmp->getWidth();
		tmp->setMemoryLoc("-" + to_string(last_var) + "(%rbp)");
	}
	last_var = 0; //reverse it to get the formals
	for (auto rit = formals.rbegin(); rit != formals.rend(); ++rit){
		(*rit)->setMemoryLoc(to_string(last_var) + "(%rbp)");
		last_var += (*rit)->getWidth();
	}
}

void Procedure::toX64(std::ostream& out){
	//Allocate all locals
	allocLocals();

	enter->codegenLabels(out);

	//Init globals
	// if (getName().compare("main") == 0) {
	// 	size_t last_var = 0;
	// 	for (auto tmp : getProg()->getInitProc()->getTemps()){
	// 		last_var += tmp->getWidth();
	// 		tmp->setMemoryLoc("-" + to_string(last_var) + "(%rbp)");
	// 	}
	// 	for (auto quad : *getProg()->getInitProc()->getQuads()) {
	// 		quad->codegenLabels(out);
	// 		out << "#" << quad->toString() << "\n";
	// 		quad->codegenX64(out);
	// 	}
	// 	out << "\n";
	// }
	if (getName().compare("main") == 0) {
		out << "callq init\n\n";
	}

	enter->codegenX64(out);
	out << "#Fn body " << myName << "\n";
	for (auto quad : *bodyQuads){
		quad->codegenLabels(out);
		out << "#" << quad->toString() << "\n";
		quad->codegenX64(out);
	}
	out << "#Fn epilogue " << myName << "\n";
	leave->codegenLabels(out);
	leave->codegenX64(out);
}

void Quad::codegenLabels(std::ostream& out){
	if (labels.empty()){ return; }

	size_t numLabels = labels.size();
	size_t labelIdx = 0;
	for ( Label * label : labels){
		if (label->getName().compare("fun_<init>") == 0) {
			out << "init: \n";
		}
		else {
			out << label->getName() << ": \n";
		}
		//if (labelIdx != numLabels - 1){ out << "\n"; }
		labelIdx++;
	}
}

void BinOpQuad::codegenX64(std::ostream& out){
	src1->genLoadVal(out, A);
	src2->genLoadVal(out, B);
	if (opr == ADD64) {
		out << "addq %rbx, %rax\n";
	}
	else if (opr == SUB64) {
		out << "subq %rbx, %rax\n";
	}
	else if (opr == MULT64) {
		out << "imulq %rbx\n";
	}
	else if (opr == DIV64) {
		out << "movq $0, %rdx\n";
		out << "idivq %rbx\n";
	}
	else if (opr == AND64) {
		out << "andq %rbx, %rax\n";
	}
	else if (opr == OR64) {
		out << "orq %rbx, %rax\n";
	}
	else if (opr == EQ64) {
		out << "cmpq %rbx, %rax\n";
		out << "movq $0, %rax\n";
		out << "sete %al\n";
	}
	else if (opr == NEQ64) {
		out << "cmpq %rbx, %rax\n";
		out << "movq $0, %rax\n";
		out << "setne %al\n";
	}
	else if (opr == GT64) {
		out << "cmpq %rbx, %rax\n";
		out << "movq $0, %rax\n";
		out << "setg %al\n";
	}
	else if (opr == GTE64) {
		out << "cmpq %rbx, %rax\n";
		out << "movq $0, %rax\n";
		out << "setge %al\n";
	}
	else if (opr == LT64) {
		out << "cmpq %rbx, %rax\n";
		out << "movq $0, %rax\n";
		out << "setl %al\n";
	}
	else if (opr == LTE64) {
		out << "cmpq %rbx, %rax\n";
		out << "movq $0, %rax\n";
		out << "setle %al\n";
	}
	dst->genStoreVal(out, A);
}

void UnaryOpQuad::codegenX64(std::ostream& out){
	src->genLoadVal(out, A);
	if (op == NEG64) {
		out << "negq %rax\n";
	}
	else if (op == NOT64) {
		out << "notq %rax\n";
	}
	dst->genStoreVal(out, A);
}

void AssignQuad::codegenX64(std::ostream& out){
	src->genLoadVal(out, A);
	dst->genStoreVal(out, A);
}

void ReadQuad::codegenX64(std::ostream& out){
	if (myDstType->isInt()) {
		out << "callq getInt\n";
	}
	else if (myDstType->isBool()) {
		out << "callq getBool\n";
	}
	myDst->genStoreVal(out, A);
}

void MagicQuad::codegenX64(std::ostream& out){
	out << "callq magic\n";
	myDst->genStoreVal(out, A);
}

void ExitQuad::codegenX64(std::ostream& out){
	out << "movq $0, %rdi\n";
	out << "movq $60, %rax\n";
	out << "syscall\n";
}

void WriteQuad::codegenX64(std::ostream& out){
	mySrc->genLoadVal(out, A);
	out << "movq %rax, %rdi\n";
	if (mySrcType->isString()) {
		out << "callq printString\n";
	}
	else if (mySrcType->isInt()) {
		out << "callq printInt\n";
	}
	else if (mySrcType->isBool()) {
		out << "callq printBool\n";
	}
}

void GotoQuad::codegenX64(std::ostream& out){
	out << "jmp " << tgt->getName() << "\n";
}

void IfzQuad::codegenX64(std::ostream& out){
	cnd->genLoadVal(out, A);
	out << "cmpq $1, %rax\n";
	out << "jne " << tgt->getName() << "\n";
}

void NopQuad::codegenX64(std::ostream& out){
	out << "nop" << "\n";
}

void CallQuad::codegenX64(std::ostream& out){
	string funName = "";
	if (sym->getName().compare("main") == 0) {
		funName = "main";
	}
	else {
		funName = "fun_" + sym->getName();
	}
	out << "callq " << funName << "\n";
	//horrible way of doing this, but only way I could think of decrementing stack of arguments
	for (auto proc : *myProc->getProg()->getProcs()) {
		if (proc->getName().compare(sym->getName()) == 0) {
			out << "addq $" << proc->getFormals().size()*8 << ", %rsp\n";
		}
	}
}

void EnterQuad::codegenX64(std::ostream& out){
	out << "pushq %rbp\n";
	out << "movq %rsp, %rbp\n";
	out << "addq $16, %rbp\n";

	out << "subq $" << myProc->arSize() << ", %rsp\n";
}

void LeaveQuad::codegenX64(std::ostream& out){
	out << "addq $" << myProc->arSize() << ", %rsp\n";
	out << "popq %rbp\n";
	out << "retq\n\n";
}

void SetArgQuad::codegenX64(std::ostream& out){
	opd->genLoadVal(out, A);
	// if (index == 1) {
	// 	out << "movq %rax, %rdi\n";
	// }
	// else if (index == 2) {
	// 	out << "movq %rax, %rsi\n";
	// }
	// else if (index == 3) {
	// 	out << "movq %rax, %rdx\n";
	// }
	// else if (index == 4) {
	// 	out << "movq %rax, %rcx\n";
	// }
	// else if (index == 5) {
	// 	out << "movq %rax, %r8\n";
	// }
	// else if (index == 6) {
	// 	out << "movq %rax, %r9\n";
	// }
	// else {
	// 	out << "subq 8, %rsp\n";
	// 	out << "movq %rax, %rsp\n";
	// }
	// out << "subq 8, %rsp\n";
	// out << "movq %rax, (%rsp)\n";
	out << "pushq %rax\n";
}

void GetArgQuad::codegenX64(std::ostream& out){
	// if (index == 1) {
	// 	opd->setMemoryLoc()
	// }
	// else if (index == 2) {
	// 	out << "movq %rax, %rsi\n";
	// }
	// else if (index == 3) {
	// 	out << "movq %rax, %rdx\n";
	// }
	// else if (index == 4) {
	// 	out << "movq %rax, %rcx\n";
	// }
	// else if (index == 5) {
	// 	out << "movq %rax, %r8\n";
	// }
	// else if (index == 6) {
	// 	out << "movq %rax, %r9\n";
	// }
	// else {
	// 	out << "subq 8, %rsp\n";
	// 	out << "movq %rax, %rsp\n";
	// }
	return;
}

void SetRetQuad::codegenX64(std::ostream& out){
	opd->genLoadVal(out, A);
}

void GetRetQuad::codegenX64(std::ostream& out){
	opd->genStoreVal(out, A);
}

void LocQuad::codegenX64(std::ostream& out){
	TODO(Implement me)
}

void SymOpd::genLoadVal(std::ostream& out, Register reg){
	out << getMovOp() << " " << getMemoryLoc() << ", " << getReg(reg) << "\n";
}

void SymOpd::genStoreVal(std::ostream& out, Register reg){
	out << "movq " << getReg(reg) << ", " << getMemoryLoc() << "\n";
}

void SymOpd::genLoadAddr(std::ostream& out, Register reg) {
	TODO(Implement me if necessary)
}

void AuxOpd::genLoadVal(std::ostream& out, Register reg){
	out << getMovOp() << " " << getMemoryLoc() << ", " << getReg(reg) << "\n";
}

void AuxOpd::genStoreVal(std::ostream& out, Register reg){
	out << "movq " << getReg(reg) << ", " << getMemoryLoc() << "\n";
}
void AuxOpd::genLoadAddr(std::ostream& out, Register reg){
	TODO(Implement me)
}


void AddrOpd::genStoreVal(std::ostream& out, Register reg){
	TODO(Implement me)
}

void AddrOpd::genLoadVal(std::ostream& out, Register reg){
	TODO(Implement me)
}

void AddrOpd::genStoreAddr(std::ostream& out, Register reg){
	TODO(Implement me)
}

void AddrOpd::genLoadAddr(std::ostream & out, Register reg){
	TODO(Implement me)
}

void LitOpd::genLoadVal(std::ostream & out, Register reg){
	out << getMovOp() << " $" << val << ", " << getReg(reg) << "\n";
}

}
