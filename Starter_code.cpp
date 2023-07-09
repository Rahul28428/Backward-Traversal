#include <iostream>
#include <vector>
#include <queue>
#include<string>
#include "code.h"
using namespace std;

TreeNode<string>* takeInputLevelWise() {
    string rootData;
    cout << "Enter root data: ";
    cin >> rootData;
    TreeNode<string>* root = new TreeNode<string>(rootData);

    queue<TreeNode<string>*> pendingNodes;

    pendingNodes.push(root);
    while (!pendingNodes.empty()) {
        TreeNode<string>* front = pendingNodes.front();
        pendingNodes.pop();
        int numChild;
        cout << "Enter number of children of " << front->data << ": ";
        cin >> numChild;
        for (int i = 0; i < numChild; i++) {
            string childData;
            cout << "Enter " << i+1 << "th child of " << front->data << ": ";
            cin >> childData;
            TreeNode<string>* child = new TreeNode<string>(childData);
            front->children.push_back(child);
            pendingNodes.push(child);
        }
    }
    return root;
}

void printTree(TreeNode<string>* root) {
    if (root == NULL) {
        return;
    }

    cout << root->data << ":";
    for (int i = 0; i < root->children.size(); i++) {
        cout << root->children[i]->data << ",";
    }
    cout << endl;
    for (int i = 0; i < root->children.size(); i++) {
        printTree(root->children[i]);
    }
}

void printPathToTop(TreeNode<string>* node, vector<string>& path) {
    if (node == nullptr) {
        return;
    }

    path.push_back(node->data);  // Add the current node to the path

    // If the node is the top-most node, print the path
    if (node->children.empty()) {
        for (int i = path.size() - 1; i >= 0; i--) {
            cout << path[i];
            if (i != 0) {
                cout << " -> ";
            }
        }
        cout << endl;
    }

    for (int i = 0; i < node->children.size(); i++) {
        printPathToTop(node->children[i], path);  // Recursively print paths of all children
    }

    path.pop_back();  // Remove the current node from the path after visiting all its children
}


void printPath(TreeNode<string>* root) {
    if (root == nullptr) {
        return;
    }

    vector<string> path;
    printPathToTop(root, path);  // Print path from each leaf node to the top

    cout << root->data << ":";
    for (int i = 0; i < root->children.size(); i++) {
        cout << root->children[i]->data << ",";
    }
    cout << endl;

    for (int i = 0; i < root->children.size(); i++) {
        printTree(root->children[i]);  // Recursively print the tree
    }
}

bool findNodePath(TreeNode<string>* node, const string& target, vector<string>& path) {
    if (node == nullptr) {
        return false;
    }

    path.push_back(node->data);  // Add the current node to the path

    if (node->data == target) {
        return true;  // Node found
    }

    for (int i = 0; i < node->children.size(); i++) {
        if (findNodePath(node->children[i], target, path)) {
            return true;  // Node found in child subtree
        }
    }

    path.pop_back();  // Remove the current node from the path if the target was not found
    return false;  // Node not found
}

// one 3 two three four 1  five 2 six seven 3 eight nine ten 0 2 eleven twelve 0 0 2 thirteen fourteen 0 0  0 0 2 fifteen sixteen 0 0

int main() {
    TreeNode<string>* root = takeInputLevelWise();
    printTree(root);

    string targetNode;
    cout << "Enter the signal name to find its path: ";
    cin >> targetNode;

    vector<string> path;
    bool found = findNodePath(root, targetNode, path);

    if (found) {
        cout << "Path to " << targetNode << ": ";
        for (int i = path.size() - 1; i >= 0; i--) {
            cout << path[i];
            if (i != 0) {
                cout << " -> ";
            }
        }
        cout << endl;
    } else {
        cout << "Signal not found!" << endl;
    }

    return 0;
}

