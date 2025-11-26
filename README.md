# MP3 Playlist Organizer

### Made by: Safa, Banaf, Sofea, Nessa, Sobena
---
A C++ music library management system that efficiently organizes songs using linked lists and tree data structures.

## Features

- **🎵 Music Library** - Hierarchical category organization using tree data structure
- **📝 Playlist Management** - Efficient song operations using doubly linked lists
- **🔍 Smart Search** - Search songs across all categories
- **🎮 Playback Controls** - Navigate between songs with next/previous
- **📊 Performance Optimized** - Demonstrates data structure efficiency

## Data Structures Used

### Linked Lists
- **Doubly Linked List** for playlist management
- **O(1)** insertion and deletion
- **O(1)** navigation between songs

### Tree Structure
- **N-ary Tree** for category hierarchy
- **O(h)** search time (h = tree height)
- Natural organization of music categories

## How to Use

1. **Browse Library** - View all categories and songs
2. **Search Songs** - Find songs by title across all categories
3. **Play from Category** - Set current playlist from any category
4. **Add Songs** - Add new songs to specific categories
5. **Navigate Playlist** - Use next/previous to browse songs
6. **Performance Demo** - See data structure benefits in action

## Example Categories
- Rock/Classic/Queen
- Pop/2000s/Britney Spears  
- Jazz/Modern/Norah Jones

## Download and Setup
1. Go to the GitHub repository page
2. Click the green "Code" button
3. Select "Download ZIP"
4. Extract the ZIP file to your preferred location
5. Open terminal/command prompt in the extracted folder
### Compilation
```bash
g++ -o music_library main.cpp

# On Windows:
music_library.exe

# On Linux/macOS:
./music_library
