#include "UI.h"
#include "HTMLsource.h"
#include "Lexer.h"
#include "Parser.h"
#include <algorithm> 
#include <string> 
UI::UI()
{
}

UI::~UI()
{
}

void UI::startUI() 
{
	cout << "Witaj w programie oczyszczajacym plik html ze stylow i formatowan" << endl;
	showHelp();
	char task;

	while (true)
	{
		cin.clear();
		fflush(stdin);
		cout << "Wybierz opcje: ";
		cin >> task;

		switch (task)
		{
		case '0':
			return;
		case '1':
		{
			getFilename();
			Lexer lexer(filename);
			Token t;
			do{
				 t = nextTokenForUI(lexer);
				 cout << "typ " << t.getType() << " value " << t.getValue() << endl;
			} while (t.getType() != ENDOFFILE && getchar() != 'q');
			
			break;
		}
		case '2':
		{
			getFilename();

			Lexer lexer(filename);
			Parser parser(&lexer);

			parser.parse();
			parser.printTree();

			break;
		}
		case '3':
		{
			getFilename();

			Lexer lexer(filename);
			Parser parser(&lexer);
			parser.parse();

			size_t pos = filename.find(".txt");
			fileToWrite = filename.substr(0, pos);

			ofstream hfile(fileToWrite + "clean" + ".txt");
			if (!hfile.is_open())
			{
				std::cout << "Coul not open" << filename << "clean.txt file!" << endl;
				break;
			}
			else{
				hfile.close();
				HTMLclean hFile(fileToWrite + "clean" + ".txt");
				parser.getTreeRoot().writeWithoutStyles(&hFile);
				cout << "HTML - without styles file was successfully writen out." << endl;
			}
			break;
		}
		case '4':
		{
			getFilename();
			Lexer lexer(filename);
			Parser parser(&lexer);
			parser.parse();
			parser.getTreeRoot().createNestsCollection();
			break;
		}
		default:
			cout << "Bledna opcja, wybierz ponownie:" << endl;
			showHelp();
			break;
		}
		
	}
}
int UI::getFilename()
{
	filename = "";
	cout << "Podaj nazwe pliku: ";
	cin >> filename;
	
	ifstream f(filename, ifstream::in);
	if (!f.is_open())
	{
		cout << "Plik " << filename << " nie moze zostac odczytany lub nie istnieje" << endl;
		return 1;
	}
	f.clear();
	f.close();
	cout << "Plik zostal odczytany" << endl;
	return 0;
}
void UI::showHelp()
{
	cout << "Wybierz opcje:" << endl;
	cout << " 1 Czytaj plik HTML, i wyswietl tokeny\n 2 Pokaz document object model\n 3 Zbuduj plik wyjsciowy\n 4 Pokaz zbior zagniezdzen\n 0 - Wyjscie z programu" << endl;
}
void UI::generateOutputFile(string htmlFileName)
{
	filename = htmlFileName;
}

Token UI::nextTokenForUI(Lexer& lexer){
	Token t;

	if (previousToken3.getType() == OPEN && previousToken2.getValue() == "script" && previousToken.getType() == CLOSE) {
		t = lexer.getScript();

		if (t.getType() == EMPTY){
			t = lexer.nextToken();
		}
		if (t.getType() == UNDEFINED){
			eh.errif("Blad!: ", t, true, 2);
		}

		previousToken3 = previousToken2;
		previousToken2 = previousToken;
		previousToken = t;
		return t;
	}
	else if (previousToken.getType() == CLOSE || previousToken.getType() == CLOSE_EMPTY || previousToken.getType() == COMMENT) {
		t = lexer.getText();

		if (t.getType() == EMPTY){
			t = lexer.nextToken();
		}
		if (t.getType() == UNDEFINED){
			eh.errif("Blad!: ", t, true, 2);
		}

		previousToken3 = previousToken2;
		previousToken2 = previousToken;
		previousToken = t;
		return t;
	}

	t = lexer.nextToken();

	if (t.getType() == UNDEFINED){
		eh.errif("Blad!: ", t, true, 2);
	}
	else if (t.getType() == EMPTY){
		t = lexer.nextToken();
		if (t.getType() == UNDEFINED){
			eh.errif("Blad!: ", t, true, 2);
		}
	}
	else if (t.getType() == ENDOFFILE){
		return t;
	}
	previousToken3 = previousToken2;
	previousToken2 = previousToken;
	previousToken = t;
	return t;
}