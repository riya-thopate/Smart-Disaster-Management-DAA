#include <iostream>
#include <queue>
#include <unordered_map>
#include <string>

using namespace std;

struct Node {
    char data;
    int frequency;

    Node* left;
    Node* right;

    Node(char data, int frequency) {
        this->data = data;
        this->frequency = frequency;
        left = nullptr;
        right = nullptr;
    }

    Node(int frequency, Node* left, Node* right) {
        data = '\0';
        this->frequency = frequency;
        this->left = left;
        this->right = right;
    }
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->frequency > b->frequency;
    }
};

void generateCodes(
    Node* root,
    string code,
    unordered_map<char, string>& codes
) {
    if (root == nullptr) {
        return;
    }

    // Leaf node
    if (root->left == nullptr &&
        root->right == nullptr) {

        codes[root->data] = code;
        return;
    }

    generateCodes(
        root->left,
        code + "0",
        codes
    );

    generateCodes(
        root->right,
        code + "1",
        codes
    );
}

void deleteTree(Node* root) {

    if (root == nullptr) {
        return;
    }

    deleteTree(root->left);
    deleteTree(root->right);

    delete root;
}

int main() {

    string report =
        "DISASTER";

    unordered_map<char, int> frequency;

    // Calculate character frequency
    for (char c : report) {
        frequency[c]++;
    }

    priority_queue<
        Node*,
        vector<Node*>,
        Compare
    > minHeap;

    // Create leaf nodes
    for (auto entry : frequency) {

        minHeap.push(
            new Node(
                entry.first,
                entry.second
            )
        );
    }

    // Build Huffman Tree
    while (minHeap.size() > 1) {

        Node* left =
            minHeap.top();
        minHeap.pop();

        Node* right =
            minHeap.top();
        minHeap.pop();

        Node* merged =
            new Node(
                left->frequency
                + right->frequency,
                left,
                right
            );

        minHeap.push(merged);
    }

    Node* root = minHeap.top();

    unordered_map<char, string> codes;

    generateCodes(
        root,
        "",
        codes
    );

    cout << "========================================" << endl;
    cout << "           HUFFMAN CODING" << endl;
    cout << "            GREEDY ALGORITHM" << endl;
    cout << "========================================" << endl;

    cout << "\nDisaster report text:" << endl;
    cout << report << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Character Frequencies:" << endl;

    for (auto entry : frequency) {

        cout << entry.first
             << " -> "
             << entry.second
             << endl;
    }

    cout << "\n----------------------------------------" << endl;

    cout << "Huffman Codes:" << endl;

    for (auto entry : codes) {

        cout << entry.first
             << " -> "
             << entry.second
             << endl;
    }

    string encodedText = "";

    for (char c : report) {
        encodedText += codes[c];
    }

    int originalBits =
        report.length() * 8;

    int compressedBits =
        encodedText.length();

    cout << "\n----------------------------------------" << endl;

    cout << "Original size = "
         << originalBits
         << " bits"
         << endl;

    cout << "Compressed size = "
         << compressedBits
         << " bits"
         << endl;

    cout << "Encoded report = "
         << encodedText
         << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Interpretation:" << endl;

    cout << "Huffman Coding assigns shorter binary"
         << endl;

    cout << "codes to frequently occurring characters"
         << endl;

    cout << "to reduce the storage required for reports."
         << endl;

    cout << "\n----------------------------------------" << endl;

    cout << "Huffman Coding completed successfully."
         << endl;

    deleteTree(root);

    return 0;
}