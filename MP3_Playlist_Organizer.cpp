#include <iostream>
#include <string>

using namespace std;
class Song {

};

class Playlist {

};

// =============================================
// Tree Structure for Category Organization
// =============================================
class CategoryNode {
public:
    string name;
    CategoryNode** children; //dynamic array of pointers to child categories
    Playlist* playlist; //playlist containing songs for this category
    int childCount; //current num of child categories
    int childCapacity; //max capacity of children array

    CategoryNode(string categoryName) { //create a new category with given name
        name = categoryName;
        children = nullptr;
        playlist = new Playlist();
        childCount = 0;
        childCapacity = 0;
    }

    void addChild(CategoryNode* child) { //add a child category to this node
        if (childCount >= childCapacity) { //if we need more space, expand the array
            int newCapacity = childCapacity + 5;
            CategoryNode** newChildren = new CategoryNode*[newCapacity];
            
            //copy existing children to new array
            for (int i = 0; i < childCount; i++) {
                newChildren[i] = children[i];
            }
            
            if (children != nullptr) { //clean up old array if it exists
                delete[] children;
            }
            
            children = newChildren;
            childCapacity = newCapacity;
        }
        
        //add the new child to the end of the array
        children[childCount] = child;
        childCount++;
        cout << "✓ Added child category: " << child->name << " to " << name << endl;
    }

    CategoryNode* findChild(string childName) { //find a child category by name
        for (int i = 0; i < childCount; i++) {
            if (children[i]->name == childName) {
                return children[i];
            }
        }
        return nullptr;
    }

    void displayCategory(int depth = 0) {
        for (int i = 0; i < depth; i++){
            cout << "  ";
        }
        cout << "[" << name << "]";

        //show song count if there are songs
        if (playlist->getSongCount() > 0) {
            cout << " (" << playlist->getSongCount() << " songs)";
        }
        cout << endl;

        for (int i = 0; i < childCount; i++) {
            children[i]->displayCategory(depth + 1);
        }
    }

    //simple cleanup method
    void cleanup() {
        cout << "Cleaning up category: " << name << endl;
        
        //first cleanup all children
        for (int i = 0; i < childCount; i++) {
            if (children[i] != nullptr) {
                children[i]->cleanup();
                delete children[i];
                children[i] = nullptr;
            }
        }
        
        //then cleanup the children array
        if (children != nullptr) {
            delete[] children;
            children = nullptr;
        }
        
        //finally cleanup the playlist
        if (playlist != nullptr) {
            delete playlist;
            playlist = nullptr;
        }
        
        childCount = 0;
        childCapacity = 0;
    }
};

class MusicTree {
private:
    CategoryNode* root;

public:
    MusicTree() {
        root = new CategoryNode("Music Library");
        cout << "✓ Created Music Library" << endl;
    }

    //add category path like "Rock/Queen/Greatest Hits"
    //this creates the entire hierarchy if it doesn't exist
    void addCategory(string path) {
        CategoryNode* current = root;
        string currentPart = "";
        
        cout << "Adding category path: " << path << endl;
        
        for (int i = 0; i <= path.length(); i++) {
            if (i == path.length() || path[i] == '/') { //check if we reached end of string or /
                if (!currentPart.empty()) { //we have a complete category name to process
                    cout << "  Processing part: " << currentPart << endl; 
                    CategoryNode* existing = current->findChild(currentPart); //check if this category already exists
                    
                    if (existing == nullptr) { //create new category
                        CategoryNode* newCategory = new CategoryNode(currentPart);
                        current->addChild(newCategory);
                        current = newCategory;
                        cout << "  Created new category: " << currentPart << endl;
                    } else {
                        //use existing category
                        current = existing;
                        cout << "  Using existing category: " << currentPart << endl;
                    }
                    currentPart = "";
                }
            } else { //build the current category name character by character
                currentPart += path[i];
            }
        }
        cout << "✓ Finished creating category: " << path << endl;
    }

    //find category by path like "Rock/Queen"
    CategoryNode* findCategory(string path) {
        CategoryNode* current = root;
        string currentPart = "";
        
        for (int i = 0; i <= path.length(); i++) { 
            if (i == path.length() || path[i] == '/') {
                if (!currentPart.empty()) {
                    current = current->findChild(currentPart);
                    if (current == nullptr) {
                        cout << "✗ Category not found: " << currentPart << " in path " << path << endl;
                        return nullptr;
                    }
                    currentPart = "";
                }
            } else {
                currentPart += path[i];
            }
        }
        return current; //return the final category in path
    }

    //search for song by title throughout entire library
    Song* searchSongInLibrary(string title) {
        cout << "Searching for song: " << title << endl;
        return searchSongInNode(root, title); //start recursive search from root
    }

private: //recursive helper function to search for song in a node and its children
    Song* searchSongInNode(CategoryNode* node, string title) {

        Song* found = node->playlist->searchSong(title); //firstt check current node's playlist
        if (found != nullptr) {
            cout << "✓ Found song in category: " << node->name << endl;
            return found;
        }
        
        //if not found, search in all child categories
        for (int i = 0; i < node->childCount; i++) {
            found = searchSongInNode(node->children[i], title);
            if (found != nullptr) {
                return found;
            }
        }
        
        return nullptr;
    }

public:
    void displayLibrary() {
        cout << "\n=== MUSIC LIBRARY STRUCTURE ===" << endl;
        cout << "Total categories: " << countCategories(root) << endl;
        cout << "Total songs: " << countSongs(root) << endl;
        root->displayCategory();
        cout << "===============================" << endl;
    }

    // count all categories in the tree
    int countCategories(CategoryNode* node) {
        int count = 1; //count this node itself
        //add counts fromm all children recursively
        for (int i = 0; i < node->childCount; i++) {
            count += countCategories(node->children[i]);
        }
        return count;
    }

    //method to count all songs in the tree
    int countSongs(CategoryNode* node) {
        //start with songs in current category
        int count = node->playlist->getSongCount();
        for (int i = 0; i < node->childCount; i++) {
            count += countSongs(node->children[i]);
        }
        return count;
    }

    //clean up entire music tree
    void cleanup() {
        if (root != nullptr) {
            root->cleanup(); //root and everything under it
            delete root;
            root = nullptr;
            cout << "✓ Music Library cleaned up" << endl;
        }
    }

    //getter for root node for external access
    CategoryNode* getRoot() {
        return root;
    }
};

class MusicLibrary {

};

class PerformanceTest {

};

void displayMenu() {

}

int main() {

}
