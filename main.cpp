#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <fstream>

const std::string AUTOSAVE_FILE = "autosave.txt";

void clearScreen() {
    std::cout << "\033[2J\033[H";
}

void drawListsHorizontal(const std::vector<std::string>& todos,
                         const std::vector<std::string>& doings,
                         const std::vector<std::string>& finished) {
    size_t maxSize = std::max(todos.size(), std::max(doings.size(), finished.size()));
    const int colWidth = 30; // a bit wider for index + item

    std::cout << std::left
              << std::setw(colWidth) << "To Do"
              << std::setw(colWidth) << "Doing"
              << std::setw(colWidth) << "Finished"
              << "\n";

    std::cout << std::string(colWidth * 3, '=') << "\n";

    for (size_t i = 0; i < maxSize; ++i) {
        // todos
        if (i < todos.size()) {
            if (i == 0)
                std::cout << std::setw(colWidth) << todos[i];  // title without index
            else
                std::cout << std::setw(colWidth) << ("#" + std::to_string(i) + " " + todos[i]);
        } else {
            std::cout << std::setw(colWidth) << "";
        }

        // doings
        if (i < doings.size()) {
            if (i == 0)
                std::cout << std::setw(colWidth) << doings[i];
            else
                std::cout << std::setw(colWidth) << ("#" + std::to_string(i) + " " + doings[i]);
        } else {
            std::cout << std::setw(colWidth) << "";
        }

        // finished
        if (i < finished.size()) {
            if (i == 0)
                std::cout << std::setw(colWidth) << finished[i];
            else
                std::cout << std::setw(colWidth) << ("#" + std::to_string(i) + " " + finished[i]);
        } else {
            std::cout << std::setw(colWidth) << "";
        }

        std::cout << "\n";
    }

    std::cout << std::string(colWidth * 3, '=') << "\n";
}

void saveProject(const std::vector<std::string>& todos,
                 const std::vector<std::string>& doings,
                 const std::vector<std::string>& finished) {
    std::ofstream ofs(AUTOSAVE_FILE);
    if (!ofs) {
        std::cerr << "Error saving to autosave file!\n";
        return;
    }
    for (const auto& item : todos) ofs << item << "\n";
    ofs << "\n";
    for (const auto& item : doings) ofs << item << "\n";
    ofs << "\n";
    for (const auto& item : finished) ofs << item << "\n";
}

void loadProject(std::vector<std::string>& todos,
                 std::vector<std::string>& doings,
                 std::vector<std::string>& finished) {
    std::ifstream ifs(AUTOSAVE_FILE);
    if (!ifs) {
        // File does not exist - start fresh
        todos = {"To do:"};
        doings = {"Doing:"};
        finished = {"Finished:"};
        return;
    }

    todos.clear();
    doings.clear();
    finished.clear();

    enum class Section { NONE, TODO, DOING, FINISHED } current = Section::NONE;

    std::string line;
    while (std::getline(ifs, line)) {
        if (line == "To do:") {
            current = Section::TODO;
            todos.push_back(line);
        } else if (line == "Doing:") {
            current = Section::DOING;
            doings.push_back(line);
        } else if (line == "Finished:") {
            current = Section::FINISHED;
            finished.push_back(line);
        } else if (line.empty()) {
            // blank line - ignore
        } else {
            switch (current) {
                case Section::TODO: todos.push_back(line); break;
                case Section::DOING: doings.push_back(line); break;
                case Section::FINISHED: finished.push_back(line); break;
                default: break;
            }
        }
    }
}

void addItem(std::vector<std::string>& list, const std::string& item) {
    list.push_back(item);
}

bool removeItem(std::vector<std::string>& list, int index, const std::string& title) {
    if (index == 0) {
        std::cout << "❌ You cannot remove the title '" << title << "'.\n";
        return false;
    }
    if (index > 0 && index < static_cast<int>(list.size())) {
        std::cout << "🗑️ Removing from " << title << ": \"" << list[index] << "\"\n";
        list.erase(list.begin() + index);
        return true;
    } else {
        std::cout << "⚠️ Invalid index for " << title << ".\n";
        return false;
    }
}

