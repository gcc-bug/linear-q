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
    class Sttree: public std::enable_shared_from_this<Sttree>{
        private:
        label data;
        std::vector<std::shared_ptr<Sttree>> children;

        public:
        Sttree(label value) : data(value) {}
        label inline getData() const{
            return this->data;
        }
        std::vector<std::shared_ptr<Sttree>> inline getChildren() const{
            return children;
        }

        void inline exportToDot(std::ofstream& outfile) const{
            for (const auto& child : this->children) {
                outfile << "    " << data << " -- " << child->data << ";\n";
                child->exportToDot(outfile);
            }
        }

        void exportTreeToDot(const std::string& filename) const{
            std::ofstream outfile(filename + ".dot");
            outfile << "graph G {\n";
            this->exportToDot(outfile);

            outfile << "}\n";
            outfile.close();

            std::string command = "dot -Tpng " + filename + ".dot -o " + filename + ".png";
            system(command.c_str());
        }

        void inline insertChild(label value){
            this->children.push_back(std::make_shared<Sttree>(value));
        }
        void inline insertChild(std::shared_ptr<Sttree> child){
            this->children.push_back(child);
        }
        void traverse(int depth = 0) const {
            std::cout << std::string(depth, ' ') << data << std::endl;
            for (const auto& child : children) {
                child->traverse(depth + 1); // child is a shared_ptr, use -> to access members
            }
        }

        bool isChild(const std::shared_ptr<Sttree> child) const {
            // Since we can't directly compare shared_ptr with raw pointer, we compare pointed-to values
            for (const auto& uniqueChild : children) {
                if (uniqueChild == child) return true; // uniqueChild.get() returns raw pointer
            }
            return false;
        }

        std::shared_ptr<Sttree> findNode(const label& data) {
            if (this->data == data) return shared_from_this();

            for (const auto& child : children) {
                auto result = child->findNode(data);
                if (result) return result;
            }

            return nullptr; // Return nullptr if not found
        }
    };

    void insertChild(std::shared_ptr<Sttree> parent, label value) {
        parent->insertChild(value);
    }
    void insertChild(std::shared_ptr<Sttree> parent, std::shared_ptr<Sttree> child) {
        parent->insertChild(child);
    }

} // namespace lq

#endif