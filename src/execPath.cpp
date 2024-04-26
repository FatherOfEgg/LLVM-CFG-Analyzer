#include "execPath.hpp"

#include <iostream>
#include <vector>
#include <map>

llvm::PreservedAnalyses LongestPathPass::run(llvm::Module &mod, llvm::ModuleAnalysisManager &) {
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

void LongestPathPass::topologicalSort(const llvm::Function &F, std::vector<const llvm::BasicBlock*> &result) {
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
