#include <iostream>
#include <windows.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

struct Adresat
{
    int idGlobalne, idUzytkowe, idWlasciciela;
    string imie, nazwisko, email, adres, numer_tel;
};

struct Uzytkownik
{
    int id;
    string nazwa, haslo;
};



Uzytkownik PorzadkowanieDanychUzytkownika (string kompletDanychUzytkownika)
{
    Uzytkownik uzytkownik;
    string informacja="";
    int numerInformacji=1;

    for (int pozycjaZnaku=0; pozycjaZnaku<kompletDanychUzytkownika.length(); pozycjaZnaku++)
    {
        if (kompletDanychUzytkownika[pozycjaZnaku] != '|')
        {
            informacja+=kompletDanychUzytkownika[pozycjaZnaku];
        }
        else
        {
            switch (numerInformacji)
            {
            case 1:
                uzytkownik.id = atoi (informacja.c_str());
                break;
            case 2:
                uzytkownik.nazwa= informacja;
                break;
            case 3:
                uzytkownik.haslo= informacja;
                break;
            }
            informacja="";
            numerInformacji++;
        }
    }
    return uzytkownik;
}




void PobranieListyUzytkownikowZPliku (vector <Uzytkownik>&uzytkownicy)
{
    Uzytkownik nowaOsoba;
    string kompletDanychNowejOsoby="";

    fstream plik;
    plik.open("Uzytkownicy.txt",ios::in);

    if (plik.good()==true)
    {
        while (getline(plik, kompletDanychNowejOsoby))
        {
            nowaOsoba=PorzadkowanieDanychUzytkownika(kompletDanychNowejOsoby);
            uzytkownicy.push_back(nowaOsoba);
        }
        plik.close();
    }
}




Adresat PorzadkowanieDanychAdresata (string kompletDanychAdresata)
{
    Adresat adresat;
    string informacja="";
    int numerInformacji=1;

    for (int pozycjaZnaku=0; pozycjaZnaku<kompletDanychAdresata.length(); pozycjaZnaku++)
    {
        if (kompletDanychAdresata[pozycjaZnaku] != '|')
        {
            informacja+=kompletDanychAdresata[pozycjaZnaku];
        }
        else
        {
            switch (numerInformacji)
            {
            case 1:
                adresat.idGlobalne = atoi (informacja.c_str());
                break;
            case 2:
                adresat.idWlasciciela= atoi (informacja.c_str());
                break;
            case 3:
                adresat.imie= informacja;
                break;
            case 4:
                adresat.nazwisko= informacja;
                break;
            case 5:
                adresat.numer_tel= informacja;
                break;
            case 6:
                adresat.email= informacja;
                break;
            case 7:
                adresat.adres= informacja;
                break;
            }
            informacja="";
            numerInformacji++;
        }
    }
    return adresat;
}



vector <Adresat> KorygowanieIDAdresatow (vector <Adresat> &adresaci)
{
    int numer=1;
    for (int i=0; i<adresaci.size();i++)
    {
        adresaci[i].idUzytkowe= numer;
        numer+=1;
    }
    return adresaci;
}



void PobranieListyAdresatowZPliku (vector <Adresat>&adresaci, int idZalogowanegoUzytkownika)
{
    Adresat nowaOsoba;
    string kompletDanychNowejOsoby="";

    fstream plik;
    plik.open("Adresaci.txt",ios::in);

    if (plik.good()==true)
    {
        while (getline(plik, kompletDanychNowejOsoby))
        {
            nowaOsoba=PorzadkowanieDanychAdresata(kompletDanychNowejOsoby);
            if (nowaOsoba.idWlasciciela==idZalogowanegoUzytkownika)
            {
            adresaci.push_back(nowaOsoba);
            }
        }
        plik.close();
        KorygowanieIDAdresatow(adresaci);
    }
}





