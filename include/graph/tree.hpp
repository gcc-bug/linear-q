#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "../Config.hpp"

#ifndef ST_TREE_CLASS
#define ST_TREE_CLASS

namespace lq
{
    struct Sttree {
        label data;
        std::vector<Sttree*> children;

        Sttree(label value) : data(value) {}

        void exportToDot(std::ofstream& outfile) {
            for (auto child : children) {
                outfile << "    " << data << " -- " << child->data << ";\n";
                child->exportToDot(outfile);
            }
        }
    };

    void insertChild(Sttree* parent, label value) {
        Sttree* newNode = new Sttree(value);
        parent->children.push_back(newNode);
    }
    void insertChild(Sttree* parent, Sttree* child) {
        parent->children.push_back(child);
    }
    void traverse(Sttree* root, int depth = 0) {
        if (root == nullptr) return;
        std::cout << std::string(depth, ' ') << root->data << std::endl;
        for (auto child : root->children) {
            traverse(child, depth + 1);
        }
    }

    void exportTreeToDot(Sttree* root, const std::string& filename) {
        std::ofstream outfile(filename + ".dot");
        outfile << "graph G {\n";

        if (root != nullptr) {
            root->exportToDot(outfile);
        }

        outfile << "}\n";
        outfile.close();

        std::string command = "dot -Tpng " + filename + ".dot -o " + filename + ".png";
        system(command.c_str());
    }

    bool isChild(Sttree* A, Sttree* B){
        for(auto child: B->children){
            if(child->data == A->data) return true;
        }
        return false;
    }


} // namespace lq

#endif