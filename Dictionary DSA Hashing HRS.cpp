#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <conio.h>
#include<cstdlib>
#include <cctype>
#include <chrono>
#include <thread>

using namespace std;
//other functions
void displayRedText(const string& text) {
    cout << "\033[1;31m" << text << "\033[0m"; // Red color
}
void displayBlueText(const string& text) {
    cout << "\033[1;34m" << text << "\033[0m"; // Blue color
}
void displayGreenText(const string& text) {
    cout << "\033[1;32m" << text << "\033[0m"; // Green color
}
void displayYellowText(const string& text) {
    cout << "\033[1;33m" << text << "\033[0m"; // Yellow color
}
void displayCyanText(const string& text) {
    cout << "\033[1;36m" << text << "\033[0m"; // Cyan color
}
void displayMagentaText(const string& text) {
    cout << "\033[1;35m" << text << "\033[0m"; // Magenta color
}
void clearScreen() {
    cout << "\033[2J\033[1;1H"; \
}
void printLogo() {
    // Red color
    cout << "\033[1;31m";
    cout << "\t\t\t\t _   _ _    _          _____  _      _   _   " << endl;
    cout << "\t\t\t\t| \\ | | |  | |        |  __ \\(_)    | | (_)  " << endl;
    cout << "\033[0m"; // Reset color

    // Blue color
    cout << "\033[1;34m";
    cout << "\t\t\t\t|  \\| | |  | | ______ | |  | |_  __| | _  ___  _ __   __ _ _ __ _   _" << endl;
    cout << "\033[0m"; // Reset color

    // Green color
    cout << "\033[1;32m";
    cout << "\t\t\t\t| . ` | |  | ||_____|| |  | | |/ __| __| |/ _ \\| ' \\ / _` | '__| | | |" << endl;
    cout << "\033[0m"; // Reset color

    // Yellow color
    cout << "\033[1;33m";
    cout << "\t\t\t\t| |\\  | |__| |        | |__| | | (__| |_| | (_) | | | | (_| | |  | |_| |" << endl;
    cout << "\033[0m"; // Reset color

    // Magenta color
    cout << "\033[1;35m";
    cout << "\t\t\t\t|_| \\_|\\____/         |_____/|_|\\___|\\__|_|\\___/|_| |_|\\__,_|_|   \\__, |" << endl;
    cout << "\t\t\t\t                                                                   __/ |" << endl;
    cout << "\t\t\t\t                                                                  |___/ " << endl;
    cout << "\033[0m"; // Reset color
}
void loadingAnimation(int durationInSeconds) {
    auto start = chrono::steady_clock::now();
    while (true) {
        auto end = chrono::steady_clock::now();
        auto elapsedSeconds = chrono::duration_cast<chrono::seconds>(end - start).count();
        if (elapsedSeconds >= durationInSeconds) {
            break;
        }

        static int count = 0;
        cout << "\033[1;36m"; 
        cout << "\t\t\t\t";

        switch (count % 2) {
        case 0:
            system("cls");
            printLogo();
            cout << "\t\t\t\t";
            displayGreenText("Loading");
            break;
        case 1:
            system("cls");
            printLogo();
            cout << "\t\t\t\t";
            displayBlueText("Loading...");
            break;
        }

        cout << flush;
        cout << "\033[0m"; // Reset color

        count++;
        this_thread::sleep_for(chrono::milliseconds(500)); 
    }
}
//end of other functions
struct Lexeme {
    string word;
    string meaning;
    bool flag;
    Lexeme() {
        flag = false;
    }
};
string removeSpaces(const string& str) {
    string result;
    for (char c : str) {
        if (!isspace(c)) {
            result += c;
        }
    }
    return result;
}
class Dictionary
{
    Lexeme* Dict;
    int capacity;

public:
    Dictionary(int size) {
        capacity = size;
        Dict = new Lexeme[capacity]();
    }