vector <Uzytkownik> RejestracjaNowegoUzytkownika (vector <Uzytkownik>&uzytkownicy)
{
    system("cls");
    Uzytkownik uzytkownik;
    string nowaNazwa, noweHaslo;
    cout<<"Podaj nazwe uzytkownika: ";
    cin>>nowaNazwa;
    int i=0;
    int iloscUzytkownikow= uzytkownicy.size();
    while (i< iloscUzytkownikow)
    {
        if (uzytkownicy[i].nazwa==nowaNazwa)
        {
            cout<<"Taki uzytkownik istnieje. Wpisz inny login: ";
            cin>>nowaNazwa;
            i=0;
        }
        else
        {
           i++;
        }
    }
    cout<<"Podaj haslo: ";
    cin>>noweHaslo;
    if (uzytkownicy.empty())
    {
    uzytkownik.id=1;
    uzytkownik.nazwa=nowaNazwa;
    uzytkownik.haslo=noweHaslo;
    }
    else
    {
    uzytkownik.id=uzytkownicy.back().id+1;
    uzytkownik.nazwa=nowaNazwa;
    uzytkownik.haslo=noweHaslo;
    }
    uzytkownicy.push_back(uzytkownik);
    cout<<"Konto zalozone."<<endl;

    fstream plik;
    plik.open("Uzytkownicy.txt",ios::app);
    plik<<uzytkownik.id<<"|";
    plik<<uzytkownik.nazwa<<"|";
    plik<<uzytkownik.haslo<<"|"<<endl;
    plik.close();
    Sleep(1000);

    return uzytkownicy;
}





int LogowanieUzytkownika (vector <Uzytkownik>&uzytkownicy, vector <Adresat>&adresaci)
{
    system("cls");
    string wprowadzanaNazwa, wprowadzaneHaslo;
    cout<<"Podaj nazwe uzytkownika: ";
    cin>>wprowadzanaNazwa;
    int iloscUzytkownikow= uzytkownicy.size(), numerLogowania=0;
    int i=0;
    while (i< iloscUzytkownikow)
    {
        if (uzytkownicy[i].nazwa==wprowadzanaNazwa)
        {
            for (int proby=0; proby <3; proby++)
            {
                cout<<"Podaj haslo. Pozostalo prob: "<< 3-proby<<":";
                cin>>wprowadzaneHaslo;
                if (uzytkownicy[i].haslo==wprowadzaneHaslo)
                {
                    cout<<"Zalogowales sie"<<endl;
                    Sleep(1000);
                    adresaci.clear();
                    PobranieListyAdresatowZPliku(adresaci,uzytkownicy[i].id);
                    //cout<<calkowitaIloscAdresatow;
                    //system("pause");
                    return uzytkownicy[i].id;
                }
            }
            cout<<"Podales 3 razy nieprawidlowe haslo. Powrot do menu glownego";
            Sleep(2000);
            return 0;
        }
        i++;
    }
    cout<<"Login nieprawidlowy"<<endl;
    Sleep(1000);
    return numerLogowania;
}



void zmianaHasla (vector <Uzytkownik>&uzytkownicy, int idZalogowanegoUzytkownika)
{
    string haslo;
    int iloscUzytkownikow= uzytkownicy.size();
    cout<<"Podaj nowe haslo: ";
    cin>>haslo;
    for (int i=0;i<iloscUzytkownikow;i++)
    {
        if (uzytkownicy[i].id==idZalogowanegoUzytkownika)
        {
            uzytkownicy[i].haslo=haslo;
            cout<<"Haslo zostalo zmienione"<<endl;
            Sleep(1000);
        }
    }
}



void UaktualnienieAdresatowWPliku (vector <Adresat>&adresaci, int idZalogowanegoUzytkownika)
{
        fstream plik;
        plik.open("Adresaci_tymczasowy.txt",ios::out);
        for (int i=0; i<adresaci.size(); i++)
        {
            plik<<adresaci[i].idGlobalne<<"|";
            plik<<adresaci[i].idWlasciciela<<"|";
            plik<<adresaci[i].imie<<"|";
            plik<<adresaci[i].nazwisko<<"|";
            plik<<adresaci[i].numer_tel<<"|";
            plik<<adresaci[i].email<<"|";
            plik<<adresaci[i].adres<<"|"<<endl;
        }
        plik.close();
}




