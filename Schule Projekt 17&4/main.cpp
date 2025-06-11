#include <iostream>
#include <conio.h>
#include <time.h>
#include <windows.h>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

using namespace std;

bool gezogen[32] = {false}; //Array von 32 karten sind false

// class fungiert wie ein Main Baustein
class Karte // class Karte dient zur erstellung einer Karte
{

public: // Public da wenn man es auf private stellt kann man diese nicht mehr außerhalb der Klasse aufrufen
    int zahl;
    string name, art, name2;

    Karte(int z, string n, string a, string n2) // Das ist der construktor
    {
        zahl = z;
        name = n;
        art = a;
        name2 = n2;
    }
};

// Typ der Variable wie int usw. => dann die Variable = Karte danach folgt Variable
//[32] ist ein Array und das ist eine Liste
Karte karten[32] =
{
    // Sieben
    Karte(7, "Sieben", "\u2665", "7"), // Herz (Unicode: U+2665)
    Karte(7, "Sieben", "\u2666", "7"), // Karo (Unicode: U+2666)
    Karte(7, "Sieben", "\u2663", "7"), // Kreuz (Unicode: U+2663)
    Karte(7, "Sieben", "\u2660", "7"), // Pik (Unicode: U+2660)

    // Acht
    Karte(8, "Acht", "\u2665", "8"),
    Karte(8, "Acht", "\u2666", "8"),
    Karte(8, "Acht", "\u2663", "8"),
    Karte(8, "Acht", "\u2660", "8"),

    // Neun
    Karte(9, "Neun", "\u2665", "9"),
    Karte(9, "Neun", "\u2666", "9"),
    Karte(9, "Neun", "\u2663", "9"),
    Karte(9, "Neun", "\u2660", "9"),

    // Zehn
    Karte(10, "Zehn", "\u2665", "10"),
    Karte(10, "Zehn", "\u2666", "10"),
    Karte(10, "Zehn", "\u2663", "10"),
    Karte(10, "Zehn", "\u2660", "10"),

    // Bube (Wert = 2)
    Karte(2, "Bube", "\u2665", "B"),
    Karte(2, "Bube", "\u2666", "B"),
    Karte(2, "Bube", "\u2663", "B"),
    Karte(2, "Bube", "\u2660", "B"),

    // Dame (Wert = 3)
    Karte(3, "Dame", "\u2665", "D"),
    Karte(3, "Dame", "\u2666", "D"),
    Karte(3, "Dame", "\u2663", "D"),
    Karte(3, "Dame", "\u2660", "D"),

    // König (Wert = 4)
    Karte(4, "König", "\u2665", "K"),
    Karte(4, "König", "\u2666", "K"),
    Karte(4, "König", "\u2663", "K"),
    Karte(4, "König", "\u2660", "K"),

    // Ass (Wert = 11)
    Karte(11, "Ass", "\u2665", "A"),
    Karte(11, "Ass", "\u2666", "A"),
    Karte(11, "Ass", "\u2663", "A"),
    Karte(11, "Ass", "\u2660", "A")
};

string laengeEingabe(int maxLen)//c ist die Nutzereingabe und wird überprüft auf länge, enter und entfernen
{
    string s;
    char c;
    while ((c = getch()) != 13) //Schleife läuft, bis Enter gedrückt wird (13 ist das zeichen für Enter)
    {
        if (c == 8 && !s.empty()) //Wenn die Backspace-Taste (8 in Ascii) gedrückt wurde und der String s nicht leer ist
        {
            cout << "\b \b"; //bewegt cursor zurück
            s.pop_back(); //entfernt das eingegebene zeichen
        }
        else if ((int)s.size() < maxLen && c >= 32 && c <= 126) //erste hälfte überprüft die länge und die zweite gibt nur zeichen aus die druckbar sind
        {
            cout << c;
            s += c; //Wird am string angehängt
        }
    }
    return s;
}
string rahmenzeile(const string& text, int gesamtbreite ) //Name der Zeile, länge
{
    int inhalt_breite = gesamtbreite - 2; // -2 für '║'

    // Text abschneiden, falls zu lang
    string zeile = text.substr(0, inhalt_breite);

    // Mit Leerzeichen auffüllen
    zeile += string(inhalt_breite - zeile.length(), ' ');

    return "║" + zeile + "║\n";
}

