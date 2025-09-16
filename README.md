# Simple Console To-Do List Manager

A minimal command-line tool in C++ to manage tasks across three lists: **To Do**, **Doing**, and **Finished**.

---

## Features

- Display tasks horizontally in three columns.
- Add, remove, and move tasks between lists.
- Autosave project state to `autosave.txt`.
- Load autosave or start a new project on launch.

---

## Usage

### Starting the program

- Run the program.
- To start a new project, type:

new <projectname>

- To load the last autosave, just press Enter.

### Commands

- **Add a task**

add <list> <task_description>

Example:

add todo Buy groceries


- **Remove a task**

remove <list> <task_index>

Example:

remove doing 2


- **Move a task**

move <from-list> <task_index> <to-list>

Example:

move todo 1 doing


- **Exit the program**

exit


---

## Lists

- `todo` — tasks to do
- `doing` — tasks in progress
- `finished` — completed tasks

---

## Notes

- The first item in each list is a title and cannot be removed or moved.
- Project state autosaves after each change in `autosave.txt`.
- Use a terminal that supports ANSI escape codes for clear screen functionality.

---

## Build

Compile with a C++11-compatible compiler, e.g. `g++`:

```bash
g++ -o todo_manager todo_manager.cpp

Run the program:

./todo_manager

License

MIT License (or your preferred license)

Enjoy managing your tasks!


If you want, I can also help with a simple Makefile or more detailed build instructions. Just ask!