void UzupelnieniePozostalychAdresatowWPliku (vector <Adresat>&adresaci, int idZalogowanegoUzytkownika)
{
    Adresat nowaOsoba;
    string kompletDanychNowejOsoby="";

    fstream plik;
    plik.open("Adresaci.txt",ios::in);

        while (getline(plik, kompletDanychNowejOsoby))
        {
            nowaOsoba=PorzadkowanieDanychAdresata(kompletDanychNowejOsoby);
            if (nowaOsoba.idWlasciciela!=idZalogowanegoUzytkownika)
            {
            adresaci.push_back(nowaOsoba);
            }
        }
        plik.close();

        plik.open("Adresaci_tymczasowy.txt",ios::app);
        for (int i=0; i<adresaci.size(); i++)
        {
            plik<<adresaci[i].idGlobalne<<"|";
            plik<<adresaci[i].idWlasciciela<<"|";
            plik<<adresaci[i].imie<<"|";
            plik<<adresaci[i].nazwisko<<"|";
            plik<<adresaci[i].numer_tel<<"|";
            plik<<adresaci[i].email<<"|";
            plik<<adresaci[i].adres<<"|"<<endl;
        }

        plik.close();
}



int WylogowanieZKonta (vector <Adresat>&adresaci, int idZalogowanego)
{
    char potwierdzenie;
    system("cls");
    cout<<"Czy na pewno chcesz sie wylogowac?"<<endl<<"1-TAK / 2-NIE"<<endl;
    cin>>potwierdzenie;
    if (potwierdzenie=='1')
    {
    cout<<"Zostales wylogowany"<<endl;
    Sleep(1500);
    return 0;
    }
    else if (potwierdzenie=='2')
    {
    cout<<"Zdecydowales sie pozostac zalogowany"<<endl;
    Sleep(1500);
    return 1;
    }
    else
    {
        system("cls");
        cout<<"Bledny wybor! Nie nastapi wylogowanie";
        Sleep(1500);
        return 1;
    }
}






void Zamykanie_programu (vector <Uzytkownik> &uzytkownicy)
{
    char potwierdzeniezakonczenia;
    cout<<"Czy na pewno chcesz zamknac Twoj Niezbednik?"<<endl<<"Tak - 1"<<endl<<"Nie - 2"<<endl;
    cin>>potwierdzeniezakonczenia;
    if (potwierdzeniezakonczenia=='1')
    {
   /*     fstream plik;
        plik.open("Uzytkownicy.txt",ios::out);
        for (int i=0; i<uzytkownicy.size(); i++)
        {
            plik<<uzytkownicy[i].id<<"|";
            plik<<uzytkownicy[i].nazwa<<"|";
            plik<<uzytkownicy[i].haslo<<"|"<<endl;
        }
        plik.close(); */

        system("cls");
        cout<<"Dziekujemy za skorzystanie z Twojego Niezbednika";
        Sleep(1000);
        exit(0);
    }
    else if (potwierdzeniezakonczenia=='2')
    {
        cout<<"Zdecydowales sie pozostac w programie";
        Sleep(1000);
    }
    else
    {
        cout<<"Bledny wybor. Program nie zostaje zamkniety";
        Sleep(1000);
    }
}






void Wyswietlanie_calej_listy (vector <Adresat> &adresaci)
{
    system("cls");
    if (!adresaci.empty())
    {
        for (vector<Adresat>::iterator itr=adresaci.begin(); itr!=adresaci.end(); itr++)
        {
            //cout<<"probnie wyswietlamy ID globalne: "<<itr->idGlobalne<<endl<<endl;
            cout<<"ID:             "<<itr->idUzytkowe<<endl;
            cout<<"imie:           "<<itr->imie<<endl;
            cout<<"nazwisko:       "<<itr->nazwisko<<endl;
            cout<<"numer telefonu: "<<itr->numer_tel<<endl;
            cout<<"e-mail:         "<<itr->email<<endl;
            cout<<"adres:          "<<itr->adres<<endl;
            cout<<endl;
        }
        cout<<endl;
    }
    else
    {
        cout<<"Brak pozycji w ksiazce adresowej."<<endl;
    }
    system("pause");
}






