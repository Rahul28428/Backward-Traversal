#include <iostream>
#include <string>
#include <queue>
#include <map>
#include <vector>

using namespace std;

struct TreeNode
{
    string value;
    vector<TreeNode *> children;

    TreeNode(const string &val) : value(val) {}
};

map<string, TreeNode *> nodeMap;

void constructTree(const string &input)
{
    size_t colonPos = input.find(':');
    string parent = input.substr(0, colonPos);
    string childrenStr = input.substr(colonPos + 1); // Skip colon

    TreeNode *parentNode;
    if (nodeMap.find(parent) != nodeMap.end())
    {
        parentNode = nodeMap[parent];
    }
    else
    {
        parentNode = new TreeNode(parent);
        nodeMap[parent] = parentNode;
    }

    size_t commaPos = 0;
    while (commaPos != string::npos)
    {
        size_t nextCommaPos = childrenStr.find(',', commaPos);
        string child;
        if (nextCommaPos != string::npos)
        {
            child = childrenStr.substr(commaPos, nextCommaPos - commaPos);
            commaPos = nextCommaPos + 1; // Skip comma
        }
        else
        {
            child = childrenStr.substr(commaPos);
            commaPos = nextCommaPos;
        }

        TreeNode *childNode;
        if (nodeMap.find(child) != nodeMap.end())
        {
            childNode = nodeMap[child];
        }
        else
        {
            childNode = new TreeNode(child);
            nodeMap[child] = childNode;
        }

        parentNode->children.push_back(childNode);
    }
}

void printTree(TreeNode *node, const string &prefix = "")
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
            printTree(node->children[i], "");
        }
    }
    else
    {
        cout << ":" << endl;
    }
}

bool printPathToTop(TreeNode *node, const string &targetNode)
{
    if (!node)
        return false;

    if (node->value == targetNode)
    {
        cout << node->value;
        return true;
    }

    for (size_t i = 0; i < node->children.size(); i++)
    {
        if (printPathToTop(node->children[i], targetNode))
        {
            cout << " -> " << node->value;
            return true;
        }
    }

    return false;
}

int main()
{
    vector<string> inputLines;
    inputLines.push_back("one:two,three,four");
    inputLines.push_back("two:five");
    inputLines.push_back("three:six,seven");
    inputLines.push_back("four:eight,nine,ten");

    for (size_t i = 0; i < inputLines.size(); ++i)
    {
        constructTree(inputLines[i]);
    }

    TreeNode *root = nullptr;
    for (map<string, TreeNode *>::const_iterator it = nodeMap.begin(); it != nodeMap.end(); ++it)
    {
        if (it->second->value == "one")
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

        if (nodeMap.find(targetNode) != nodeMap.end())
        {
            cout << "Path to top: ";
            if (!printPathToTop(root, targetNode))
                cout << "Signal not found!" << endl;
            else
                cout << endl;
        }
        else
        {
            cout << "Signal not found!" << endl;
        }
    }
    else
    {
        cout << "Root signal 'one' not found!" << endl;
    }

    // Clean up memory
    for (map<string, TreeNode *>::const_iterator it = nodeMap.begin(); it != nodeMap.end(); ++it)
    {
        delete it->second;
    }

    return 0;
}
