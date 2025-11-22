#include <iostream>
#include <string>

using namespace std;
class Song {
        string title;
    string artist;
    string album;
    string duration;
    Song* prev;
    Song* next;

    Song(string t, string a, string al, string d) {
        title = t;
        artist = a;
        album = al;
        duration = d;
        prev = nullptr;
        next = nullptr;
    }

};

class Playlist {
    private:
    Song* head;
    Song* tail;
    Song* current;
    int songCount;

public:
    // Constructor initializes empty playlist
    Playlist() {
        head = nullptr;
        tail = nullptr;
        current = nullptr;
        songCount = 0;
    }

    // Add a new song to the end of the playlist
    void addSong(Song* newSong) {

        // If playlist is empty, new song becomes head, tail, and current
        if (head == nullptr) {
            head = newSong;
            tail = newSong;
            current = newSong;
        } 
        else {
            // Connect new song to old tail
            tail->next = newSong;
            newSong->prev = tail;

            // Move tail pointer to new song
            tail = newSong;
        }

        songCount++;

        cout << newSong->title << "' added to playlist." << endl;
    }

    // Remove song by title
    void removeSong(string title) {
        Song* temp = head;

        while (temp != nullptr) {

            // Found matching song
            if (temp->title == title) {

                // Case 1: Song is not the first song
                if (temp->prev != nullptr) {
                    temp->prev->next = temp->next;
                } else {
                    head = temp->next; // Removing head
                }

                // Case 2: Song is not the last song
                if (temp->next != nullptr) {
                    temp->next->prev = temp->prev;
                } else {
                    tail = temp->prev; // Removing tail
                }

                // If removed song was playing, move to next or previous
                if (current == temp) {
                    if (temp->next != nullptr) {
                        current = temp->next;
                    } else {
                        current = temp->prev;
                    }
                }

                delete temp;
                songCount--;

                cout << title << "' removed from playlist." << endl;
                return;
            }

            temp = temp->next;
        }

        cout << "Song not found: " << title << endl;
    }

    // Move to next song
    Song* playNext() {

        if (current == nullptr) {
            cout << "No song to play." << endl;
            return nullptr;
        }

        if (current->next == nullptr) {
            cout << "Already at the last song." << endl;
            return current;
        }

        current = current->next;
        return current;
    }

    // Move to previous song
    Song* playPrevious() {

        if (current == nullptr) {
            cout << "No song to play." << endl;
            return nullptr;
        }

        if (current->prev == nullptr) {
            cout << "Already at the first song." << endl;
            return current;
        }

        current = current->prev;
        return current;
    }

    // Search for song by title
    Song* searchSong(string title) {
        Song* temp = head;

        while (temp != nullptr) {
            if (temp->title == title) {
                return temp;
            }
            temp = temp->next;
        }

        return nullptr;
    }