void Wyszukiwanie_po_imieniu (vector <Adresat> &adresaci)
{
    system("cls");
    string szukane_imie;
    cout<<"Podaj imie, ktore ma zostac wyszukane"<<endl;
    cin>>szukane_imie;
    int licznikNietrafionychPozycji=0;

    system("cls");
    if (!adresaci.empty())
    {
        for (vector<Adresat>::iterator itr=adresaci.begin(); itr!=adresaci.end(); itr++)
        {

            if (itr->imie==szukane_imie)
            {
                cout<<"ID:             "<<itr->idUzytkowe<<endl;
                cout<<"imie:           "<<itr->imie<<endl;
                cout<<"nazwisko:       "<<itr->nazwisko<<endl;
                cout<<"numer telefonu: "<<itr->numer_tel<<endl;
                cout<<"e-mail:         "<<itr->email<<endl;
                cout<<"adres:          "<<itr->adres<<endl;
                cout<<endl;
            }
            else
            {
                licznikNietrafionychPozycji+=1;
            }
        }
        if (licznikNietrafionychPozycji==adresaci.size())
        {
            cout<< "Brak wynikow wyszukiwania"<<endl<<endl;
        }
    }
    else
    {
        cout<<"Brak pozycji w ksiazce"<<endl;
    }
    licznikNietrafionychPozycji=0;
    system("pause");
}




void Wyszukiwanie_po_nazwisku (vector <Adresat> &adresaci)
{
    system("cls");
    string szukane_nazwisko;
    cout<<"Podaj nazwisko, ktore ma zostac wyszukane"<<endl;
    cin>>szukane_nazwisko;
    int licznikNietrafionychPozycji=0;

    system("cls");
    if (!adresaci.empty())
    {
        for (vector<Adresat>::iterator itr=adresaci.begin(); itr!=adresaci.end(); itr++)
        {

            if (itr->nazwisko==szukane_nazwisko)
            {
                cout<<"ID:             "<<itr->idUzytkowe<<endl;
                cout<<"imie:           "<<itr->imie<<endl;
                cout<<"nazwisko:       "<<itr->nazwisko<<endl;
                cout<<"numer telefonu: "<<itr->numer_tel<<endl;
                cout<<"e-mail:         "<<itr->email<<endl;
                cout<<"adres:          "<<itr->adres<<endl;
                cout<<endl;
            }
            else
            {
                licznikNietrafionychPozycji+=1;
            }
        }
        if (licznikNietrafionychPozycji==adresaci.size())
        {
            cout<< "Brak wynikow wyszukiwania"<<endl<<endl;
        }
    }
    else
    {
        cout<<"Brak pozycji w ksiazce"<<endl;
    }
    licznikNietrafionychPozycji=0;
    system("pause");
}





vector <Adresat> Wprowadzanie_nowych_osob (vector <Adresat> &adresaci, int IDZalogowanego, int najwyzszyZajetyID)
{
    Adresat adresat;
    system("cls");
    cout<<"Wprowadz nowa osobe do ksiazki"<<endl;
    if (adresaci.empty()==true)
    {
        adresat.idUzytkowe=1;
    }
    else
    {
        adresat.idUzytkowe=adresaci.back().idUzytkowe+1;
    }
    adresat.idWlasciciela=IDZalogowanego;
    cout<<"Podaj imie: ";
    cin.sync();
    getline (cin, adresat.imie);
    cout<<"Podaj nazwisko: ";          cin>>adresat.nazwisko;
    cout<<"Podaj numer telefonu: ";    cin>>adresat.numer_tel;
    int cyfraNumeru=0;
    while (cyfraNumeru<adresat.numer_tel.length())
    {
        if (adresat.numer_tel[cyfraNumeru]<48 || adresat.numer_tel[cyfraNumeru]>57)
        {
            cout<<"Numer niepoprawny! Wprowadz inny numer: ";
            cin>>adresat.numer_tel;
            cyfraNumeru=0;
        }
        else
            cyfraNumeru+=1;
    }
    int ilosc=0;
    while (ilosc< adresaci.size())
    {
        if (adresat.numer_tel==adresaci[ilosc].numer_tel)
        {
            cout<<"Ten numer jest juz przypisany innemu uzytkownikowi! Podaj inny numer: "; cin>>adresat.numer_tel;
            int cyfraNumeru=0;
            while (cyfraNumeru<adresat.numer_tel.length())
            {
                if (adresat.numer_tel[cyfraNumeru]<48 || adresat.numer_tel[cyfraNumeru]>57)
                {
                    cout<<"Numer niepoprawny! Wprowadz inny numer: "; cin>>adresat.numer_tel;
                    cyfraNumeru=0;
                }
                else
                    cyfraNumeru+=1;
            }
            ilosc=0;
        }
        else
        {
            ilosc++;
        }
    }
    cout<<"Podaj e-mail: "; cin>>adresat.email;
    cout<<"Podaj adres: ";
    cin.sync();
    getline (cin, adresat.adres);
    adresat.idGlobalne=najwyzszyZajetyID+1;
    adresaci.push_back(adresat);
    system("cls");
    cout<<"Dodano osobe do kontaktow!";
    Sleep(1000);

    return adresaci;
}





