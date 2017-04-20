#include <iostream>
#include <fstream>
#include <regex>

using namespace std;

float FPS;

string validateString (int hours, int minutes, int seconds, int miliseconds){
    //Zamiana intów na stringi, spróbować jakiejś metody z mniejszą ilością zmiennych
    ostringstream ss;
        ss << hours;
        string hours_str = ss.str();
    ostringstream ss2;
        ss2 << minutes;
        string minutes_str = ss2.str();
    ostringstream ss3;
        ss3 << seconds;
        string seconds_str = ss3.str();
    ostringstream ss4;
        ss4 << miliseconds;
        string miliseconds_str = ss4.str();
        //Sprawdzanie czy długość jest odpowiednia
        if (hours_str.length() == 1){
            hours_str = "0"+hours_str;
        }
        if (minutes_str.length() == 1){
            minutes_str = "0"+minutes_str;
        }
        if (seconds_str.length() == 1) {
            seconds_str = "0"+seconds_str;
        }
        if (miliseconds_str.length() == 1){
            miliseconds_str == "00"+miliseconds_str;
        } else {
         if (miliseconds_str.length() == 2) {
                miliseconds_str == "0"+miliseconds_str;
            }
        }
    return hours_str+":"+minutes_str+":"+seconds_str+","+miliseconds_str;
}

string fps2time (string frame) {
    int frames;
    frames = atoi(frame.c_str());
    int seconds = frames/FPS;
    int miliseconds = fmod(frames, FPS);
    int minutes = seconds / 60;
    int hours = minutes / 60;
    miliseconds = 1000/FPS;
    seconds -= minutes*60;
    minutes -= hours*60;
    return validateString(hours, minutes, seconds, miliseconds);
}

int validateInString(string str) {
    regex pattern;
    pattern = "\\{[0-9]+\\}\\{[0-9]+\\}\\w*";
    smatch result;
    if (regex_search(str, result, pattern) == 0) {
        return 0;
    }
    return -1;
}

string getStartFrame(string str) {
    int found1 = str.find('{');
    int found2 = str.find('}');
    return str.substr(found1+1, found2-1);
}

string getEndFrame(string str) {
    int found1 = str.find('}');
    int found2 = str.find('}', found1+1);
    return str.substr(found1+2, found2-2-found1);
}

string getString (string str) {
    int found1 = str.find('}');
    int found2 = str.find('}', found1+1);
    return str.substr(found2+1, str.length()-found2);
}

string getOutName (char* str_in) {
    string str_temp = (string)(str_in);
    string str_return  = str_temp.replace(str_temp.end()-3, str_temp.end(), "srt");
    return str_return;
}

string convertPipes (string str) {
    size_t found = str.find('|');
    if (found != string::npos){
                for (int i = 0; i < str.length(); ++i) {
                    if (str[i] == '|')
                    str[i] = '\n';
                }
            }
    return str;
}

int checkInputFile (char* str_in) {
    string str_temp = (string)(str_in);
    if (str_temp.substr(str_temp.length()-4, 4) != ".txt") {
        return -1;
    } else {
        return 0;
    }
}

void showError (int error_number) {
    switch (error_number) {
        case -1:
            cout << "Błąd pliku wejściowego (nie można otworzyć)\n";
            break;
        case -2:
            cout << "Błąd pliku wyjściowego (nie można otworzyć)\n";
            break;
        case -3:
            cout << "Brak podanych parametrów" << endl << endl;
            cout << "Użycie programu: txt2srt plik.txt [plik.srt] [ilość_klatek]" << endl << endl;
            cout << "Jeśli ilość klatek nie zostało podana, to przypisywana jest domyślna wartość 23.976." << endl;
            cout << "Plik wyjściowy jest domyślnie zapisywany w tym samym katalogu co plik źródłowy." << endl;
            break;
        case -4:
            cout << "Nieprawidłowy plik źródłowy" << endl;
            break;
        case -6:
            cout << "Zła wartość parametru FPS" << endl;
            break;
    }
    cin.get();
}

void showError (int error_number, int i){
    switch (error_number) {
        case -5:
            cout << "Linia numer " << i <<  " nie jest zgodna z formatem MicroDVD" << endl;
            break;
    }

}

int main(int argc, char* argv[]) {
    FPS = 23.976;
    //Sprawdzanie czy podano argument pliku źródłowego
    if (argc < 2) {
        showError(-3);
        return -1;
    } else if (checkInputFile(argv[1]) == -1){ //Sprawdzanie czy plik źródłowy ma rozszerzenie .txt
        showError(-4);
        return -1;
    } else {
        if (argc == 4) {    //Ustawianie FPSów
            float fps_temp = atof(argv[3]);
            if (fps_temp != 0) {
            FPS = fps_temp;
            cout << "Ilość klatek ustawiono na " << FPS << endl ;
            } else {
                showError(-6);
                return -1;
            }
        }
        string str;
        ifstream in;
        ofstream out;
        //Wczytywanie pliku .txt i sprawdzanie czy jest good
        in.open(argv[1]);
        if( !in.good() ){
            showError (-1);
            return -1;
        } else {
            cout << "Pomyślnie otworzono plik wejściowy" << endl;
        }
        //Wczytywanie pliku .srt i sprawdzanie go
        if (argc > 2) {
            out.open(argv[2] , ofstream::out | ofstream::trunc );
        } else {
            out.open(getOutName(argv[1]), ofstream::out | ofstream::trunc );
        }
        if( !out.good() ){
            showError (-2);
            return -1;
        } else {
            cout << "Pomyślnie otworzono plik wyjściowy" << endl;
        }
        //Wyświtlanie informacji
        cout << "Plik wejściowy: " << argv[1] << endl;
        if (argc > 2) {
            cout << "Plik wyjściowy: " << argv[2] << endl;
        } else {
            cout << "Plik wyjściowy: " << getOutName(argv[1]) << endl;
        }
        cout << "Ilość klatek: " << FPS << endl;
        //Główna pętla przetwarzająca
        auto i = 0;
        while (!in.eof()){
            i++;
            getline(in, str);
            str = convertPipes(str);
            if (str.length() > 7) {
                    if (validateInString(str) == -1) {
                        out << i << endl << fps2time(getStartFrame(str)) << " --> " << fps2time(getEndFrame(str)) << endl << getString(str) << endl << endl;
                    } else {
                        showError(-5, i);
                        return -1;
                    }
            }
        }
        out.flush();
        out.close();
        cout << "Pomyślnie przekonwertowano plik " << argv[1] << " (" << i << " lini)" << endl;
    }
    return 0;
}