void falscheEingabe()
{
    cout << "\n\n\n╔══════════════════════════════════════════╗\n";
    cout << "║Falsche Eingabe bitte erneut versuchen!   ║\n";
    cout << "║Weiter mit Taste                          ║\n";
    cout << "╚══════════════════════════════════════════╝";

}

void clearLines(int anzahl)
{
    for (int i = 0; i < anzahl; ++i) //zählt solange hoch bis er Anzahl erreicht
        cout << "\033[F\033[K"; //\033[F zieht den cursor nach oben << \033[K löscht die Zeile
    cout << flush; //Puffer wird sofort ausgegeben
}

class Spieler
{
public: // Public: = da wir diese außerhalb der class benutzten
    vector<string> lines;
    vector<Karte> gezogeneKarten;
    string name;
    int punkteAbfrage, gewinne;
    bool fertig, uebergezogen, dealer;

    Spieler() // Constructor stellt für die Spieler die auf false und 0
    {
        punkteAbfrage = 0;
        fertig = false;
        uebergezogen = false;
        gewinne = 0;
    }

    // Generiert eine Zufallskarte aus 32 Karten die wir oben erstellt haben
    Karte zufallsKarte()
    {
        int zufall;

        do
        {
            zufall = rand() % 31; // Zufallszahl zwischen 0 und 31
        }

        while (gezogen[zufall]); // Solange die Karte schon gezogen wurde, versuche eine neue

        gezogen[zufall] = true; // Karte als gezogen markieren
        return karten[zufall];
    }

    void zieheKarte()
    {
        Karte varZufallsKarte = zufallsKarte(); // Karte  varZufallsKarte = weil zufallskarte (typ) ist Karte!
        punkteAbfrage += varZufallsKarte.zahl;  // Wir wollen das die Zufallskarte konstant ist
        cout << name << " zieht die Karte: " << varZufallsKarte.name << " " << varZufallsKarte.art << " [Wert " << varZufallsKarte.zahl << "]\n";

        // Speichere die gezogene Karte im gespeicherten Array
        gezogeneKarten.push_back(varZufallsKarte); //Pushback fügt die karte am ende des vectors hinzu
        PlaySound(TEXT("ZieheKarte.wav"), NULL, SND_FILENAME | SND_ASYNC);
        zeigeGezogeneKarten();
        cout << "Der Punktestand beträgt: " << punkteAbfrage;
        cout << "\n\n";

    }

    // Hier werden die Runden gespielt und auf überziehen geprüft und auf 21 gesprüft
    void spielen()
    {
        string frage;

        while(true)
        {

            cout << name << " [" << punkteAbfrage << "] " << " Hit or Stay [H]|[S]: ";
            frage = laengeEingabe(1);
            cout << "\n";
            if(frage == "h" || frage == "H")
            {
                cout << "\n";
                zieheKarte();
                ueberzogen();
                wincon();
                break;
            }

            else if (frage == "S" || frage == "s")
            {

                fertig = true;
                zeigeGezogeneKarten();
                cout << "\n";
                break;
            }

            else
            {
                falscheEingabe();
                getch();
                cout << "\n";
                clearLines(8);
                continue;
            }
        }
    }

    // Hier wird überprüft ob man mehr als 21 hat
    void ueberzogen()
    {
        if (punkteAbfrage > 21)
        {
            cout << "╔═════════════════════════════════════════════╗\n";
            string zeile3 = name + " hat überzogen und verliert die Runde";
            cout << rahmenzeile(zeile3,48);
            cout << "╚═════════════════════════════════════════════╝\n\n";
            uebergezogen = true;
            fertig = true;
        }
    }

    // Hier wird überprüft ob man 21 hat
    void wincon()
    {
        if (punkteAbfrage == 21)
        {


            fertig = true;
        }
    }

    // Wie Punkteabfrage werden hier nur gewinne zusammengerechnet
    void gewinnKonto()
    {
        gewinne += 1;
        fertig = true;
    }

    // Das ist resettet die punkte am Ende jeder Runde und setzt Überziehen und fertig wieder auf falsch
    void resetPunkte()
    {
        punkteAbfrage = 0;
        fertig = false;
        uebergezogen = false;
        gezogeneKarten.clear();
    }

