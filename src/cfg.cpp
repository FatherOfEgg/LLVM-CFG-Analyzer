#include "cfg.hpp"

#include <iostream>
#include "execPath.hpp"
#include "callChain.hpp"

void CFG::Open(char *IRFile) {
    mod = parseIRFile(IRFile, err, context);
    if (!mod) {
        err.print(IRFile, llvm::errs());
        exit(1);
    }

    llvm::Function *mainFunc = mod->getFunction("main");
    if (!mainFunc) {
        std::cout << "Error: 'main' function not found in the IR file." << std::endl;
        exit(1);
    }

    if (mainFunc->isDeclaration()) {
        std::cout << "Error: 'main' function is a declaration, not a definition." << std::endl;
        exit(1);
    }
}

void CFG::Analyze() {
    FindLongestExecutionPath();
    FindLongestFunctionCallChain();
}

void CFG::FindLongestExecutionPath() {
    llvm::PassBuilder pb;
    llvm::ModuleAnalysisManager mam;
    pb.registerModuleAnalyses(mam);

    llvm::ModulePassManager passManager;
    passManager.addPass(LongestPathPass());

    passManager.run(*mod, mam);
}

void CFG::FindLongestFunctionCallChain() {
    CallChain cc(&*mod);
    cc.FindLongestCallChain();
}