    //1
    void LoadDictionary()
    {
        ifstream file("Dictionary.txt");
        if (!file.is_open()) {
            displayRedText("\nUnable to open file");
            return;
        }

        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            Lexeme lexeme;
            if (getline(iss, lexeme.word, '\t') && getline(iss, lexeme.meaning))
            {
                for (int i = 0; i < lexeme.word.length(); i++)
                {
                    if (isupper(lexeme.word[i]))
                    {
                        lexeme.word[i] = tolower(lexeme.word[i]);
                    }
                }
                lexeme.word = removeSpaces(lexeme.word);

                for (int i = 0; i < lexeme.meaning.length(); i++)
                {
                    if (isupper(lexeme.meaning[i]))
                    {
                        lexeme.meaning[i] = tolower(lexeme.meaning[i]);
                    }
                }
                lexeme.meaning = removeSpaces(lexeme.meaning);

                AddWord(lexeme.word, lexeme.meaning);
            }
            else {
                cout << "\n\t\t\t\t";
                displayRedText("Invalid line format: " + line + "\n");
            }
        }
        file.close();
    }

    //2
    int ASCIIindex(const string& word)
    {
        int index = 0;
        for (int i = 0; i < word.length(); i++)
        {
            index += word[i];
        }
        index = index % capacity;
        return index;
    }
    //3
    void AddWord(const string& word, const string& meaning)
    {

        int index = ASCIIindex(word);
        while (Dict[index].flag) {
            index = (index + 1) % capacity;
        }
        Dict[index].word = word;
        Dict[index].meaning = meaning;
        Dict[index].flag = true;
    }
    //4
    bool WordExists(const string& word)
    {
        int index = ASCIIindex(word);
        while (Dict[index].flag) {
            if (Dict[index].word == word)
            {
                return true;
            }
            index = (index + 1) % capacity;
        }
        return false;
    }
    //5
    string Meaning(const string& word)
    {
        int index = ASCIIindex(word);
        while (Dict[index].flag) {
            if (Dict[index].word == word) {
                return Dict[index].meaning;
            }
            index = (index + 1) % capacity;
        }
        return "";
    }
    //6
    void DisplayDictionary() {
        int count = 0;
        for (int i = 0; i < capacity; ++i) {
            if (Dict[i].flag) {
                cout << "\t\t\t\t";
                displayYellowText("Word: ");
                cout << Dict[i].word;
                displayCyanText(", Meaning: ");
                cout << Dict[i].meaning << endl;
                count++;
            }
        }
        cout << "\t\t\t\t";
        displayBlueText("Count is : ");
        cout << count << endl;
        return;
    }

    //7
    void AddWordManually() {
        string newWord;
        cout << "\n\t\t\t\t";
        displayGreenText("Enter the word to add: ");
        getline(cin, newWord);

        for (int i = 0; i < newWord.length(); i++)
        {
            if (isupper(newWord[i]))
            {
                newWord[i] = tolower(newWord[i]);
            }
        }

        if (WordExists(newWord)) {
            cout << "\n\t\t\t\t";
            displayRedText("Word already exists in the dictionary.\n");
            return;
        }

        string meaning;
        cout << "\n\t\t\t\t";
        displayGreenText("Enter the meaning of the word: ");
        getline(cin, meaning);

        for (int i = 0; i < meaning.length(); i++)
        {
            if (isupper(meaning[i]))
            {
                meaning[i] = tolower(meaning[i]);
            }
        }

        AddWord(newWord, meaning);
        cout << "\n\t\t\t\t";
        displayBlueText("Word added to the dictionary.\n");
    }

    //8
    void Search() {
        system("cls");
        printLogo();
        cout << "\n\t\t\t\t";
        displayGreenText("Enter Word For Search : ");
        string word;
        char ch;
        while ((ch = _getche()) != '\r') {
            system("cls");
            printLogo();
            cout << "\t\t\t\tEnter Word For Search : " << word;
            if (ch == '\b') {
                if (!word.empty()) {
                    word.pop_back();
                    cout << "\b \b";
                    SuggestWords(word);
                }
            }
            else {
                cout << ch;
                word = word + ch;

                for (int i = 0; i < word.length(); i++) {
                    if (isupper(word[i])) {
                        word[i] = tolower(word[i]);
                    }
                }

                SuggestWords(word);
            }
        }
        cout << endl;

        string meaning = Meaning(word);
        if (!meaning.empty()) {
            cout << "\n\t\t\t\t";
            displayCyanText("Word: ");
            cout << word;
            displayYellowText(", Meaning: ");
            cout << meaning << endl;
        }
        else {
            cout << "\n\t\t\t\t";
            displayRedText("Word not found in the dictionary.\n");
        }
    }

    //9
    void DeleteWord(const string& word) {
        int index = ASCIIindex(word);
        while (Dict[index].flag) {
            if (Dict[index].word == word) {
                cout << "\n\t\t\t\t";
                displayYellowText("Do you want to delete the word '");
                cout << word << "'? Enter ->(Yes/No) : ";
                string confirmation;
                getline(cin, confirmation);

                while (1) {
                    for (int i = 0; i < confirmation.length(); i++)
                    {
                        if (isupper(confirmation[i]))
                        {
                            confirmation[i] = tolower(confirmation[i]);
                        }
                    }

                    if (confirmation == "yes") {
                        Dict[index].flag = false;
                        cout << "\n\t\t\t\t";
                        displayBlueText("Word '");
                        cout << "\n\t\t\t\t";
                        displayBlueText(word + "' deleted from the dictionary.\n");

                        return;
                    }
                    else if (confirmation == "no") {
                        cout << "\n\t\t\t\t";
                        displayCyanText("Deletion canceled.\n");
                        return;
                    }
                    else {
                        cout << "\n\t\t\t\t";
                        displayRedText("Invalid input. Please enter Yes or No.\n");
                        getline(cin, confirmation);
                        for (int i = 0; i < confirmation.length(); i++)
                        {
                            if (isupper(confirmation[i]))
                            {
                                confirmation[i] = tolower(confirmation[i]);
                            }
                        }
                    }
                }
            }
            index = (index + 1) % capacity;
        }
        cout << "\n\t\t\t\t";
        displayRedText("Word not found in the dictionary. Cannot delete.\n");
    }

    //10
    void UpdateWord(const string& word) {
        int index = ASCIIindex(word);
        while (Dict[index].flag) {
            if (Dict[index].word == word) {
                cout << "\n\t\t\t\t";
                displayGreenText("Word found in the dictionary. Current Meaning: ");
                cout << Dict[index].meaning << endl;

                string newMeaning;
                cout << "\n\t\t\t\t";
                displayYellowText("Enter the new meaning for the word '");
                cout << word << "': ";
                getline(cin, newMeaning);

                for (int i = 0; i < newMeaning.length(); i++) {
                    if (isupper(newMeaning[i])) {
                        newMeaning[i] = tolower(newMeaning[i]);
                    }
                }
                cout << "\n\t\t\t\t";
                displayYellowText("Do you want to update the word '");
                cout << "\n\t\t\t\t";
                displayYellowText(word + "'? Enter ->(Yes/No): ");

                string confirmation;
                getline(cin, confirmation);

                for (int i = 0; i < confirmation.length(); i++) {
                    if (isupper(confirmation[i])) {
                        confirmation[i] = tolower(confirmation[i]);
                    }
                }

                while (1) {
                    if (confirmation == "Yes" || confirmation == "yes") {
                        Dict[index].meaning = newMeaning;
                        cout << "\n\t\t\t\t";
                        displayGreenText("Word '");
                        cout << "\n\t\t\t\t";
                        displayGreenText(word + "' updated in the dictionary.");
                        cout << endl;

                        return;
                    }
                    else if (confirmation == "No" || confirmation == "no") {
                        cout << "\n\t\t\t\t";
                        displayCyanText("Update canceled.\n");

                        return;
                    }
                    else {
                        cout << "\n\t\t\t\t";
                        displayRedText("Invalid input. Please enter Yes or No.");
                        cout << endl;

                        getline(cin, confirmation);
                        for (int i = 0; i < confirmation.length(); i++) {
                            if (isupper(confirmation[i])) {
                                confirmation[i] = tolower(confirmation[i]);
                            }
                        }
                    }
                }
            }
            index = (index + 1) % capacity;
        }
        cout << "\n\t\t\t\t";
        displayRedText("Word not found in the dictionary. Cannot update.");
        cout << endl;

    }
    //11
    void SuggestWords(const string& word) {
        const int MAX_SUGGESTIONS = 10;
        string suggestions[MAX_SUGGESTIONS];
        int count = 0;

        for (int i = 0; i < capacity && count < MAX_SUGGESTIONS; ++i) {
            if (Dict[i].flag) {
                bool startsWithWord = true;
                for (size_t j = 0; j < word.length(); ++j) {
                    if (j >= Dict[i].word.length() || Dict[i].word[j] != word[j]) {
                        startsWithWord = false;
                        break;
                    }
                }
                if (startsWithWord) {
                    suggestions[count] = Dict[i].word;
                    count++;
                }
            }
        }

        if (count > 0) {
            cout << "\n\t\t\t\t";
            displayYellowText("Suggestions:\n");
            for (int i = 0; i < count; ++i) {
                cout << "\t\t\t\t";
                displayGreenText(suggestions[i]);
                cout << endl;
            }
        }
        else {
            cout << "\n\t\t\t\t";
            displayRedText("No suggestions found.\n");
        }
    }

};