    // Dealer AI
    void dealerCheck(Spieler gegner)
    {
        // Wenn Gegner weniger Punkte hat und fertig ist oder wenn dealer mehr als 17 , dann fertig
        if (punkteAbfrage >= gegner.punkteAbfrage && (gegner.fertig || punkteAbfrage > 17))
        {
            fertig = true;
        }
        else
        {
            zieheKarte();
            ueberzogen();
        }
    }

    // Zeige alle gezogenen Karten des Spielers an
    void zeigeGezogeneKarten()
    {
        vector<vector<string>> alleKartenZeilen; //  ist eine Liste von Listen von Strings, also eine Tabelle oder Sammlung von Zeilen mit mehreren Texten.

        for (const Karte& karte : gezogeneKarten) // (& verhindert das eine kopie angelegt wird bei jeden durchlauf) Geht alle karten durch die gezogen wurden
        {
            vector<string> lines;


            if (karte.zahl == 7)
            {
                lines =
                {
                    "╭───────────╮",
                    "| " + to_string(karte.zahl) + "         |", // to string um den int zahl zum string zu machen
                    "| " + karte.art + "       " + karte.art + " |",
                    "|     " + karte.art + "     |",
                    "| " + karte.art + "       " + karte.art + " |",
                    "|           |",
                    "| " + karte.art + "       " + karte.art + " |",
                    "|         " + to_string(karte.zahl) + " |",
                    "╰───────────╯"
                };
            }
            else if (karte.zahl == 8)
            {
                lines =
                {
                    "╭───────────╮",
                    "| " + to_string(karte.zahl) + "         |",
                    "| " + karte.art + "       " + karte.art + " |",
                    "|     " + karte.art + "     |",
                    "| " + karte.art + "       " + karte.art + " |",
                    "|     " + karte.art + "     |",
                    "| " + karte.art + "       " + karte.art + " |",
                    "|         " + to_string(karte.zahl) + " |",
                    "╰───────────╯"
                };
            }
            else if (karte.zahl == 9)
            {
                lines =
                {
                    "╭───────────╮",
                    "| " + to_string(karte.zahl) + "         |",
                    "| " + karte.art + "       " + karte.art + " |",
                    "| " + karte.art + "       " + karte.art + " |",
                    "|     " + karte.art + "     |",
                    "| " + karte.art + "       " + karte.art + " |",
                    "| " + karte.art + "       " + karte.art + " |",
                    "|         " + to_string(karte.zahl) + " |",
                    "╰───────────╯"
                };
            }
            else if (karte.zahl == 10)
            {
                lines =
                {
                    "╭───────────╮",
                    "| " + to_string(karte.zahl) + "        |",
                    "| " + karte.art + "       " + karte.art + " |",
                    "| " + karte.art + "   " + karte.art + "   " + karte.art + " |",
                    "|     " + karte.art + "     |",
                    "| " + karte.art + "       " + karte.art + " |",
                    "| " + karte.art + "       " + karte.art + " |",
                    "|        " + to_string(karte.zahl) + " |",
                    "╰───────────╯"
                };
            }
            else if (karte.zahl == 2) // Bube
            {
                lines =
                {
                    "╭───────────╮",
                    "| " + karte.name2 + "       " + karte.art + " |",
                    "|   ╔══╗    |",
                    "|   ║   ║   |",
                    "|   ║══╣    |",
                    "|   ║   ║   |",
                    "|   ╚══╝    |",
                    "|" + karte.art + "        " + karte.name2 + " |",
                    "╰───────────╯"
                };
            }
            else if (karte.zahl == 3) // Dame
            {
                lines =
                {
                    "╭───────────╮",
                    "| " + karte.name2 + "       " + karte.art + " |",
                    "|   ╔══╗    |",
                    "|   ║   ║   |",
                    "|   ║    ║  |",
                    "|   ║   ║   |",
                    "|   ╚══╝    |",
                    "|" + karte.art + "        " + karte.name2 + " |",
                    "╰───────────╯"
                };
            }
            else if (karte.zahl == 4) // König
            {
                lines =
                {
                    "╭───────────╮",
                    "| " + karte.name2 + "       " + karte.art + " |",
                    "|   ║   ║   |",
                    "|   ║  ║    |",
                    "|   ║═╣     |",
                    "|   ║  ║    |",
                    "|   ║   ║   |",
                    "|" + karte.art + "        " + karte.name2 + " |",
                    "╰───────────╯"
                };
            }
            else if (karte.zahl == 11) // Ass
            {
                lines =
                {
                    "╭───────────╮",
                    "| " + karte.name2 + "       " + karte.art + " |",
                    "|   ╔═══╗   |",
                    "|   ║   ║   |",
                    "|   ╠═══╣   |",
                    "|   ║   ║   |",
                    "|   ║   ║   |",
                    "|" + karte.art + "        " + karte.name2 + " |",
                    "╰───────────╯"
                };
            }

            alleKartenZeilen.push_back(lines); // fügt die gezogenen Karten der Funktion hinten an
        }


        // Nun werden alle Karten nebeneinander ausgegeben
        for (int zeile = 0; zeile < 9; ++zeile) // Karten sind 9 hoch gibt von 0 - 8 aus da zeile 1 zeile 0 ist
        {
            for (const auto &karteZeilen : alleKartenZeilen) // : speichert die zeilen in der funktion
            {
                cout << karteZeilen[zeile] << " "; //gibt alle zeilen aus
            }
            cout << "\n";
        }
    }



};


