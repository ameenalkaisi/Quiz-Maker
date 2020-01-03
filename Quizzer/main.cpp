#include <iostream>
#include <random>
#include <ctime>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <strstream>
#include <string>
using namespace std;

const char letters[26] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l' , 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };

/*
* @brief Returns if a string is contained in another string
* @param word String that is being  looked for
* @param arr The main string
*/
const bool contains(const string &word, const string &arr)
{
	return (arr.find(word) <= arr.size());
}

/*
* @brief Returns a random number within a range - range will also contain the minimum and maximum value
* @param max  Maximum range - must be passed
* @param min Minimum range - will equal 0 if not passed
*/
const int random(const int max, const int min = 0)
{
	return (rand() % (max - min + 1) + min);
}

class Answer
{
public:
	// Constructer - initializes answer to newAnswer
	Answer(string newAnswer) : answer(newAnswer), used(false)
	{
		letter = ' ';
	}

	// checks if the other variable's answer  is the same as this one's
	const bool operator==(Answer &other)
	{
		return other.answer == answer;
	}

	// If used by ostream, give the answer
	friend ostream& operator<<(ostream &os, Answer &a)
	{
		os << a.answer;
		return os;
	}

	string answer; // The string for answer
	char letter; // Letter corrolated with answer
	bool used; // If answer has been used for something - later used for organizing answers randomly
};

class Question
{
public:
	/*
	* @brief Creates a new Question object
	* @param newQuestion Sets the question
	* @param newCorrectAnswer Sets the correct answer for this question
	* @param newAnswers Sets the answers for this question
	*/
	Question(const string &newQuestion, const string &newCorrectAnswer, const vector<string> &newAnswers)
		: question(newQuestion), used(false), wasCorrect(false), correctAnswer(newCorrectAnswer)
	{
		// Initialize the answers vector
		for (string a : newAnswers)
			answers.push_back(Answer(a));

		// Initialize the size of answersNum
		answersNum = new int[answers.size()];

		// Randomly organize answers within answersNum
		for (int i = 0; i < (int)answers.size(); i++)
		{
			int rng;
			do
				rng = random(answers.size() - 1);
			while (answers[rng].used);

			answers[rng].used = true;
			answersNum[i] = rng;
			answers[rng].letter = letters[i];
			if (answers[rng] == correctAnswer)
				correctAnswer.letter = letters[i];
		}
	}

	// Prints question
	void print()
	{
		cout << question << endl;

		for (int i = 0; i < (int)answers.size(); i++)
			cout << "(" << letters[i] << ") " << answers[answersNum[i]] << endl;
	}

	// Takes a letter and checks if it's equal to correctAnswers letter
	const bool &ask(const char &answer)
	{
		used = true; // Question is now used
		wasCorrect = answer == correctAnswer.letter; // wasCorrect will equal the same thing as the output of this function

		return answer == correctAnswer.letter;
	}

	string question; // Question
	vector<Answer> answers; // The answers of question
	Answer correctAnswer; // The correct answer of the question
	int *answersNum; // Array for randomly organizing the answers
	bool used; // If the question was used or not - later used for randomly organizing questions
	bool wasCorrect; // If the user answered correctly
};

class Test
{
public:
	// Test constructor, initialize the questions
	Test(const vector<Question> &newQuestions) : questions(newQuestions), mark(0)
	{
		// Initialize the questsNum array to the size of the questions
		questsNum = new int[questions.size()];

		// Randomly organize questions
		for (int i = 0; i < (int)questions.size(); i++)
		{
			int rng;
			do
			{
				rng = random(questions.size() - 1);
			} while (questions[rng].used);

			questions[rng].used = true;
			questsNum[i] = rng;
		}
	}

	// Run the test. Get an answer or command and execute accordingly
	void runTest()
	{
		cout << "Type ?# to  go to question of that number, and type <> when you are done.\n" << endl;

		int currentNum = 0; // The current question the user is on
		string choice; // User's choice/answer
		while (true)
		{
			choice.clear();

			// Printn the question and get input
			cout << currentNum + 1 << ". ";
			questions[questsNum[currentNum]].print();
			cout << "\n\n: ";
			getline(cin, choice);

			if (contains("?", choice)) // If input contains "?" then try to go to the  number after ?
			{
				choice.erase(choice.begin());

				int newNum;
				stringstream ss(choice);
				ss >> newNum;
				newNum--;

				if (newNum < 0 || newNum >(int)questions.size() - 1)
					cout << endl << "Question doesn't exist" << endl;
				else
					currentNum = newNum;

				cout << endl;

				continue;
			}
			else if (contains("<>", choice)) // If input contains "<>" then exit the loop
				break;
			else // Else treat input as the answer
			{
				if (!questions[questsNum[currentNum]].wasCorrect && questions[questsNum[currentNum]].ask(choice[0]))
					mark++;
				else if (questions[questsNum[currentNum]].wasCorrect && !questions[questsNum[currentNum]].ask(choice[0]))
					mark--;

				currentNum += (currentNum == (int)questions.size() - 1 ? 0 : 1);
				continue;
			}
		}
	}

	vector<Question> questions; // Questions within the test
	int *questsNum; // Array used to randomly organize questions
	double mark; // The number of questions the user got correct
};