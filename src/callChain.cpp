#include "callChain.hpp"

#include <iostream>

CallChain::CallChain(llvm::Module *mod) {
    llvm::CallGraph cg(*mod);

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
}

void CallChain::FindLongestCallChain() {
    std::set<std::string> visited;
    int longestPath = dfs("main", callGraph, visited) - 1;

    std::cout << "Longest execution path length in main: " << longestPath << std::endl;
}

int CallChain::dfs(std::string node, std::unordered_map<std::string, std::vector<std::string>> callGraph, std::set<std::string> &visited) {
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