void setCursorPosition(int x, int y)
{
    COORD cord;
    cord.X = x;
    cord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cord); // ist ne funktion die den cursor auf die funktion coord setzt

}


void menue()
{
    cout << "╔═══════════════════════════════════════════════════════════╗\n";
    cout << "║ Willkommen zu 17 & 4                                      ║\n";
    cout << "║-----------------------------------------------------------║\n";
    cout << "║ Best of One   [1]      Best of Five  [5]                  ║\n";
    cout << "║                                                           ║\n";
    cout << "║ Best of Three [3]      Quit          [x]                  ║\n";
    cout << "║                                                           ║\n";
    cout << "║ Regelwerk     [r]                                         ║\n";
    cout << "║                                                           ║\n";
    cout << "║ Bitte wählen Sie den gewünschten Modus aus:               ║\n";
    cout << "║                                              ‾‾‾          ║\n";
    cout << "╚═══════════════════════════════════════════════════════════╝\n";
}

void playervsdealer()
{
    cout << "╔═════════════════════════════════════════════════╗\n";
    cout << "║ Player vs Dealer  [d]   Player vs Player  [p]   ║\n";
    cout << "║                                                 ║\n";
    cout << "║ Zurück zum Menü   [b]                           ║\n";
    cout << "║                                                 ║\n";
    cout << "║ Meine Eingabe:                                  ║\n";
    cout << "║                 ‾‾‾                             ║\n";
    cout << "╚═════════════════════════════════════════════════╝";
}

void weiter()
{
    cout << "╔════════════════════╗\n";
    cout << "║ Weiter mit Taste!  ║\n";
    cout << "╚════════════════════╝\n\n";
}