vector <Adresat> Usuwanie_pozycji_z_ksiazki (vector <Adresat> &adresaci, int IDZalogowanego)
{
    char rozmiar=adresaci.size()+'0';
    system("cls");
    if (adresaci.empty())
    {
        cout<<"Ksiazka jest pusta. Nie ma pozycji do usuniecia";
    }
    else
    {
        cout<<"Wybierz numer pozycji do usuniecia:"<<endl;
        for (vector<Adresat>::iterator itr=adresaci.begin(); itr!=adresaci.end(); itr++)
        {
            cout<<itr->idUzytkowe<<" "<<itr->imie<<" "<<itr->nazwisko<<" (tel.: "<<itr->numer_tel<<")"<<endl;
        }
        cout<<endl;
    }
    char numerUsuniecia, potwierdzenie;
    cin>>numerUsuniecia;
    int pozycjadousuniecia=numerUsuniecia-'0';
    if (pozycjadousuniecia>0 && pozycjadousuniecia<=adresaci.size())
    {
        cout<<"Czy na pewno chcesz usunac te pozycje? /T-TAK , N-NIE/ "<<endl;
        cin>>potwierdzenie;
        if (potwierdzenie=='T' || potwierdzenie=='t')
        {
            adresaci.erase(adresaci.begin()+pozycjadousuniecia-1);
            system("cls");
            cout<<"Pozycja usunieta!";
            KorygowanieIDAdresatow(adresaci);
            Sleep(1500);
        }
        else
            cout<<"Kontakt nie zostal usuniety.";  Sleep(1000);
    }
    else
    {
        cout<<"Nie ma takiej pozycji!";  Sleep(1000);
    }

    /*UaktualnienieAdresatowWPliku (adresaci, IDZalogowanego);
    adresaci.clear();
    UzupelnieniePozostalychAdresatowWPliku (adresaci, IDZalogowanego);
    remove("Adresaci.txt");
    rename( "Adresaci_tymczasowy.txt", "Adresaci.txt" ); */
    return adresaci;
}





