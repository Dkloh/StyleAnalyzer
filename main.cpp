// Purpose: This program reads a file and analyzes the text for the number of words, sentences and style of writing

#include <iostream>
#include <fstream>
#include <string>
#include <map> 
using namespace std;

// Binary Search Tree Node
template <class T>
class BNode {
public:
    T info;
    int count;
    BNode* left;
    BNode* right;

    BNode(const T& data) : info(data), count(1), left(NULL), right(NULL) {}
};

// Binary Search Tree for storing unique words
template <class T>
class BST {
private:
    BNode<T>* root;

    // Insert a new node into the BST
    void insert(BNode<T>*& node, const T& data) {
        if (!node) {
            node = new BNode<T>(data);
        } else if (data < node->info) {
            insert(node->left, data);
        } else if (data > node->info) {
            insert(node->right, data);
        } else {
            node->count++;
        }
    }

    // Destroy the BST
    void destroy(BNode<T>*& node) {
        if (node) {
            destroy(node->left);
            destroy(node->right);
            delete node;
            node = NULL;
        }
    }

public:
    BST() : root(NULL) {}

    // Insert a new element into the BST
    void insert(const T& data) {
        insert(root, data);
    }

    // Destroy the BST
    void destroy() {
        destroy(root);
    }

    // Destructor
    ~BST() {
        destroy();
    }

    // Get the root node of the BST
    BNode<T>* getRoot() const {
        return root;
    }
};

// Function to check if a character is alphanumeric
bool isAlnum(char c) {
    return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

// Function to convert a character to lowercase
char toLower(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + ('a' - 'A');
    }
    return c;
}

// Class to handle text analysis and style warnings
class StyleAnalyzer {
private:
    int totalWords;
    int uniqueWords;
    int uniqueWordsMoreThanThreeLetters;
    int totalWordLength;
    int totalSentenceLength;
    int totalSentences;
    BST<string> wordTree;
    map<string, int> wordFrequency;

public:
    StyleAnalyzer() : totalWords(0), uniqueWords(0), uniqueWordsMoreThanThreeLetters(0),
                      totalWordLength(0), totalSentenceLength(0), totalSentences(0) {}

    // Function for analyzing the text and update statistics
    void analyzeText(ifstream& inFile) {
        string word;
        char c;
        int currentSentenceWords = 0;  

        while (inFile.get(c)) {
            if (isAlnum(c)) {
                word += toLower(c);
            } else if (!word.empty()) {
                processWord(word);
                word.clear();
                currentSentenceWords++;  
            }

            if (c == '.' || c == '!' || c == '?') {
                totalSentenceLength += currentSentenceWords; 
                totalSentences++;
                currentSentenceWords = 0;  
            }
        }
        
        if (!word.empty()) {
            processWord(word);
            currentSentenceWords++;  
        }
        
        if (currentSentenceWords > 0) {
            totalSentenceLength += currentSentenceWords;
            totalSentences++;
        }
    }

    // Function to process each word and update statistics
    void processWord(string& word) {
        int wordLength = word.length();
        totalWords++;

        if (wordLength > 3) {
            totalWordLength += wordLength;
            wordTree.insert(word);
            uniqueWords++;

            wordFrequency[word]++; 
            uniqueWordsMoreThanThreeLetters++;
        }
    }

    // Function to calculate and show statistics
    void calculateAndShowStatistics(const string& filename, ofstream& outFile) {
        outFile << "FILE NAME: " << filename << endl << endl;

        outFile << "STATISTICAL SUMMARY" << endl;
        outFile << "TOTAL NUMBER OF WORDS: " << totalWords << endl;
        outFile << "TOTAL NUMBER OF UNIQUE WORDS: " << uniqueWords << endl;
        outFile << "TOTAL NUMBER OF UNIQUE WORDS OF MORE THAN THREE LETTERS: " << uniqueWordsMoreThanThreeLetters << endl;
        outFile << "AVERAGE WORD LENGTH: " << totalWordLength / static_cast<double>(uniqueWords) << " characters" << endl;
        outFile << "AVERAGE SENTENCE LENGTH: " << totalSentenceLength / static_cast<double>(totalSentences) << " words" << endl;

        outFile << endl << "STYLE WARNINGS" << endl;

        // Check for style warnings and print them
        if ((totalSentenceLength / static_cast<double>(totalSentences)) > 10) {
            outFile << "WARNING: The average sentence length is greater than 10 words." << endl;
        }
        if ((totalWordLength / static_cast<double>(uniqueWords)) > 5) {
            outFile << "WARNING: The average word length is greater than 5 characters." << endl;
        }
        for (const auto& entry : wordFrequency) {
            if (entry.second > 0.05 * uniqueWordsMoreThanThreeLetters) {
                outFile << "WARNING: The word '" << entry.first << "' is used more than 5% of the time." << endl;
            }
        }

        outFile << endl << "INDEX OF UNIQUE WORDS" << endl;
        showUniqueWordsIndex(outFile);
    }

    // Function to show unique words index in alphabetical order
    void showUniqueWordsIndex(ofstream& outFile) {
        showInOrder(wordTree.getRoot(), outFile);
    }

    // Function to show words in alphabetical order
    void showInOrder(BNode<string>* node, ofstream& outFile) {
        if (node) {
            showInOrder(node->left, outFile);
            outFile << node->info << endl;
            showInOrder(node->right, outFile);
        }
    }
};

int main() {
    string filename;
    cout << "Please enter the name of the file with the text to analyze: " << endl;
    cin >> filename;
    cout << "Check your results in the analysis file";
    
    ifstream inFile(filename);
    if (inFile.is_open()) {
        ofstream outFile("analysis_result.txt");
        if (outFile.is_open()) {
            StyleAnalyzer analyzer;
            analyzer.analyzeText(inFile);
            analyzer.calculateAndShowStatistics(filename, outFile);
            outFile.close();
        } else {
            cout << "Error: Unable to create output file." << endl;
        }
        inFile.close();
    } else {
        cout << "Error: Unable to open file." << endl;
    }
    return 0;
}
