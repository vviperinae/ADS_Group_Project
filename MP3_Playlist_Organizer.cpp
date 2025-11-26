/*
TFB2023/TEB1113:Algorithm and Data Structure 

Title of Project: MP3 Playlist Organizer - Efficient Music Management System

Authors/ID/Course
Safa Sarfraz 24001006 COE
Aisyah Sofea Binti Mohd Sallehuddin	22011342 COE
Dania Anessa binti Mohd Aswawi 	22011086 COE
Sobena A/P Ramachanthirarao 	22010905 COE
Puteri Banafsha Binti Azmi 	 22010863 COE

Description of the Project: 

A music player application that uses linked lists for instant playlist navigation and tree structures 
for organized music categorization. This design ensures fast song skipping and efficient playlist 
management, even with large music libraries.

*/



#include <iostream>
#include <string>

using namespace std;


// =================================================================
// Doubly Linked List for playlist navigation(search, next song, etc)
// ==================================================================

class Song {
public:  // added public access modifier so song data can be accessed
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
    // constructor initializes empty playlist
    Playlist() {
        head = nullptr;
        tail = nullptr;
        current = nullptr;
        songCount = 0;
    }

    // add a new song to the end of the playlist
    void addSong(Song* newSong) {
        // if playlist is empty, new song becomes head, tail, and current
        if (head == nullptr) {
            head = newSong;
            tail = newSong;
            current = newSong;
        } 
        else {
            // connect new song to old tail
            tail->next = newSong;
            newSong->prev = tail;
            // move tail pointer to new song
            tail = newSong;
        }

        songCount++;
        cout << "'" << newSong->title << "' added to playlist." << endl;  // fixed quote placement
    }

    // remove song by title
    void removeSong(string title) {
        Song* temp = head;

        while (temp != nullptr) {
            // found matching song
            if (temp->title == title) {
                // case 1: song is not the first song
                if (temp->prev != nullptr) {
                    temp->prev->next = temp->next;
                } else {
                    head = temp->next; // removing head
                }

                // case 2: song is not the last song
                if (temp->next != nullptr) {
                    temp->next->prev = temp->prev;
                } else {
                    tail = temp->prev; // removing tail
                }

                // if removed song was playing, move to next or previous
                if (current == temp) {
                    if (temp->next != nullptr) {
                        current = temp->next;
                    } else {
                        current = temp->prev;
                    }
                }

                delete temp;
                songCount--;

                cout << "'" << title << "' removed from playlist." << endl;  // fixed quote placement
                return;
            }

            temp = temp->next;
        }

        cout << "Song not found: " << title << endl;
    }

    // move to next song
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

    // move to previous song
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

    // search for song by title
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

    // display playlist contents
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

    // get current song
    Song* getCurrentSong() {
        return current;
    }

    // get song count
    int getSongCount() {
        return songCount;
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
        cout << "Added child category: " << child->name << " to " << name << endl;
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
        cout << "Created Music Library" << endl;
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
        cout << "Finished creating category: " << path << endl;
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
                        cout << "Category not found: " << currentPart << " in path " << path << endl;
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
        Song* found = node->playlist->searchSong(title); //first check current node's playlist
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
        //add counts from all children recursively
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
            cout << "Music Library cleaned up" << endl;
        }
    }

    //getter for root node for external access
    CategoryNode* getRoot() {
        return root;
    }
};

class MusicLibrary {
private:
    MusicTree tree;
    Playlist* currentPlaylist;
    Song* currentSong;

public:
    MusicLibrary() {
        currentPlaylist = nullptr;
        currentSong = nullptr;
    }

    void displayLibrary() {
        //display the entire music library structure
        tree.displayLibrary();
    }

    void searchSong(string title) {
        //search for a song by title in the entire library
        Song* found = tree.searchSongInLibrary(title);
        if (found != nullptr) {
            cout << "Found: " << found->title << " - " << found->artist << " (" << found->duration << ")" << endl;
        } else {
            cout << "Song not found: " << title << endl;
        }
    }

    void setCurrentPlaylist(string categoryPath) {
        //set the current playlist to a specific category
        CategoryNode* category = tree.findCategory(categoryPath);
        if (category != nullptr) {
            currentPlaylist = category->playlist;
            currentSong = currentPlaylist->getCurrentSong();
            cout << "Now playing from category: " << categoryPath << endl;
            currentPlaylist->displayPlaylist();
        } else {
            cout << "Category not found: " << categoryPath << endl;
        }
    }