int main() {
    printLogo();
    loadingAnimation(1);
    Dictionary Dict(500000);
    Dict.LoadDictionary();
    char choice;

    while (true) {
        system("cls");
        printLogo();
        displayBlueText("\n\t\t\t\tDictionary Menu:\n");
        displayBlueText("\t\t\t\t1. ");
        displayGreenText("Display Dictionary\n");
        displayBlueText("\t\t\t\t2. ");
        displayYellowText("Add Word\n");
        displayBlueText("\t\t\t\t3. ");
        displayCyanText("Search Word\n");
        displayBlueText("\t\t\t\t4. ");
        displayMagentaText("Delete Word\n");
        displayBlueText("\t\t\t\t5. ");
        displayRedText("Update Word\n");
        displayBlueText("\t\t\t\t6. ");
        displayBlueText("Suggest Words\n");
        displayBlueText("\t\t\t\tEnter your choice (1-6): ");

        choice = _getche();

        switch (choice) {
        case '1':
            system("cls");
            printLogo();
            cout << "\n";
            displayGreenText("\t\t\t\tDisplaying Dictionary...\n");
            Dict.DisplayDictionary();
            cout << "\n\t\t\t\t";
            displayBlueText("Press any key to continue :)\n");
            _getch();
            break;
        case '2':
            system("cls");
            printLogo();
            cout << "\n";
            displayYellowText("\t\t\t\tAdding Word...\n");
            Dict.AddWordManually();
            cout << "\n\t\t\t\t";
            displayBlueText("Press any key to continue :)\n");
            _getch();
            break;
        case '3':
            system("cls");
            printLogo();
            cout << "\n";
            displayCyanText("\t\t\t\tSearching Word...\n");
            Dict.Search();
            cout << "\n\t\t\t\t";
            displayBlueText("Press any key to continue :)\n");
            _getch();
            break;
        case '4': {
            system("cls");
            printLogo();
            string wordToDelete;
            cout << "\n\t\t\t\t";
            displayGreenText("Enter the word you want to delete: ");

            getline(cin, wordToDelete);
            for (int i = 0; i < wordToDelete.length(); i++) {
                if (isupper(wordToDelete[i])) {
                    wordToDelete[i] = tolower(wordToDelete[i]);
                }
            }
            displayMagentaText("\n\t\t\t\tDeleting Word...\n");
            Dict.DeleteWord(wordToDelete);
            cout << "\n\t\t\t\t";
            displayBlueText("Press any key to continue :)\n");
            _getch();
            break;
        }
        case '5': {
            system("cls");
            printLogo();
            string wordToUpdate;
            cout << endl;
            cout << "\t\t\t\t";
            displayYellowText("Enter the word you want to update: ");

            getline(cin, wordToUpdate);
            for (int i = 0; i < wordToUpdate.length(); i++) {
                if (isupper(wordToUpdate[i])) {
                    wordToUpdate[i] = tolower(wordToUpdate[i]);
                }
            }
            displayRedText("\n\t\t\t\tUpdating Word...\n");
            Dict.UpdateWord(wordToUpdate);
            cout << "\n\t\t\t\t";
            displayBlueText("Press any key to continue :)\n");
            _getch();
            break;
        }
        case '6': {
            system("cls");
            printLogo();
            cout << "\n\t\t\t\t";
            displayYellowText("Enter characters: ");

            string word;
            char ch;
            while ((ch = _getche()) != '\r') {
                system("cls");
                printLogo();
                cout << "\t\t\t\tEnter characters : " << word;
                if (ch == '\b') {
                    if (!word.empty()) {
                        word.pop_back();
                        cout << "\b \b";
                        Dict.SuggestWords(word);
                    }
                }
                else {
                    cout << ch;
                    word = word + ch;

                    for (int i = 0; i < word.length(); i++) {
                        if (isupper(word[i])) {
                            word[i] = tolower(word[i]);
                        }
                    }

                    Dict.SuggestWords(word);
                }
            }
            cout << endl;
            string meaning = Dict.Meaning(word);
            if (!meaning.empty()) {
                cout << "\n\t\t\t\tWord: " << word << ", Meaning: " << meaning << endl;
            }
            else {
                cout << "\n\t\t\t\tWord not found in the dictionary." << endl;
            }
            cout << "\n\t\t\t\t";
            displayBlueText("Press any key to continue :)\n");
            _getch();
            break;
        }
        default:
            system("cls");
            printLogo();
            cout << "\n\t\t\t\t";
            displayRedText("Invalid Choice!\n");
            break;
        }
    }
    return 0;
}