bool moveItem(std::vector<std::string>& fromList, std::vector<std::string>& toList, int index,
              const std::string& fromTitle, const std::string& toTitle) {
    if (index == 0) {
        std::cout << "❌ You cannot move the title '" << fromTitle << "'.\n";
        return false;
    }
    if (index > 0 && index < static_cast<int>(fromList.size())) {
        std::string item = fromList[index];
        std::cout << "🔄 Moving item \"" << item << "\" from " << fromTitle << " to " << toTitle << ".\n";
        fromList.erase(fromList.begin() + index);
        toList.push_back(item);
        return true;
    } else {
        std::cout << "⚠️ Invalid index for " << fromTitle << ".\n";
        return false;
    }
}

std::vector<std::string>* getListByName(std::string_view name,
                                        std::vector<std::string>& todos,
                                        std::vector<std::string>& doings,
                                        std::vector<std::string>& finished) {
    if (name == "todo") return &todos;
    else if (name == "doing") return &doings;
    else if (name == "finished") return &finished;
    else return nullptr;
}

int main() {
    std::vector<std::string> todos;
    std::vector<std::string> doings;
    std::vector<std::string> finished;

    // On startup, prompt user for "new <projectname>" or just load autosave.
    std::cout << "Type 'new <projectname>' to start a new project, or press Enter to load last autosave:\n> ";
    std::string startupInput;
    std::getline(std::cin, startupInput);

    if (startupInput.rfind("new ", 0) == 0) {
        // New project
        todos = {"To do:"};
        doings = {"Doing:"};
        finished = {"Finished:"};
        saveProject(todos, doings, finished);
        std::cout << "New project created.\n";
    } else {
        // Load autosave
        loadProject(todos, doings, finished);
        std::cout << "Autosave loaded.\n";
    }

    while (true) {
        clearScreen();  // Clear terminal screen before drawing
        drawListsHorizontal(todos, doings, finished);

        std::cout << "\nEnter a command:\n> ";
        std::string input;
        std::getline(std::cin, input);

        std::istringstream iss(input);
        std::string command;
        iss >> command;

        if (command == "exit") {
            std::cout << "\nExiting program. Goodbye!\n";
            break;
        }
        else if (command == "add") {
            std::string listName;
            iss >> listName;
            std::string item;
            std::getline(iss >> std::ws, item);

            if (item.empty()) {
                std::cout << "⚠️ Nothing to add. Please provide an item.\n";
                continue;
            }

            auto list = getListByName(listName, todos, doings, finished);
            if (list) {
                addItem(*list, item);
                std::cout << "✅ Added to " << listName << ": \"" << item << "\"\n";
                saveProject(todos, doings, finished);
            } else {
                std::cout << "❓ Unknown list '" << listName << "'. Use 'todo', 'doing', or 'finished'.\n";
            }
        }
        else if (command == "remove") {
            std::string listName;
            iss >> listName;
            int index;
            if (!(iss >> index)) {
                std::cout << "⚠️ Please provide a valid index to remove.\n";
                continue;
            }

            auto list = getListByName(listName, todos, doings, finished);
            if (list) {
                if (removeItem(*list, index, listName)) {
                    saveProject(todos, doings, finished);
                }
            } else {
                std::cout << "❓ Unknown list '" << listName << "'. Use 'todo', 'doing', or 'finished'.\n";
            }
        }
        else if (command == "move") {
            std::string fromListName, toListName;
            int index;
            iss >> fromListName >> index >> toListName;

            if (fromListName.empty() || toListName.empty() || !iss) {
                std::cout << "⚠️ Usage: move <from-list> <index> <to-list>\n";
                continue;
            }

            auto fromList = getListByName(fromListName, todos, doings, finished);
            auto toList = getListByName(toListName, todos, doings, finished);

            if (!fromList) {
                std::cout << "❓ Unknown from-list '" << fromListName << "'. Use 'todo', 'doing', or 'finished'.\n";
                continue;
            }
            if (!toList) {
                std::cout << "❓ Unknown to-list '" << toListName << "'. Use 'todo', 'doing', or 'finished'.\n";
                continue;
            }

            if (moveItem(*fromList, *toList, index, fromListName, toListName)) {
                saveProject(todos, doings, finished);
            }
        }
        else {
            std::cout << "❓ Unknown command. Use 'add', 'remove', 'move', or 'exit'.\n";
        }
    }

    return 0;
}