Adresat EdytorDanychWskazanejOsoby (Adresat adresat)
{
    system("cls");
    char numerEdytowanejInformacji;
    string nowaWartosc;
    cout<<"Imie: "<<adresat.imie<<"  Nazwisko: "<<adresat.nazwisko<<endl<<"Adres: "<<adresat.adres<<"  E-mail: "<<adresat.email<<"  Nr telefonu: "<<adresat.numer_tel<<endl<<endl;
    char potwierdzenieDalszejEdycji='T';
    while (potwierdzenieDalszejEdycji=='T'||potwierdzenieDalszejEdycji=='t')
    {
        cout<<"Ktora dana chcialbys edytowac?"<<endl;
        cout<<"1-imie"<<endl<<"2-nazwisko"<<endl<<"3-adres"<<endl<<"4-e-mail"<<endl<<"5-numer telefonu"<<endl;
        cin>>numerEdytowanejInformacji;
        if (numerEdytowanejInformacji=='1')
        {
            cout<<"Podaj nowe imie: ";
            cin.sync();
            getline (cin, adresat.imie);
        }
        if (numerEdytowanejInformacji=='2')
        {
            cout<<"Podaj nowe nazwisko: ";
            cin>>(nowaWartosc, adresat.nazwisko);
        }
        if (numerEdytowanejInformacji=='3')
        {
            cout<<"Podaj nowy adres: ";
            cin.sync();
            getline (cin, adresat.adres);
        }
        if (numerEdytowanejInformacji=='4')
        {
            cout<<"Podaj nowy e-mail: ";
            cin>>(nowaWartosc, adresat.email);
        }
        if (numerEdytowanejInformacji=='5')
        {
            cout<<"Podaj nowy numer telefonu: ";
            cin>>(nowaWartosc, adresat.numer_tel);
            int cyfraNumeru=0;
            while (cyfraNumeru<adresat.numer_tel.length())
            {
                if (adresat.numer_tel[cyfraNumeru]<48 || adresat.numer_tel[cyfraNumeru]>57)
                {
                    cout<<"Numer niepoprawny! Wprowadz inny numer: ";
                    cin>>(nowaWartosc, adresat.numer_tel);
                    cyfraNumeru=0;
                }
                else
                    cyfraNumeru+=1;
            }
        }
        else if (numerEdytowanejInformacji<'1' || numerEdytowanejInformacji>'5')
        {
            cout<<"Blad wyboru"<<endl;
            Sleep(500);
            break;
        }
        cout<<"Czy chcesz dokonac dalszych modyfikacji?  /T-TAK,  N-NIE/"<<endl;
        cin>>potwierdzenieDalszejEdycji;
    }
    return adresat;
}


vector <Adresat> Edytowanie_pozycji_z_ksiazki (vector <Adresat> &adresaci, int idZalogowanego)
{
    int pozycjaDoEdycji;
    system("cls");
    if (adresaci.empty())
    {
        cout<<"Ksiazka jest pusta. Nie ma pozycji do edycji";
    }
    else
    {
        cout<<"Wybierz numer pozycji, ktora chcesz edytowac:"<<endl;
        for (vector<Adresat>::iterator itr=adresaci.begin(); itr!=adresaci.end(); itr++)
        {
            cout<<itr->idUzytkowe<<" "<<itr->imie<<" "<<itr->nazwisko<<" (tel.: "<<itr->numer_tel<<")"<<endl;
        }
        cout<<endl;
        cin>>pozycjaDoEdycji;
        if (pozycjaDoEdycji>=1&&pozycjaDoEdycji<=adresaci.size())
        {
            adresaci[pozycjaDoEdycji-1]=EdytorDanychWskazanejOsoby(adresaci[pozycjaDoEdycji-1]);
        }
        else
            cout<<"Bledny wybor";

        Sleep(1200);
        return adresaci;
    }
}



int UstalenieNajwyzszegoZajetegoIdAdresata (vector <Adresat>&adresaci)
{
    int najwyzszeZajeteID=0;
    Adresat WyszukiwaczOsobyONajwyzszymID;
    string liniaTekstu="";

    fstream plik;
    plik.open("Adresaci.txt",ios::in);

    if (plik.good()==true)
    {
        while (getline(plik, liniaTekstu))
        {
            WyszukiwaczOsobyONajwyzszymID=PorzadkowanieDanychAdresata(liniaTekstu);
            if (WyszukiwaczOsobyONajwyzszymID.idGlobalne>najwyzszeZajeteID)
            {
            najwyzszeZajeteID=WyszukiwaczOsobyONajwyzszymID.idGlobalne;
            }
        }
        plik.close();
    }
    return najwyzszeZajeteID;
}



