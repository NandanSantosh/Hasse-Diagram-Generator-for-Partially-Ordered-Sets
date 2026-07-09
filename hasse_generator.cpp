#include <bits/stdc++.h>
using namespace std;
using ll = long long;

class combination {
public:
    ll factorial(int n) {
        ll fact = 1;
        for (int i = 1; i <= n; ++i) {
            fact *= i;
        }
        return fact;
    }

    ll choose(int n, int r) {
        if (r < 0 || r > n) return 0;
        if (r == 0 || r == n) return 1;
        return factorial(n) / (factorial(r) * factorial(n - r));
    }
};

class Node {
public:
    vector<int> elements;
    vector<Node*> children;
    double probability = 0;

    void print() {
        cout << "{";
        for (size_t i = 0; i < elements.size(); ++i) {
            cout << elements[i];
            if (i < elements.size() - 1) {
                cout << ",";
            }
        }
        cout << "}";
    }
};

class hasse {
public:
    vector<Node*> allNodes;
    combination comb;

    bool is_subset(const vector<int>& parent, const vector<int>& child) {
        if (parent.size() != child.size() + 1) {
            return false;
        }
        size_t childIdx = 0;
        for (size_t parentIdx = 0; parentIdx < parent.size(); ++parentIdx) {
            if (childIdx < child.size() && parent[parentIdx] == child[childIdx]) {
                childIdx++;
            }
        }
        return childIdx == child.size();
    }

    void generate_subsets(const vector<int>& baseSet, int k, int start, vector<int>& current, vector<vector<int>>& result) {
        if (current.size() == k) {
            result.push_back(current);
            return;
        }
        for (size_t i = start; i < baseSet.size(); ++i) {
            current.push_back(baseSet[i]);
            generate_subsets(baseSet, k, i + 1, current, result);
            current.pop_back();
        }
    }

    Node* build(vector<int> rootSet) {
        sort(rootSet.begin(), rootSet.end());
        int n = rootSet.size();
        vector<vector<Node*>> layers(n + 1);
        for (int k = n; k >= 0; k--) {
            vector<vector<int>> subsets;
            vector<int> current;
            generate_subsets(rootSet, k, 0, current, subsets);
            for (const auto& sub : subsets) {
                Node* node = new Node();
                node->elements = sub;
                node->probability = (double)sub.size() / n; 
                layers[k].push_back(node);
                allNodes.push_back(node);
            }
        }

        for (int k = n; k > 0; k--) {
            for (Node* parentNode : layers[k]) {
                for (Node* childNode : layers[k - 1]) {
                    if (is_subset(parentNode->elements, childNode->elements)) {
                        parentNode->children.push_back(childNode);
                    }
                }
            }
        }
        return (layers[n].empty())?NULL:layers[n][0];
    }

    void printDiagramTree(Node* node, int depth = 0) {
        if (node==NULL) {
            return;
        }
        for (int i = 0; i < depth; ++i) {
            cout << "  ";
        }
        node->print();
        int k = node->elements.size();
        ll expectedChildren = comb.choose(k, k - 1);
        cout << " (Prob: " << node->probability << ") [Children: " << expectedChildren << "]\n";
        for (Node* child : node->children) {
            printDiagramTree(child, depth + 1);
        }
    }

    // void clearMemory() {
    //     for (Node* node : allNodes) {
    //         delete node;
    //     }
    //     allNodes.clear();
    // }
};

int main() {
    vector<int> set;
    // set={1,2,3,4};
    set={1,2,3,4,5};
    hasse g;
    Node* root = g.build(set);
    g.printDiagramTree(root);
    //g.clearMemory();
    return 0;
}