#include <iostream>
#include <algorithm>
#include <vector>
#include <Windows.h> // for system("cls") :-)
#include <conio.h>   // _getch
#include <random>
#include <cctype>    // std::tolower

#define WIN32_LEAN_AND_MEAN

// todo: hidden_word can't be guessed if it contains more than 1 word

class Game {
public:
    std::string hidden_word = "";
    std::vector<std::string> random_word = { "giraffe", "elephant", "africa", "antarctica", "uzbekistan", "somalia", "maths", "physics" };
    std::string guess = "";

    std::vector<char> guessed_letters;
    int string_num_left = 0;
    int attempts_left = 6;
    bool play = true;
};

Game hangman;

//function prototypes
void find_occurence(std::vector<char>& container, std::string& guess, std::string& hidden_word, int& attempts_left, int& string_num_left);
void handle_guess(const std::vector<char>& container, const std::string& guess, const int& attempts_left);

std::string get_lowercase(std::string& string)
{
    std::string string1 = string;
    std::transform(string1.begin(), string1.end(), string.begin(), ::tolower);
    return string;
}

void generateWord(const std::vector<std::string>& list_of_words, std::string& hidden_word) {

    if (list_of_words.empty())
    {
        std::cout << "random word container not populated\n-------------------------" << std::endl;
        hangman.play = false;
        return;
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(0, list_of_words.size() - 1);
    int index = distrib(gen);
    hidden_word = list_of_words[index];
}

void start_game(const std::string& hidden_word, std::string& guess, const int& string_num_left, const int& attempts_left)  // first, take input
{
    std::cout << "The hidden word has " << hidden_word.length() - string_num_left << " characters left.\n";
    std::cout << "Attempts left: " << attempts_left << "\n";
    std::cout << "Guess a letter or type 0 to exit: ";
    std::cin >> guess;
    system("cls");
    handle_guess(hangman.guessed_letters, hangman.guess, hangman.attempts_left);
}

void handle_guess(const std::vector<char>& guessed_letters, const std::string& guess, const int& attempts_left) // secondly, check if the user has guessed the valid letter
{
    if (attempts_left == 1)
    {
        std::cout << "out of attempts! game over.\n----------------------" << std::endl;
        hangman.play = false;
        return;
    }

    if (guess[0] == '0')
    {
        hangman.play = false;
        return;
    }

    // check if letter is valid
    if (guess.length() != 1 || !isalpha(guess[0]))
    {
        std::cout << "Invalid input. please guess a single character.\n----------------------------" << std::endl;
        return;
    }

    // check if letter was already guessed
    if (std::find(guessed_letters.begin(), guessed_letters.end(), guess[0]) != guessed_letters.end()) // in case std::find fails, it returns guessed_letters.end(). hence the check
    {
        std::cout << "you've already guessed that letter! try again\n---------------------" << std::endl;
        return;
    }

    // in case the guess was valid, proceed to find occurence
    find_occurence(hangman.guessed_letters, hangman.guess, hangman.hidden_word, hangman.attempts_left, hangman.string_num_left);
}

void find_occurence(std::vector<char>& container, std::string& guess, std::string& hidden_word, int& attempts_left, int& string_num_left)
{
    get_lowercase(hidden_word); // modifies the original string but it doesn't really matter anyway
    // first, count the number of occurences
    auto count = std::count(hidden_word.begin(), hidden_word.end(), get_lowercase(guess).c_str()[0]);
    
    if (count == 0)
    {
        std::cout << "wrong guess! try again\n--------------------" << std::endl;
        attempts_left--;
        return;
    }
    
    auto letter = hidden_word.begin();
    for (int i = 0; i < count; i++) // loop throught the string until all occurences are found
    {
        letter = std::find(letter, hidden_word.end(), get_lowercase(guess).c_str()[0]); // get an iterator to the first occurence
        auto distance = std::distance(hidden_word.begin(), letter); // count the distance from beggining of string to that iterator
        std::cout << "letter " << guess.c_str()[0] << " detected at character " << distance+1 << std::endl;
        letter++;
        string_num_left++;
    }

    container.push_back(guess.c_str()[0]);
    
    if (string_num_left == hidden_word.length())
    {
        system("cls");
        std::cout << "you've guessed the word!" << std::endl;
        std::cout << "the word was: " << hidden_word << std::endl;
        hangman.play = false;
        return;
    }
}

void game_loop()
{
    start_game(hangman.hidden_word, hangman.guess, hangman.string_num_left, hangman.attempts_left);
}

int main() {
    generateWord(hangman.random_word, hangman.hidden_word); // having this piece of code at this place means you can guess the word only once and then the game is finished

    while (hangman.play) {
        game_loop();
    }

    std::cout << "Press any key to exit...\n";
    _getch();
}
