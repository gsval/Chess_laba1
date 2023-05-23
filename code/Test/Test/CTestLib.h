//создать класс одного теста
#pragma once
#include "CChessGameLib.h"
#include <iostream>
#include <ctime>
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"

using namespace std;


enum Status { FAILED, PASSED };

class Test {
public: 
    TGame *game;
    TFigure* figura;
    string name;// имя теста
    TCoord first_coord,last_coord;
    //int(*function_reselt)(); // должна возвращать интвое значения и ничего не должна принимать, проверяет правильность второй функции 
    Status status;
    //bool(*function_test)(TCoord, TCoord); //функция, которая будет вызываться при запуске теста. Принимает начальную и конечную координату
    //
    Test() :name("default"), status(FAILED) {}
    Test(string _name, TGame *_game, TCoord _first_coord, TCoord _last_coord) :name("default"), status(FAILED){ //списки инициализации НЕ ТРОГАТЬ
        //поменять всё 
        first_coord = _first_coord;
        last_coord = _last_coord;
        game = _game;
       name = _name;
     
    };
   //конструктор копирования менять и диструктор не нужен
    Test(Test& tmp) {
        function_test = tmp.function_test;
        name = tmp.name;
        par1 = tmp.par1;
        par2 = tmp.par2;
        expected = tmp.expected;
    }
    ~Test() {};
    ostream& operator<<(ostream& out) {
        out << "--------------------" << endl;
        out << "\tName: " << name << endl;
        out << "\tFirst parametr: " << first_coord << endl;
        out << "\tSecond parametr: " << last_coord << endl;
        out << "\tStatus: " << status << endl;
        out << "---------------------" << endl;
        return out;
    }
    void run() { //запускает выполнение функции на 21 строке
        figura=game->mass[first_coord];
        bool tmp = game->move(first_coord, last_coord);
        bool tmp2 = (figura == game->mass[last_coord]);
        if (tmp && (tmp2)) { status = PASSED; }
    }
};
//

class Tests {
    string name;
    TGame *game;
    Test** all;
    size_t size;
    size_t count;

public:
    Tests() : name("default"), size(0), count(0) {
        all = nullptr;
    };
    Tests(Tests& tmp) {
        name = tmp.name;
        size = tmp.size;
        count = tmp.count;
        all = new Test[count];
        for (int i = 0; i < count; i++) {
            all[i] = tmp.all[i];
        }

    };
    Tests(string _name,TGame *_game) {
        name = _name;
        game = _game;
    };
    ~Tests() {
        delete[] all;
    }
    void create_test(string name, TCoord _first_coord, TCoord _last_coord) {

        Test* tmp = new Test(name, game,_first_coord,_last_coord);
        Test** tmp_mass = new Test*[count + 1];
        for (int i = 0; i < count; i++) {
            tmp_mass[i] = all[i];
        }
        tmp_mass[count] = tmp;
        delete all;
        all = tmp_mass;
        count++;
    }
    void run_all() {
        int true_test = 0;
        for (int i = 0; i < count; i++) {
            cout << GREEN << "[   RUN      ] " << RESET << all[i]->name << endl;
            all[i]->run();
            if (all[i]->status == PASSED) {
                true_test++;
                cout << GREEN << "[       OK   ] " << RESET << all[i]->name << endl;
            }
            else if (all[i]->status == FAILED) {
                cout << RED << "[   FAILED   ] " << RESET << endl;
                cout << "\tExpected value: " << all[i]->figura->get_figure_type() << endl;
                cout << "\tWitch is: " << (all[i]->game->mass[all[i]->last_coord])->get_figure_type() << endl;
                size++;

            }
        }

        cout << GREEN << "[============] " << RESET << count << " test from run" << endl;
        cout << GREEN << "[   PASSED   ] " << RESET << true_test << " tests" << endl;
        cout << RED << "[   FAILES   ] " << RESET << size << " tests, listed below:" << endl;
        for (int i = 0; i < count; i++) {
            if (all[i]->status != PASSED) {
                cout << "\t" << all[i]->name << endl;
            }
        }

    }

};