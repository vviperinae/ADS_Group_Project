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
    CategoryNode** children;
    Playlist* playlist;
    int childCount;
    int childCapacity;

    CategoryNode(string categoryName) {
        name = categoryName;
        children = nullptr;
        playlist = new Playlist();
        childCount = 0;
        childCapacity = 0;
    }

    void addChild(CategoryNode* child) {
        // If we need more space, expand the array
        if (childCount >= childCapacity) {
            // Always make space for at least 5 more children
            int newCapacity = childCapacity + 5;
            CategoryNode** newChildren = new CategoryNode*[newCapacity];
            
            // Copy existing children to new array
            for (int i = 0; i < childCount; i++) {
                newChildren[i] = children[i];
            }
            
            // Clean up old array if it exists
            if (children != nullptr) {
                delete[] children;
            }
            
            children = newChildren;
            childCapacity = newCapacity;
        }
        
        // Add the new child
        children[childCount] = child;
        childCount++;
        cout << "✓ Added child category: " << child->name << " to " << name << endl;
    }

    CategoryNode* findChild(string childName) {
        for (int i = 0; i < childCount; i++) {
            if (children[i]->name == childName) {
                return children[i];
            }
        }
        return nullptr;
    }

    void displayCategory(int depth = 0) {
        // Add indentation based on depth
        for (int i = 0; i < depth; i++) cout << "  ";
        cout << "[" << name << "]";
        
        // Show song count if there are songs
        if (playlist->getSongCount() > 0) {
            cout << " (" << playlist->getSongCount() << " songs)";
        }
        cout << endl;
        
        // Display all children recursively
        for (int i = 0; i < childCount; i++) {
            children[i]->displayCategory(depth + 1);
        }
    }

    // Simple cleanup method
    void cleanup() {
        cout << "Cleaning up category: " << name << endl;
        
        // First cleanup all children
        for (int i = 0; i < childCount; i++) {
            if (children[i] != nullptr) {
                children[i]->cleanup();
                delete children[i];
                children[i] = nullptr;
            }
        }
        
        // Then cleanup the children array
        if (children != nullptr) {
            delete[] children;
            children = nullptr;
        }
        
        // Finally cleanup the playlist
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

    // Add category path like "Rock/Queen/Greatest Hits"
    void addCategory(string path) {
        CategoryNode* current = root;
        string currentPart = "";
        
        cout << "Adding category path: " << path << endl;
        
        for (int i = 0; i <= path.length(); i++) {
            if (i == path.length() || path[i] == '/') {
                if (!currentPart.empty()) {
                    cout << "  Processing part: " << currentPart << endl;
                    
                    CategoryNode* existing = current->findChild(currentPart);
                    if (existing == nullptr) {
                        // Create new category
                        CategoryNode* newCategory = new CategoryNode(currentPart);
                        current->addChild(newCategory);
                        current = newCategory;
                        cout << "  Created new category: " << currentPart << endl;
                    } else {
                        // Use existing category
                        current = existing;
                        cout << "  Using existing category: " << currentPart << endl;
                    }
                    currentPart = "";
                }
            } else {
                currentPart += path[i];
            }
        }
        cout << "✓ Finished creating category: " << path << endl;
    }

    // Find category by path like "Rock/Queen"
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
        return current;
    }

    // Search for song in entire library
    Song* searchSongInLibrary(string title) {
        cout << "Searching for song: " << title << endl;
        return searchSongInNode(root, title);
    }

private:
    Song* searchSongInNode(CategoryNode* node, string title) {
        // Check current node's playlist first
        Song* found = node->playlist->searchSong(title);
        if (found != nullptr) {
            cout << "✓ Found song in category: " << node->name << endl;
            return found;
        }
        
        // If not found, search in all child categories
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

    // Helper method to count all categories
    int countCategories(CategoryNode* node) {
        int count = 1; // Count this node
        for (int i = 0; i < node->childCount; i++) {
            count += countCategories(node->children[i]);
        }
        return count;
    }

    // Helper method to count all songs
    int countSongs(CategoryNode* node) {
        int count = node->playlist->getSongCount();
        for (int i = 0; i < node->childCount; i++) {
            count += countSongs(node->children[i]);
        }
        return count;
    }

    void cleanup() {
        if (root != nullptr) {
            root->cleanup();
            delete root;
            root = nullptr;
            cout << "✓ Music Library cleaned up" << endl;
        }
    }

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
