#include <llvm/IR/Function.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/CFG.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>

#include <iostream>
#include <memory>

using namespace llvm;

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <LLVM IR file>" << std::endl;
        return 1;
    }

    LLVMContext Context;
    SMDiagnostic Err;

    std::unique_ptr<Module> Mod = parseIRFile(argv[1], Err, Context);
    if (!Mod) {
        Err.print(argv[0], errs());
        return 1;
    }

    for (Function &F : *Mod) {
        std::cout << "Function: " << F.getName().str() << std::endl;
    }

    return 0;
}