void NadpisDanychOAdresatachDoPliku(vector <Adresat> &adresaci, int idZalogowanegoUzytkownika)
{
    UaktualnienieAdresatowWPliku (adresaci, idZalogowanegoUzytkownika);
    adresaci.clear();
    UzupelnieniePozostalychAdresatowWPliku (adresaci, idZalogowanegoUzytkownika);
    remove("Adresaci.txt");
    rename( "Adresaci_tymczasowy.txt", "Adresaci.txt" );
    adresaci.clear();
    PobranieListyAdresatowZPliku(adresaci, idZalogowanegoUzytkownika);
}



int main()
{
    vector <Adresat> adresaci;
    vector <Uzytkownik> uzytkownicy;

    int idZalogowanegoUzytkownika=0;
    int najwyzszyZajetyIdAdresata=UstalenieNajwyzszegoZajetegoIdAdresata(adresaci);
    char wybor;
    PobranieListyUzytkownikowZPliku(uzytkownicy);
    while (1)
    {
        if (idZalogowanegoUzytkownika==0)
        {
            system("cls");
            cout<<"1.Rejestracja"<<endl<<"2.Logowanie"<<endl<<"9.Zakoncz program"<<endl<<endl<<endl;
            cout<<"//POMOCNICZO- LISTA UZYTKOWNIKOW:"<<endl;
            for (vector<Uzytkownik>::iterator itr=uzytkownicy.begin(); itr!=uzytkownicy.end(); itr++)
        {

            cout<<"ID: "<<itr->id<<"  login:  "<<itr->nazwa<<"  haslo:  "<<itr->haslo<<endl;
        }
            cin>>wybor;

            if (wybor=='1')
            {
                uzytkownicy=RejestracjaNowegoUzytkownika(uzytkownicy);
            }
            else if (wybor=='2')
            {
                idZalogowanegoUzytkownika=LogowanieUzytkownika(uzytkownicy, adresaci);
            }
            else if (wybor=='9')
            {
                Zamykanie_programu(uzytkownicy);
            }

        }
        else
        {
            system("cls");
            cout<<"Jestes zalogowany jako: "<<uzytkownicy[idZalogowanegoUzytkownika-1].nazwa<<endl<<endl;
            cout<<"MENU GLOWNE TWOJEGO NIEZBEDNIKA"<<endl;
            cout<<"Wybierz sposrod opcji:"<<endl<<"1.Wyswietl cala ksiazke adresowa"<<endl;
            cout<<"2.Wyszukaj osobe po imieniu"<<endl<<"3.Wyszukaj osobe po nazwisku"<<endl;
            cout<<"4.Dodaj nowa pozycje do ksiazki"<<endl<<"5.Usun pozycje z ksiazki"<<endl;
            cout<<"6.Edytuj pozycje"<<endl<<"8.Zmiana hasla"<<endl<<"9.Wylogowanie"<<endl;
            cin>>wybor;
            if (wybor=='1')
            {
                Wyswietlanie_calej_listy(adresaci);
            }
            if (wybor=='2')
            {
                Wyszukiwanie_po_imieniu(adresaci);
            }
            if (wybor=='3')
            {
                Wyszukiwanie_po_nazwisku(adresaci);
            }
            if (wybor=='4')
            {
                Wprowadzanie_nowych_osob(adresaci, idZalogowanegoUzytkownika, najwyzszyZajetyIdAdresata);
                najwyzszyZajetyIdAdresata+=1;
                NadpisDanychOAdresatachDoPliku(adresaci, idZalogowanegoUzytkownika);
            }
            if (wybor=='5')
            {
                Usuwanie_pozycji_z_ksiazki(adresaci, idZalogowanegoUzytkownika);
                NadpisDanychOAdresatachDoPliku(adresaci, idZalogowanegoUzytkownika);
            }
            if (wybor=='6')
            {
                Edytowanie_pozycji_z_ksiazki(adresaci, idZalogowanegoUzytkownika);
                NadpisDanychOAdresatachDoPliku(adresaci, idZalogowanegoUzytkownika);
            }

            if (wybor=='8')
            {
                zmianaHasla(uzytkownicy, idZalogowanegoUzytkownika);
            }
            else if (wybor=='9')
            {
                idZalogowanegoUzytkownika= WylogowanieZKonta(adresaci, idZalogowanegoUzytkownika);
            }

        }
    }
    return 0;

}
