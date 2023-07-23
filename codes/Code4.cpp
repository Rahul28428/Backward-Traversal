#include <iostream>
#include <string>
#include <queue>
#include <unordered_map>
#include <vector>
#include <fstream>
using namespace std;

struct TreeNode
{
    string value;
    vector<TreeNode *> children;

    TreeNode(const string &val) : value(val) {}
};

unordered_map<string, TreeNode *> nodeMap;

void constructTree(const string &input)
{
    size_t colonPos = input.find(':');
    string parent = input.substr(0, colonPos);
    string childrenStr = input.substr(colonPos + 1); // Skip colon

    // Trim leading and trailing whitespace from parent and children strings
    parent = parent.substr(parent.find_first_not_of(" \t\r\n"), parent.find_last_not_of(" \t\r\n") + 1);
    childrenStr = childrenStr.substr(childrenStr.find_first_not_of(" \t\r\n"), childrenStr.find_last_not_of(" \t\r\n") + 1);

    TreeNode *parentNode;
    unordered_map<string, TreeNode *>::iterator it = nodeMap.find(parent);
    if (it != nodeMap.end())
    {
        parentNode = it->second;
    }
    else
    {
        parentNode = new TreeNode(parent);
        nodeMap[parent] = parentNode;
    }

    size_t start = 0;
    size_t commaPos = childrenStr.find(',', start);
    while (commaPos != string::npos)
    {
        string child = childrenStr.substr(start, commaPos - start);

        // Trim leading and trailing whitespace from child string
        child = child.substr(child.find_first_not_of(" \t\r\n"), child.find_last_not_of(" \t\r\n") + 1);

        TreeNode *childNode;
        it = nodeMap.find(child);
        if (it != nodeMap.end())
        {
            childNode = it->second;
        }
        else
        {
            childNode = new TreeNode(child);
            nodeMap[child] = childNode;
        }

        parentNode->children.push_back(childNode);

        start = commaPos + 1;
        commaPos = childrenStr.find(',', start);
    }

    // Process the last child
    string child = childrenStr.substr(start);

    // Trim leading and trailing whitespace from child string
    child = child.substr(child.find_first_not_of(" \t\r\n"), child.find_last_not_of(" \t\r\n") + 1);

    TreeNode *childNode;
    it = nodeMap.find(child);
    if (it != nodeMap.end())
    {
        childNode = it->second;
    }
    else
    {
        childNode = new TreeNode(child);
        nodeMap[child] = childNode;
    }

    parentNode->children.push_back(childNode);
}

void printTree(const TreeNode *node, const string &prefix = "")
{
    if (!node)
        return;

    cout << prefix << node->value;
    if (!node->children.empty())
    {
        cout << " : ";
        for (size_t i = 0; i < node->children.size(); i++)
        {
            cout << node->children[i]->value;
            if (i != node->children.size() - 1)
                cout << ", ";
        }
        cout << endl;

        for (size_t i = 0; i < node->children.size(); i++)
        {
            printTree(node->children[i], prefix + "    ");
        }
    }
    else
    {
        cout << ":" << endl;
    }
}

bool printPathToTop(const TreeNode *node, const string &targetNode, vector<string> &path)
{
    if (!node)
        return false;

    if (node->value == targetNode)
    {
        path.push_back(node->value);
        return true;
    }

    for (size_t i = 0; i < node->children.size(); i++)
    {
        if (printPathToTop(node->children[i], targetNode, path))
        {
            path.push_back(node->value);
            return true;
        }
    }

    return false;
}

bool isRootSignal(const TreeNode *node)
{
    for (unordered_map<string, TreeNode *>::const_iterator it = nodeMap.begin(); it != nodeMap.end(); ++it)
    {
        if (it->first != node->value)
        {
            const vector<TreeNode *> &children = it->second->children;
            for (size_t i = 0; i < children.size(); ++i)
            {
                if (children[i] == node)
                    return false;
            }
        }
    }
    return true;
}

int main()
{
    ifstream fileIn("input.txt");
    if (!fileIn.is_open())
    {
        cout << "Failed to open file!\n";
        return 0;
    }

    string line;
    vector<string> inputLines;

    while (getline(fileIn, line))
    {
        inputLines.push_back(line);
    }

    for (size_t i = 0; i < inputLines.size(); ++i)
    {
        constructTree(inputLines[i]);
    }

    TreeNode *root = nullptr;
    for (unordered_map<string, TreeNode *>::const_iterator it = nodeMap.begin(); it != nodeMap.end(); ++it)
    {
        if (isRootSignal(it->second))
        {
            root = it->second;
            break;
        }
    }

    if (root)
    {
        cout << "Tree Structure:" << endl;
        printTree(root);

        string targetNode;
        cout << "Enter the signal name: ";
        cin >> targetNode;

        unordered_map<string, TreeNode *>::iterator it = nodeMap.find(targetNode);
        if (it != nodeMap.end())
        {
            cout << "Path to top: ";
            vector<string> path;
            if (printPathToTop(root, targetNode, path))
            {
                for (int i = static_cast<int>(path.size()) - 1; i >= 0; i--)
                {
                    cout << path[i];
                    if (i != 0)
                        cout << " -> ";
                }
                cout << endl;
            }
            else
            {
                cout << "Signal not found!" << endl;
            }
        }
        else
        {
            cout << "Signal not found!" << endl;
        }
    }
    else
    {
        cout << "Root signal not found!" << endl;
    }

    // Clean up memory
    for (unordered_map<string, TreeNode *>::const_iterator it = nodeMap.begin(); it != nodeMap.end(); ++it)
    {
        delete it->second;
    }

    return 0;
}
