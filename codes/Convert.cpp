#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <cctype>
using namespace std;

// Split a string based on a delimiter
vector<string> splitString(const string& str, const string& delimiter) {
    vector<string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + delimiter.length();
        end = str.find(delimiter, start);
    }

    tokens.push_back(str.substr(start, end));
    return tokens;
}

// Trim leading and trailing whitespaces from a string
string trimString(const string& str) {
    size_t start = 0;
    size_t end = str.length();

    while (start < end && isspace(str[start])) {
        ++start;
    }

    while (end > start && isspace(str[end - 1])) {
        --end;
    }

    return str.substr(start, end - start);
}

int main() {
    // Open input_pre.txt file for reading
    ifstream inputFile("input_pre.txt", ios::binary);
    if (!inputFile) {
        cerr << "Failed to open input_pre.txt file." << endl;
        return 1;
    }

    // Detect and skip the BOM if present
    char bom[3] = {0};
    inputFile.read(bom, 3);
    if (bom[0] != '\xEF' || bom[1] != '\xBB' || bom[2] != '\xBF') {
        inputFile.seekg(0);
    }

    // Create a map to store the relationships
    map<string, vector<string> > relationships;

    string line;
    while (getline(inputFile, line)) {
        line = trimString(line);
        vector<string> tokens = splitString(line, "<=");
        if (tokens.size() != 2) {
            cerr << "Invalid input format: " << line << endl;
            continue;
        }

        string parent = trimString(tokens[0]);
        vector<string> children = splitString(tokens[1], "&&");
        for (size_t i = 0; i < children.size(); ++i) {
            children[i] = trimString(children[i]);
        }
        relationships[parent] = children;
    }

    // Close the input file
    inputFile.close();

    // Open input_post.txt file for writing
    ofstream outputFile("input_post.txt");
    if (!outputFile) {
        cerr << "Failed to open input_post.txt file." << endl;
        return 1;
    }

    // Write the relationships to the output file
    for (map<string, vector<string> >::const_iterator it = relationships.begin(); it != relationships.end(); ++it) {
        outputFile << it->first << ":";
        const vector<string>& children = it->second;
        vector<string> sortedChildren = children;
        sort(sortedChildren.begin(), sortedChildren.end());  // Sort the children alphabetically
        for (size_t i = 0; i < sortedChildren.size(); ++i) {
            outputFile << sortedChildren[i];
            if (i != sortedChildren.size() - 1) {
                outputFile << ",";
            }
        }
        outputFile << endl;
    }

    // Close the output file
    outputFile.close();

    cout << "Output written to input_post.txt" << endl;

    return 0;
}
