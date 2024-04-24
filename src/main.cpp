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

#include <iostream>
#include <vector>
#include <map>

struct LongestPathPass : public llvm::PassInfoMixin<LongestPathPass> {
    llvm::PreservedAnalyses run(llvm::Module &mod, llvm::ModuleAnalysisManager &) {
        llvm::Function *mainFunc = mod.getFunction("main");
        if (mainFunc) {
            std::map<const llvm::BasicBlock*, int> longestPaths;
            // Initialize all basic blocks with minimal path length
            for (const llvm::BasicBlock &BB : *mainFunc) {
                longestPaths[&BB] = 0;
            }

            // Simple topological sort based longest path finding in a DAG
            std::vector<const llvm::BasicBlock*> topoSortedBlocks;
            topologicalSort(*mainFunc, topoSortedBlocks);

            for (const llvm::BasicBlock *BB : topoSortedBlocks) {
                int maxLength = 0;
                for (const auto *Pred : predecessors(BB)) {
                    maxLength = std::max(maxLength, longestPaths[Pred] + 1);
                }
                longestPaths[BB] = maxLength;
            }

            // Find the maximum path length in the function
            int maxPath = 0;
            for (auto &entry : longestPaths) {
                maxPath = std::max(maxPath, entry.second);
            }

            std::cout << "Longest path in main has length " << maxPath << std::endl;
        }

        return llvm::PreservedAnalyses::all();
    }

    void topologicalSort(const llvm::Function &F, std::vector<const llvm::BasicBlock*> &result) {
        std::map<const llvm::BasicBlock*, int> inDegree;
        for (const llvm::BasicBlock &BB : F) {
            for (const auto *Succ : successors(&BB)) {
                inDegree[Succ]++;
            }
        }

        std::vector<const llvm::BasicBlock*> queue;
        for (const llvm::BasicBlock &BB : F) {
            if (inDegree[&BB] == 0) {
                queue.push_back(&BB);
            }
        }

        while (!queue.empty()) {
            const llvm::BasicBlock *BB = queue.back();
            queue.pop_back();
            result.push_back(BB);
            for (const llvm::BasicBlock *Succ : successors(BB)) {
                if (--inDegree[Succ] == 0) {
                    queue.push_back(Succ);
                }
            }
        }
    }
};

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "Usage: " << argv[0] << " <LLVM IR file>" << std::endl;
        return 1;
    }

    llvm::LLVMContext context;
    llvm::SMDiagnostic err;

    std::unique_ptr<llvm::Module> mod = parseIRFile(argv[1], err, context);
    if (!mod) {
        err.print(argv[0], llvm::errs());
        return 1;
    }

    llvm::Function *mainFunc = mod->getFunction("main");
    if (!mainFunc) {
        std::cout << "Error: 'main' function not found in the IR file." << std::endl;
        return 1;
    }

    if (mainFunc->isDeclaration()) {
        std::cout << "Error: 'main' function is a declaration, not a definition." << std::endl;
        return 1;
    }

    llvm::PassBuilder pb;
    llvm::ModuleAnalysisManager mam;
    pb.registerModuleAnalyses(mam);

    llvm::ModulePassManager passManager;
    passManager.addPass(LongestPathPass());

    passManager.run(*mod, mam);

    return 0;
}
