/*
 * @Author       : coldcat(by.gdc@outlook.com)
 * @Version      : V1.0
 * @Date         : 2024-01-27 22:26:28
 * @Description  : this file is about the class steiner tree
 * @TODO         : 
 */
#ifndef ST_TREE_CLASS
#define ST_TREE_CLASS
#include "Config.hpp"

namespace lq
{
    class Sttree {
        private:
        label data;
        std::set<Sttree*> children;

        public:
        Sttree(label value) : data(value) {}
        label inline getData(){
            return this->data;
        }
        std::set<Sttree*> inline getChildren(){
            return this->children;
        }

        void inline exportToDot(std::ofstream& outfile) {
            for (auto child : this->children) {
                outfile << "    " << data << " -- " << child->data << ";\n";
                child->exportToDot(outfile);
            }
        }

        void exportTreeToDot(const std::string& filename) {
            std::ofstream outfile(filename + ".dot");
            outfile << "graph G {\n";
            this->exportToDot(outfile);

            outfile << "}\n";
            outfile.close();

            std::string command = "dot -Tpng " + filename + ".dot -o " + filename + ".png";
            system(command.c_str());
        }

        void inline insertChild(label value){
            Sttree* newNode = new Sttree(value);
            this->children.insert(newNode);
        }
        void inline insertChild(Sttree* child){
            this->children.insert(child);
        }
        void traverse(int depth = 0) {
            std::cout << std::string(depth, ' ') << this->data << std::endl;
            for (auto child : this->children) {
                child->traverse(depth + 1);
            }
        }
        bool inline isChild(Sttree* child){
            return this->children.find(child) != this->children.end();
        }
        Sttree* findNode(const label& data) {
            if (this->data == data) return this;

            for (auto child : this->children) {
                Sttree* result = child->findNode(data);
                if (result != nullptr) return result;
            }

            return nullptr;
        }
    };

    void insertChild(Sttree* parent, label value) {
        parent->insertChild(value);
    }
    void insertChild(Sttree* parent, Sttree* child) {
        parent->insertChild(child);
    }

} // namespace lq

#endif