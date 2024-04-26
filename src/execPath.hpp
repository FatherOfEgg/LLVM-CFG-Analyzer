#pragma once

#include <llvm/IR/Function.h>
#include <llvm/Pass.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Instructions.h>
#include <llvm/Analysis/CFG.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Passes/PassBuilder.h>

struct LongestPathPass : public llvm::PassInfoMixin<LongestPathPass> {
    llvm::PreservedAnalyses run(llvm::Module &mod, llvm::ModuleAnalysisManager &);

private:
    void topologicalSort(const llvm::Function &F, std::vector<const llvm::BasicBlock*> &result);
};
