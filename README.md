# Tools
- Git / Gitlab (repo.kamit.fi)
- Tortoise Git
- CMake (and cmake-gui)
- Visual Studio

# Setting up version control
First, make file named: ".gitignore" to the project root folder.
Add followig line to .gitignore:
```
build
```

# Adding files to repository
### TortoiseGit
- Mouse right click on folder: Tortoise Git -> Add
- Mouse right click on folder: Tortoise Git -> Commit
- Mouse right click on folder: Tortoise Git -> Push

## Command line
```
git add .
git pull
git commit -m"Commit message comes here"
git push
```

# Compiling
Open cmake-gui (Start Menu -> Find cmake-gui)

**In cmake-gui:**

Where is the source code (location of CMakeLists.txt -file):

**"project root folder"**

Where to build binaries:

**"project root folder"/build**

Press, "Configure" (vaselect correct visual studio version), then "Generate" and finally "Open Project".

Remember to set correct project from Visual Studio to be used as startup project: "Right click project -> Set As Startup Project"