    void playNext() {
        //play the next song in current playlist
        if (currentPlaylist != nullptr) {
            currentSong = currentPlaylist->playNext();
            if (currentSong != nullptr) {
                cout << "Now playing: " << currentSong->title << " - " << currentSong->artist << endl;
            }
        } else {
            cout << "No playlist selected!" << endl;
        }
    }

    void playPrevious() {
        //play the previous song in current playlist
        if (currentPlaylist != nullptr) {
            currentSong = currentPlaylist->playPrevious();
            if (currentSong != nullptr) {
                cout << "Now playing: " << currentSong->title << " - " << currentSong->artist << endl;
            }
        } else {
            cout << "No playlist selected!" << endl;
        }
    }

    void displayCurrentSong() {
        //display the currently playing song
        if (currentSong != nullptr) {
            cout << "Current song: " << currentSong->title << " - " << currentSong->artist 
                 << " (" << currentSong->duration << ")" << endl;
        } else {
            cout << "No song is currently playing!" << endl;
        }
    }

    void addSong(string title, string artist, string album, string duration, string categoryPath) {
        //add a new song to a specific category
        //first ensure the category exists
        tree.addCategory(categoryPath);
        
        //find the category
        CategoryNode* category = tree.findCategory(categoryPath);
        if (category != nullptr) {
            Song* newSong = new Song(title, artist, album, duration);
            category->playlist->addSong(newSong);
            cout << "Song added successfully!" << endl;
        } else {
            cout << "Failed to add song to category: " << categoryPath << endl;
        }
    }

    void removeSongFromCurrent(string title) {
        //remove a song from the current playlist
        if (currentPlaylist != nullptr) {
            currentPlaylist->removeSong(title);
            currentSong = currentPlaylist->getCurrentSong();
        } else {
            cout << "No playlist selected!" << endl;
        }
    }

    void displayCurrentPlaylist() {
        //display the current playlist
        if (currentPlaylist != nullptr) {
            currentPlaylist->displayPlaylist();
        } else {
            cout << "No playlist selected!" << endl;
        }
    }
};

