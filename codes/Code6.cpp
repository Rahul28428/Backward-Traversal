#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <unordered_map>

using namespace std;

struct Node {
    string parent;
    vector<string> children;
};

unordered_map<string, Node> nodeMap;

void constructTree(const string& input) {
    size_t colonPos = input.find(':');
    string parent = input.substr(0, colonPos);
    string childrenStr = input.substr(colonPos + 1); // Skip colon

    // Trim leading and trailing whitespace from parent and children strings
    size_t parentStart = parent.find_first_not_of(" \t\r\n");
    size_t parentEnd = parent.find_last_not_of(" \t\r\n");
    parent = parent.substr(parentStart, parentEnd - parentStart + 1);

    size_t childrenStart = childrenStr.find_first_not_of(" \t\r\n");
    size_t childrenEnd = childrenStr.find_last_not_of(" \t\r\n");
    childrenStr = childrenStr.substr(childrenStart, childrenEnd - childrenStart + 1);

    size_t start = 0;
    size_t commaPos = childrenStr.find(',', start);
    while (commaPos != string::npos) {
        string child = childrenStr.substr(start, commaPos - start);

        // Trim leading and trailing whitespace from child string
        size_t childStart = child.find_first_not_of(" \t\r\n");
        size_t childEnd = child.find_last_not_of(" \t\r\n");
        child = child.substr(childStart, childEnd - childStart + 1);

        nodeMap[parent].children.push_back(child);

        start = commaPos + 1;
        commaPos = childrenStr.find(',', start);
    }

    // Process the last child
    string child = childrenStr.substr(start);

    // Trim leading and trailing whitespace from child string
    size_t childStart = child.find_first_not_of(" \t\r\n");
    size_t childEnd = child.find_last_not_of(" \t\r\n");
    child = child.substr(childStart, childEnd - childStart + 1);

    nodeMap[parent].children.push_back(child);
}

void printTree(const string& node, const string& prefix = "") {
    cout << prefix << node;
    if (!nodeMap[node].children.empty()) {
        cout << " : ";
        for (size_t i = 0; i < nodeMap[node].children.size(); i++) {
            cout << nodeMap[node].children[i];
            if (i != nodeMap[node].children.size() - 1)
                cout << ", ";
        }
        cout << endl;

        for (size_t i = 0; i < nodeMap[node].children.size(); i++) {
            printTree(nodeMap[node].children[i], prefix + "    ");
        }
    }
    else {
        cout << ":" << endl;
    }
}

bool printPathToTop(const string& node, const string& targetNode, vector<string>& path) {
    if (node == targetNode) {
        path.push_back(node);
        return true;
    }

    for (size_t i = 0; i < nodeMap[node].children.size(); i++) {
        if (printPathToTop(nodeMap[node].children[i], targetNode, path)) {
            path.push_back(node);
            return true;
        }
    }

    return false;
}

bool isRootSignal(const string& node) {
    for (unordered_map<string, Node>::const_iterator it = nodeMap.begin(); it != nodeMap.end(); ++it) {
        if (it->first != node) {
            const vector<string>& children = it->second.children;
            if (find(children.begin(), children.end(), node) != children.end())
                return false;
        }
    }
    return true;
}

int main() {
    ifstream fileIn("input.txt");
    if (!fileIn.is_open()) {
        cout << "Failed to open file!\n";
        return 0;
    }

    string line;
    vector<string> inputLines;

    while (getline(fileIn, line)) {
        inputLines.push_back(line);
    }

    for (size_t i = 0; i < inputLines.size(); ++i) {
        constructTree(inputLines[i]);
    }

    string root;
    for (unordered_map<string, Node>::const_iterator it = nodeMap.begin(); it != nodeMap.end(); ++it) {
        if (isRootSignal(it->first)) {
            root = it->first;
            break;
        }
    }

    if (!root.empty()) {
        cout << "Tree Structure:" << endl;
        printTree(root);

        string targetNode;
        cout << "Enter the signal name: ";
        cin >> targetNode;

        if (nodeMap.find(targetNode) != nodeMap.end()) {
            cout << "Path to top: ";
            vector<string> path;
            if (printPathToTop(root, targetNode, path)) {
                for (int i = static_cast<int>(path.size()) - 1; i >= 0; i--) {
                    cout << path[i];
                    if (i != 0)
                        cout << " -> ";
                }
                cout << endl;
            }
            else {
                cout << "Signal not found!" << endl;
            }
        }
        else {
            cout << "Signal not found!" << endl;
        }
    }
    else {
        cout << "Root signal not found!" << endl;
    }

    return 0;
}
