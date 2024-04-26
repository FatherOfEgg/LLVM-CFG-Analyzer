#pragma once

#include <llvm/IR/Function.h>
#include <llvm/IR/Module.h>
#include <llvm/Analysis/CallGraph.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/Support/SourceMgr.h>

#include <unordered_map>
#include <vector>
#include <set>

class CallChain {
public:
    CallChain(llvm::Module *mod);

    void FindLongestCallChain();

private:
    int dfs(std::string node, std::unordered_map<std::string, std::vector<std::string>> callGraph, std::set<std::string> &visited);

    std::unordered_map<std::string, std::vector<std::string>> callGraph;
};