// regular function for performance demonstration
void demonstratePerformance() {
    cout << "\n=== DATA STRUCTURE PERFORMANCE DEMONSTRATION ===" << endl;
    
    // linked list vs array comparison for playlists
    cout << "\n--- LINKED LIST VS ARRAY FOR PLAYLISTS ---" << endl;
    
    cout << "\nARRAY APPROACH (Baseline):" << endl;
    cout << "Playlist as Array: [Song1, Song2, Song3, Song4, Song5]" << endl;
    cout << "Removing 'Song3' from middle:" << endl;
    cout << "1. Find Song3 (O(n) search)" << endl;
    cout << "2. Shift Song4 and Song5 left (O(n) shifting)" << endl;
    cout << "Total: O(n) - gets slower with more songs" << endl;
    
    cout << "\nLINKED LIST APPROACH (Optimized):" << endl;
    Playlist testPlaylist;
    
    // add songs to demonstrate linked list
    for (int i = 1; i <= 5; i++) {
        Song* newSong = new Song("Song " + to_string(i), "Artist " + to_string(i), 
                               "Album " + to_string(i), "3:00");
        testPlaylist.addSong(newSong);
    }
    
    cout << "Initial playlist:" << endl;
    testPlaylist.displayPlaylist();
    
    cout << "\nRemoving 'Song 3' from middle..." << endl;
    cout << "1. Find Song3 (O(n) search)" << endl;
    cout << "2. Update 2 pointers (O(1) removal)" << endl;
    testPlaylist.removeSong("Song 3");
    testPlaylist.displayPlaylist();
    
    // tree data structure demonstration
    cout << "\n--- TREE DATA STRUCTURE FOR CATEGORIES ---" << endl;
    
    cout << "\nFLAT STRUCTURE (Inefficient):" << endl;
    cout << "All categories in one list:" << endl;
    cout << "- Rock" << endl;
    cout << "- Pop" << endl;
    cout << "- Rock/Classic" << endl;
    cout << "- Rock/Metal" << endl;
    cout << "- Pop/2000s" << endl;
    cout << "Search time: O(n) - linear search through all categories" << endl;
    cout << "Organization: Poor - no hierarchy" << endl;
    
    cout << "\nTREE STRUCTURE (Efficient):" << endl;
    cout << "Hierarchical organization:" << endl;
    cout << "Music Library" << endl;
    cout << "  ├── Rock" << endl;
    cout << "  │   ├── Classic" << endl;
    cout << "  │   └── Metal" << endl;
    cout << "  └── Pop" << endl;
    cout << "      └── 2000s" << endl;
    
    // demonstrate tree efficiency
    MusicTree demoTree;
    
    cout << "\nBuilding category tree..." << endl;
    demoTree.addCategory("Rock/Classic/Queen");
    demoTree.addCategory("Rock/Metal/Metallica");
    demoTree.addCategory("Pop/2000s/Britney Spears");
    
    cout << "\nTree structure benefits:" << endl;
    cout << "- Search time: O(log n) with balanced tree" << endl;
    cout << "- Natural hierarchy: matches real-world organization" << endl;
    cout << "- Efficient navigation: follow parent-child relationships" << endl;
    cout << "- Scalable: easy to add new subcategories" << endl;
    
    cout << "\nDemonstrating tree search..." << endl;
    cout << "Finding 'Rock/Metal/Metallica':" << endl;
    cout << "1. Start at root 'Music Library'" << endl;
    cout << "2. Navigate to 'Rock' (O(1))" << endl;
    cout << "3. Navigate to 'Metal' (O(1))" << endl;
    cout << "4. Navigate to 'Metallica' (O(1))" << endl;
    cout << "Total: O(h) where h is tree height" << endl;
    
    cout << "\n--- PERFORMANCE SUMMARY ---" << endl;
    cout << "Data Structure  | Operation         | Time Complexity" << endl;
    cout << "----------------------------------------------------" << endl;
    cout << "Linked List     | Add Song          | O(1)" << endl;
    cout << "Linked List     | Remove Song       | O(n) search + O(1) removal" << endl;
    cout << "Linked List     | Next/Previous     | O(1)" << endl;
    cout << "Array           | Remove Song       | O(n) search + O(n) shifting" << endl;
    cout << "Tree            | Find Category     | O(h) where h is tree height" << endl;
    cout << "Tree            | Add Category      | O(h) traversal + O(1) insertion" << endl;
    cout << "Flat List       | Find Category     | O(n) linear search" << endl;
    
    cout << "\nKEY ADVANTAGES:" << endl;
    cout << "Linked lists avoid expensive O(n) shifting during removal" << endl;
    cout << "Trees provide efficient O(h) search in hierarchical data" << endl;
    cout << "Trees naturally represent real-world category relationships" << endl;
    cout << "Both structures excel in their specific use cases" << endl;
    
    
    // cleanup
    demoTree.cleanup();
}

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
    cout << "MP3 Playlist Organizer - Optimized Version" << endl;
    
    MusicLibrary library;
    int choice = -1;
    
    while (choice != 0) {
        displayMenu();
        cin >> choice;
        
        switch(choice) {
            case 1:
                //browse the entire music library
                library.displayLibrary();
                break;
            case 2: {
                //search for a song by title
                string title;
                cout << "Enter song title: ";
                cin.ignore();
                getline(cin, title);
                library.searchSong(title);
                break;
            }
            case 3: {
                //set current playlist from a category
                string category;
                cout << "Enter category path: ";
                cin.ignore();
                getline(cin, category);
                library.setCurrentPlaylist(category);
                break;
            }
            case 4:
                //play next song in current playlist
                library.playNext();
                break;
            case 5:
                //play previous song in current playlist
                library.playPrevious();
                break;
            case 6:
                //display current playing song
                library.displayCurrentSong();
                break;
            case 7: {
                //add a new song to a category
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
                //remove song from current playlist
                string title;
                cout << "Enter song title: ";
                cin.ignore();
                getline(cin, title);
                library.removeSongFromCurrent(title);
                break;
            }
            case 9:
                //display current playlist
                library.displayCurrentPlaylist();
                break;
            case 10:
                //run performance demonstration
                demonstratePerformance();
                break;
            case 0:
                //exit the program
                cout << "Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice!" << endl;
        }
    }
    
    return 0;
}