    // Display playlist contents
    void displayPlaylist() {

        if (songCount == 0) {
            cout << "Playlist is empty!" << endl;
            return;
        }

        Song* temp = head;
        int num = 1;

        cout << "\n==== Playlist (" << songCount << " songs) ====\n";

        while (temp != nullptr) {
            cout << num << ". " << temp->title << " - " << temp->artist << " (" << temp->duration << ")";

            if (temp == current) {
                cout << "  <-- NOW PLAYING";
            }

            cout << endl;

            temp = temp->next;
            num++;
        }

        cout << "==============================\n";
    }

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

→            root->cleanup(); //root and everything under it
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
public:
    static void demonstrateArrayVsLinkedList() {
        cout << "\n=== PERFORMANCE DEMONSTRATION ===" << endl;
        
        // Test array approach (simulated)
        cout << "\n--- ARRAY APPROACH (Baseline) ---" << endl;
        cout << "Playlist as Array: [Song1, Song2, Song3, Song4, Song5]" << endl;
        cout << "Removing 'Song3' from middle:" << endl;
        cout << "1. Find Song3 (O(n))" << endl;
        cout << "2. Shift Song4 and Song5 left (O(n))" << endl;
        cout << "Total: O(n) - gets slower with more songs" << endl;
        
        // Test linked list approach
        cout << "\n--- LINKED LIST APPROACH (Optimized) ---" << endl;
        Playlist testPlaylist;
        
        // Add songs
        for (int i = 1; i <= 5; i++) {
            Song* newSong = new Song("Song " + to_string(i), "Artist " + to_string(i), 
                                   "Album " + to_string(i), "3:00");
            testPlaylist.addSong(newSong);
        }
        
        cout << "Initial playlist:" << endl;
        testPlaylist.displayPlaylist();
        
        // Demonstrate O(1) removal after finding
        cout << "\nRemoving 'Song 3' from middle..." << endl;
        cout << "1. Find Song3 (O(n) search)" << endl;
        cout << "2. Update 2 pointers (O(1) removal)" << endl;
        testPlaylist.removeSong("Song 3");
        testPlaylist.displayPlaylist();
        
        cout << "\n--- PERFORMANCE SUMMARY ---" << endl;
        cout << "Operation        | Array | Linked List" << endl;
        cout << "------------------------------------" << endl;
        cout << "Add Song         | O(1)  | O(1)" << endl;
        cout << "Remove Song      | O(n)  | O(n) search + O(1) removal" << endl;
        cout << "Next/Previous    | O(1)  | O(1)" << endl;
        cout << "Search in Playlist| O(n)  | O(n)" << endl;
        cout << "\nKey Advantage: Linked list avoids expensive O(n) shifting!" << endl;
    }
};




// =============================================
// User Interface
// =============================================
void displayMenu() {
    cout << "\n=== MP3 PLAYLIST ORGANIZER ===" << endl;
    cout << "1. Browse Music Library" << endl;
    cout << "2. Search Song" << endl;
    cout << "3. Play from Category" << endl;
    cout << "4. Play Next Song" << endl;
    cout << "5. Play Previous Song" << endl;
    cout << "6. Show Current Song" << endl;
    cout << "7. Add New Song" << endl;
    cout << "8. Remove Song" << endl;
    cout << "9. Show Current Playlist" << endl;
    cout << "10. Performance Demo" << endl;
    cout << "0. Exit" << endl;
    cout << "Choose option: ";
}


// =============================================
// Main Function
// =============================================
int main() {
    cout << "🎵 MP3 Playlist Organizer - Optimized Version 🎵" << endl;
    
    MusicLibrary library;
    int choice = -1;
    
    while (choice != 0) {
        displayMenu();
        cin >> choice;
        
        switch(choice) {
            case 1:
                library.displayLibrary();
                break;
            case 2: {
                string title;
                cout << "Enter song title: ";
                cin.ignore();
                getline(cin, title);
                library.searchSong(title);
                break;
            }
            case 3: {
                string category;
                cout << "Enter category path: ";
                cin.ignore();
                getline(cin, category);
                library.setCurrentPlaylist(category);
                break;
            }
            case 4:
                library.playNext();
                break;
            case 5:
                library.playPrevious();
                break;
            case 6:
                library.displayCurrentSong();
                break;
            case 7: {
                string title, artist, album, duration, category;
                cout << "Enter song title: ";
                cin.ignore();
                getline(cin, title);
                cout << "Enter artist: ";
                getline(cin, artist);
                cout << "Enter album: ";
                getline(cin, album);
                cout << "Enter duration: ";
                getline(cin, duration);
                cout << "Enter category path: ";
                getline(cin, category);
                library.addSong(title, artist, album, duration, category);
                break;
            }
            case 8: {
                string title;
                cout << "Enter song title: ";
                cin.ignore();
                getline(cin, title);
                library.removeSongFromCurrent(title);
                break;
            }
            case 9:
                library.displayCurrentPlaylist();
                break;
            case 10:
                PerformanceTest::demonstrateArrayVsLinkedList();
                break;
            case 0:
                cout << "Goodbye! 🎵" << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    }
    
    return 0;
}