int main()
{
    SetConsoleOutputCP(CP_UTF8);
    string eingabe, name;
    int gewinneNoetig;
    system("color A");
    srand(time(0));

    cout << "██╗    ██╗███████╗██╗      ██████╗ ██████╗ ███╗   ███╗███████╗\n";
    cout << "██║    ██║██╔════╝██║     ██╔════╝██╔═══██╗████╗ ████║██╔════╝\n";
    cout << "██║ █╗ ██║█████╗  ██║     ██║     ██║   ██║██╔████╔██║█████╗\n";
    cout << "██║███╗██║██╔══╝  ██║     ██║     ██║   ██║██║╚██╔╝██║██╔══╝\n";
    cout << "╚███╔███╔╝███████╗███████╗╚██████╗╚██████╔╝██║ ╚═╝ ██║███████╗\n";
    cout << " ╚══╝╚══╝ ╚══════╝╚══════╝ ╚═════╝ ╚═════╝ ╚═╝     ╚═╝╚══════╝\n\n\n";

    cout << "████████╗ ██████╗      ██╗███████╗       ██╗       ██╗  ██╗\n";
    cout << "╚══██╔══╝██╔═══██╗    ███║╚════██║       ██║       ██║  ██║\n";
    cout << "   ██║   ██║   ██║    ╚██║    ██╔╝    ████████╗    ███████║\n";
    cout << "   ██║   ██║   ██║     ██║   ██╔╝     ██╔═██╔═╝    ╚════██║\n";
    cout << "   ██║   ╚██████╔╝     ██║   ██║      ██████║           ██║\n";
    cout << "   ╚═╝    ╚═════╝      ╚═╝   ╚═╝      ╚═════╝           ╚═╝\n";

    getch();



    // Main Schleife
    while (true)
    {

        bool quit = false, akzeptiert = false, zurueck = false;
        system("cls");
        // Intro Modus auswahl nur Visuell
        menue();


        // Bof1, Bof3, Bof5 abfrage
        while (quit == false && akzeptiert == false) // Solange keine akzeptierte eingabe getätigt wird oder man verlassen möchte
        {

            setCursorPosition(48, 9);
            eingabe = laengeEingabe(1);


            if(eingabe == "1")
            {

                akzeptiert = true;
                gewinneNoetig = 1;
            }

            else if(eingabe == "3")
            {

                akzeptiert = true;
                gewinneNoetig = 2;
            }

            else if(eingabe == "5")
            {

                akzeptiert = true;
                gewinneNoetig = 3;
            }

            else if (eingabe == "r" || eingabe == "R")
            {
                akzeptiert = true;
            }

            else if (eingabe == "x" || eingabe == "X")
            {
                cout << "\n\n\n╔═══════════════════════╗\n";
                cout << "║Schönen Tag noch!      ║\n";
                cout << "╚═══════════════════════╝";
                quit = true;
                getch();
            }

            else
            {

                cout << "\n";
                falscheEingabe();
                getch();
                system("cls");
                menue();

            }

        }

        if (quit == true) // Um aus der Main schleife raus wenn jemand x drückt
        {
            break;
        }


        if (eingabe == "R" || eingabe == "r")
        {
            system("cls");
            cout << "╔══════════════════════════════════════════════════════════════════════════════╗\n";
            cout << "║                            SPIELREGELN – 17 UND 4                            ║\n";
            cout << "╠══════════════════════════════════════════════════════════════════════════════╣\n";
            cout << "║ Ziel:                                                                        ║\n";
            cout << "║ Mit zwei oder mehr Karten möglichst nahe an 21 Punkte kommen.                ║\n";
            cout << "║ Wer 21 überschreitet, verliert automatisch die Runde.                        ║\n";
            cout << "║                                                                              ║\n";
            cout << "║ Kartenwerte:                                                                 ║\n";
            cout << "║ 7 – 7 Punkte     8 – 8 Punkte     9 – 9 Punkte     10 – 10 Punkte            ║\n";
            cout << "║ Bube – 2         Dame – 3         König – 4         Ass – 11 Punkte          ║\n";
            cout << "║                                                                              ║\n";
            cout << "║ Spielablauf:                                                                 ║\n";
            cout << "║ 1. Beide Spieler ziehen 1 Karte.                                             ║\n";
            cout << "║ 2. Abwechselnd kann jeder Spieler weitere Karten ziehen oder passen.         ║\n";
            cout << "║ 3. Das Spiel endet, wenn beide gepasst haben oder jemand über 21 ist.        ║\n";
            cout << "║ 4. Gewinner ist, wer näher an 21 ist – ohne zu überziehen.                   ║\n";
            cout << "║ 5. Gleichstand ergibt ein Unentschieden.                                     ║\n";
            cout << "╚══════════════════════════════════════════════════════════════════════════════╝\n";

            cout << "\n\n╔════════════════════════════════════════╗\n";
            cout << "║Durch Tastendruck zurück zum Hauptmenü  ║\n";
            cout << "╚════════════════════════════════════════╝\n";
            getch();
            continue;
        }


        // Spieler Erstellt mit der Klasse über dem Main block und nach der eingabe der Spielernamen gefragt
        Spieler spieler1 = Spieler(); // Spieler = sowas wie int,string  spieler1 = Variable  Spieler() = class
        Spieler spieler2 = Spieler(); //(Funktion innerhalb einer class werden Methode genannt, aber nur Formsache)

        // Player or Bot

        cout << "\n\n\n";
        playervsdealer();

        akzeptiert = false;
        while (akzeptiert == false)
        {

            setCursorPosition(19, 17);
            eingabe = laengeEingabe(1);

            if (eingabe == "d" || eingabe == "D")
            {
                spieler2.dealer = true;
                akzeptiert = true;
                system("cls");
            }

            else if (eingabe ==  "p" || eingabe == "P")
            {

                spieler2.dealer = false;
                akzeptiert = true;
                system("cls");
            }

            else if (eingabe == "b")
            {
                akzeptiert = true;
                system("cls");
            }

            else
            {
                falscheEingabe();

                getch();
                cout << "\n";
                clearLines(12);
                playervsdealer();

            }
        }

        if(eingabe == "b")
        {
            continue;
        }



        // Spielernamen
        cout << "╔══════════════════════════════════════════╗\n";
        cout << "║Bitte Namen eingeben!                     ║\n";
        cout << "╚══════════════════════════════════════════╝\n";

        cout << "╔══════════════════════════════╗\n";
        cout << "║Spieler 1 Name:               ║\n";
        cout << "║                ‾‾‾‾‾‾        ║\n";
        cout << "╚══════════════════════════════╝\n\n";


        while(true)
        {


            setCursorPosition(17, 4);
            name = laengeEingabe(6);

            if (name.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz") != string::npos)
            {
                falscheEingabe();

                getch();
                cout << "\n";
                clearLines(8);
                cout << "╔══════════════════════════════╗\n";
                cout << "║Spieler 1 Name:               ║\n";
                cout << "║                ‾‾‾‾‾‾        ║\n";
                cout << "╚══════════════════════════════╝\n\n";

                continue;


            }
            else if (name.empty())
            {
                cout << "\n\n\n╔══════════════════════════════╗\n";
                cout << "║Name darf nicht leer sein.    ║\n";
                cout << "║Weiter mit Taste!             ║\n";
                cout << "╚══════════════════════════════╝\n";
                getch();
                cout << "\n";
                clearLines(9);
                cout << "╔══════════════════════════════╗\n";
                cout << "║Spieler 1 Name:               ║\n";
                cout << "║                ‾‾‾‾‾‾        ║\n";
                cout << "╚══════════════════════════════╝\n\n";
                continue;
            }

            else
            {
                break; // Name ist gültig -> Schleife verlassen
            }

        }


        // spieler1 wurde reihe davor kreeirt und ruft name auf welcher in der class Spieler() steht
        spieler1.name = name;


        if (spieler2.dealer == false)
        {
            cout << "\n\n\n╔══════════════════════════════╗\n";
            cout << "║Spieler 2 Name:               ║\n";
            cout << "║                ‾‾‾‾‾‾        ║\n";
            cout << "╚══════════════════════════════╝\n\n";

            while(true)
            {


                setCursorPosition(17, 8);
                name = laengeEingabe(6);

                if (name.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz") != string::npos)
                {
                    falscheEingabe();

                    getch();
                    cout << "\n";
                    clearLines(8);
                    cout << "╔══════════════════════════════╗\n";
                    cout << "║Spieler 2 Name:               ║\n";
                    cout << "║                ‾‾‾‾‾‾        ║\n";
                    cout << "╚══════════════════════════════╝\n\n";

                    continue;


                }
                else if (name.empty())
                {
                    cout << "\n\n\n╔══════════════════════════════╗\n";
                    cout << "║Name darf nicht leer sein.    ║\n";
                    cout << "║Mit Taste bestätigen!         ║\n";
                    cout << "╚══════════════════════════════╝\n";
                    getch();
                    cout << "\n";
                    clearLines(9);
                    cout << "╔══════════════════════════════╗\n";
                    cout << "║Spieler 2 Name:               ║\n";
                    cout << "║                ‾‾‾‾‾‾        ║\n";
                    cout << "╚══════════════════════════════╝\n\n";
                    continue;
                }
                else
                {
                    break; // Name ist gültig -> Schleife verlassen
                }

            }
            spieler2.name = name;
        }
        else
        {
            spieler2.name = "Dealer";
        }

        system("cls");


        // Die ersten Karten werden ausgegeben
        // Schleife läuft solange wie die benötigten gewinne nicht erreicht werden von spieler1 oder spieler2
        while (spieler1.gewinne != gewinneNoetig && spieler2.gewinne != gewinneNoetig)
        {
            spieler1.zieheKarte(); // Klammern bleiben leer, weil keine spezifischen Parameter abegfragt werden
            spieler2.zieheKarte(); // Hier werden für beide zu beginnt karten ausgegeben zieheKarte siehe class Spieler()
            weiter();
            getch();
            system("cls");


            // Hier spielen beide Spieler bis beide fertig sind und es wird überprüft ob diese fertig sind
            while (spieler1.fertig == false || spieler2.fertig == false)
            {

                if (spieler1.fertig == false)
                {
                    spieler1.spielen(); // stay = fertig siehe in Spieler() bei stay

                    if (spieler1.uebergezogen == true) // fertig bei Ueberzogen siehe class Spieler() ueberzogen
                    {
                        spieler2.gewinnKonto(); // Ein sieg wird berechnet für den anderen siehe class Spieler() gewinnKonto
                        break;
                    }
                }

                if (spieler2.fertig == false)
                {
                    // Wenn dealer dann spiel automatisch und wenn nicht dann spielen()
                    if (spieler2.dealer)
                    {
                        spieler2.dealerCheck(spieler1);
                    }
                    else
                    {
                        spieler2.spielen();
                    }

                    if (spieler2.uebergezogen == true)
                    {
                        spieler1.gewinnKonto();
                        break;
                    }
                }

                if(spieler1.fertig == false || spieler2.fertig == false)
                {
                weiter();
                getch();
                cout << "\n";
                clearLines(29);


                cout << "╔══════════════════════════════════════════════════╗\n";
                cout << "║Punktestand                                       ║\n";
                cout << "║--------------------------------------------------║\n";
                string zeile1 =  spieler1.name + " hat den Punktestand von [" + to_string(spieler1.punkteAbfrage) + "]";
                cout << rahmenzeile(zeile1,52);
                string zeile2 =  spieler2.name + " hat den Punktestand von [" + to_string(spieler2.punkteAbfrage) + "]";
                cout << rahmenzeile(zeile2,52);
                cout << "╚══════════════════════════════════════════════════╝\n\n";
                weiter();
                getch();
                system("cls");
                }
            }


            // Sieger der Runde
            if (spieler1.punkteAbfrage < 22 && spieler2.punkteAbfrage < 22)
            {
                if (spieler1.punkteAbfrage > spieler2.punkteAbfrage)
                {
                    cout << "╔═════════════════════════════════════════════════════════════════╗\n";
                    string zeile11 = spieler1.name + " [" +  to_string(spieler1.punkteAbfrage) + "]" + " Hat mehr Punkte als " + spieler2.name + " [" + to_string(spieler2.punkteAbfrage) + "]" + " und gewinnt die Runde";
                    cout << rahmenzeile(zeile11, 67);
                    cout << "╚═════════════════════════════════════════════════════════════════╝\n";
                    spieler1.gewinnKonto();


                }

                if (spieler2.punkteAbfrage > spieler1.punkteAbfrage)
                {
                    cout << "╔═════════════════════════════════════════════════════════════════╗\n";
                    string zeile10 = spieler2.name + " [" +  to_string(spieler2.punkteAbfrage) + "]" + " Hat mehr Punkte als " + spieler1.name + " [" + to_string(spieler1.punkteAbfrage) + "]" + " und gewinnt die Runde";
                    cout << rahmenzeile(zeile10, 67);
                    cout << "╚═════════════════════════════════════════════════════════════════╝\n";
                    spieler2.gewinnKonto();


                }

                if (spieler1.punkteAbfrage == spieler2.punkteAbfrage)
                {
                    cout << "╔══════════════════════════════════════════════════════════════════════════════════════════════════╗\n";
                    string zeile9 = spieler1.name + " [" + to_string(spieler1.punkteAbfrage) + "]" + " Hat gleich viele Punkte wie " + spieler2.name + " [" + to_string(spieler2.punkteAbfrage) + "]" + ", die Runde ist unentschieden und keiner gewinnt!";
                    cout << rahmenzeile(zeile9,100);
                    cout << "╚══════════════════════════════════════════════════════════════════════════════════════════════════╝\n\n";
                    weiter();
                    getch();
                    break;
                }
            }


            // Die Punkte werden für die nächste Runde zurückgesetzt
            spieler1.resetPunkte();
            spieler2.resetPunkte();



            weiter();
            getch();
            system("cls");

            cout << "╔══════════════════════════════════════════════════╗\n";
            cout << "║Gewinne                                           ║\n";
            cout << "║--------------------------------------------------║\n";
            string zeile4 =  spieler1.name + " gewonnene Runden [" + to_string(spieler1.gewinne) + "]";
            cout << rahmenzeile(zeile4,52);
            string zeile5 =  spieler2.name + " gewonnene Runden [" + to_string(spieler2.gewinne) + "]";
            cout << rahmenzeile(zeile5,52);
            cout << "╚══════════════════════════════════════════════════╝\n\n";
            weiter();
            getch();
            system("cls");
        }
    }
}
