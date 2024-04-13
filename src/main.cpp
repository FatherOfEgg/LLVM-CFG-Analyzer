#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/Analysis/CallGraph.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>

#include <iostream>
#include <vector>
#include <unordered_map>
#include <set>

int dfs(std::string node, std::unordered_map<std::string, std::vector<std::string>> callGraph, std::set<std::string> &visited) {
    if (visited.count(node)) {
        return 0;
    }
    
    visited.insert(node);
    int maxPathLength = 0;

    for (const auto &nextNode : callGraph[node]) {
        maxPathLength = std::max(maxPathLength, dfs(nextNode, callGraph, visited));
    }

    visited.erase(node);
    return maxPathLength + 1;
}

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

    llvm::CallGraph cg(*mod);
    std::unordered_map<std::string, std::vector<std::string>> callGraph;

    for (const auto &n : cg) {
        llvm::Function *f = n.second->getFunction();
        if (f && !f->isDeclaration()) {
            std::string caller = f->getName().str();

            for (const auto &callees : *n.second) {
                llvm::Function *callee = callees.second->getFunction();

                if (callee && !callee->isDeclaration()) {
                    std::string calleeName = callee->getName().str();
                    callGraph[caller].push_back(calleeName);
                }
            }
        }
    }

    std::set<std::string> visited;
    int longestPath = dfs("main", callGraph, visited) - 1;

    std::cout << "Longest execution path length in main: " << longestPath << std::endl;

    return 0;
}
