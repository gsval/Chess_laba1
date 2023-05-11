﻿#include <iostream>
using namespace std;
//блок с кодировками цвета текста в консли
#define RESET "\033[0m"
#define BLACK "\033[30m"
#define RED   "\033[31m"
#define GREEN "\033[32m"
#define ORANGE "\033[33m"
#define BLUE "\033[34m"

//перечисление какие фигуры у нас есть.
enum TFigure_type { no_type, pawn, rook, knight, bishop, queen, king };//пешка, ладья, конь, слон, королева, король
enum TFigure_color { no_color, black, white };//цвет фигуры
enum ELogic_error { invalid_index };//Возможные ошибки

//Класс координат
class TCoord {
private:
    short int x;
    short int y;
public:
    void set_x(int _x);
    void set_y(int _y);
    short int get_x();
    short int get_y();
    TCoord& operator=(const TCoord& tmp);
    bool operator==(const TCoord& tmp) const;
    TCoord(short int _x, short int _y);
    TCoord() :x(-1), y(-1) {};
    friend ostream& operator<<(ostream& os, const TCoord& coord);
};
void TCoord::set_x(int _x) { if (_x > -2 && _x < 8) { x = _x; } else throw invalid_index; }
void TCoord::set_y(int _y) { if (_y > -2 && _y < 8) { y = _y; } else throw invalid_index; }
short int TCoord::get_x() { return x; }
short int TCoord::get_y() { return y; }
TCoord& TCoord::operator=(const TCoord& tmp) {
    x = tmp.x;
    y = tmp.y;
    return *this;
}
bool TCoord::operator==(const TCoord& tmp) const {
    return ((x == tmp.x) && (y == tmp.y));
}
TCoord::TCoord(short int _x, short int _y) {
    if (_x < 8 && _x > -2 && _y > -2 && _y < 8) {
        x = _x;
        y = _y;
    }
    else throw invalid_index;
}
ostream& operator<<(ostream& os, const TCoord& coord) {
    os << "X = " << coord.x << "Y = " << coord.y;
    return os;
}

//массив координат, координату в него можно добавлять с помощю add_cooия новойrd(), кула вы передаете
//или координаты x,y по отдельности либо обьктом TCoord. Сейчас функционала у него нет, но потом он появится.
//на данном этапе вам нужна только функции добавления новой координаты
class TCoordMass {
private:
    short int count;
    TCoord* mass;
public:
    int get_count();
    void add_coord(TCoord& _tmp);
    void add_coord(short int x, short int y);
    void print();
    TCoordMass();
    TCoordMass(TCoordMass& tmp);
    ~TCoordMass() { delete[] mass; }
    TCoord operator[](int i);
    TCoordMass& operator=(const TCoordMass& tmp);
    friend ostream& operator<<(ostream& os, const TCoordMass& coord_mass);
};
ostream& operator<<(ostream& os, const TCoordMass& coord_mass) {
    os << endl;
    os << ORANGE << "\t\t\tOBJECT NAME = " << typeid(coord_mass).name() << RESET << endl;
    os << "coord count = " << coord_mass.count << endl;
    for (int i = 0; i < coord_mass.count; i++) {
        os << "index[" << i << "] X = " << coord_mass.mass[i].get_x() << "\tY = " << coord_mass.mass[i].get_y() << endl;
    }
    return os;
}
int TCoordMass::get_count() { return count; }
void TCoordMass::add_coord(TCoord& _tmp) { add_coord(_tmp.get_x(), _tmp.get_y()); }
void TCoordMass::print() {
    for (int i = 0; i < count; i++) {
        cout << "X: " << mass[i].get_x() << "Y: " << mass[i].get_y() << endl;
    }
}
void TCoordMass::add_coord(short int x, short int y) {
    TCoord* tmp_mass = new TCoord[count + 1];
    for (short int i = 0; i < count; i++) { tmp_mass[i] = mass[i]; }
    tmp_mass[count].set_x(x);
    tmp_mass[count].set_y(y);
    delete[] mass;
    mass = tmp_mass;
    count++;
}    
TCoordMass::TCoordMass() { mass = nullptr; count = 0; }
TCoordMass::TCoordMass(TCoordMass& tmp) {
    mass = new TCoord[tmp.count];
    count = tmp.count;
    for (int i = 0; i < tmp.count; i++) {
        mass[i] = tmp.mass[i];
    }
}
TCoord TCoordMass::operator[](int i) {
    if (i >= 0 && i < count)
        return mass[i]; else throw invalid_index;
}
TCoordMass& TCoordMass::operator=(const TCoordMass& tmp) {
    delete[]mass;
    mass = new TCoord[tmp.count];
    count = tmp.count;
    for (int i = 0; i < tmp.count; i++) {
        mass[i] = tmp.mass[i];
    }
    return *this;
}

//класс - общий предок всех фигур, нужен для того чтобы хранить все фигуры в одном массиве.
class TFigure {
protected:
    short int id;//Уникальный номер фигуры на поле (от 0 до 31). Задается в конструкторе наследников.
    TFigure_type type;//Тип фигуры задается в конструкторе наследующих классов.
    TCoord coord;//координата фигуры, если фигура съедена - (-1,-1)
    TFigure_color color;//цвет фигуры
public:
    TCoord get_coord();
    TFigure_type get_figure_type();
    TFigure_color get_figure_color();
private:
    virtual TCoordMass& get_list_coord() = 0;
    virtual bool check_move(TCoord coord_last) = 0;
    virtual void move_to(TCoord coord_last) = 0;
public:
    TFigure();
    TFigure(TFigure& tmp);
    TFigure(short int _id, TCoord _coord, TFigure_color _color);
    ~TFigure() {}
    friend class TGame;
};
TCoord TFigure::get_coord() { return coord; }
TFigure_type TFigure::get_figure_type() { return type; }
TFigure_color TFigure::get_figure_color() { return color; }
TFigure::TFigure() {
    id = -1;
    type = no_type;
    coord.set_x(-1);
    coord.set_y(-1);
    color = no_color;
}
TFigure::TFigure(TFigure& tmp) {
    coord = tmp.coord;
    type = tmp.type;
    id = tmp.id;
    color = tmp.color;
}
TFigure::TFigure(short int _id, TCoord _coord, TFigure_color _color) {
    id = _id;
    coord = _coord;
    color = _color;
    type = no_type;
}

//Класс пешка
class TPawn :public TFigure {
public:
    TPawn();
    TPawn(short int _id, TCoord _coord, TFigure_color _color);
    TPawn(TPawn& tmp);
    ~TPawn() {}

    TCoordMass& get_list_coord();
    bool check_move(TCoord coord_last);
    void move_to(TCoord coord_last);
};
TPawn::TPawn() {
    id = -1;
    type = pawn;
    color = no_color;
    coord.set_x(-1);
    coord.set_y(-1);
}
TPawn::TPawn(short int _id, TCoord _coord, TFigure_color _color) {
    type = pawn;
    id = _id;
    coord.set_x(_coord.get_x());
    coord.set_y(_coord.get_y());
    color = _color;
}
TPawn::TPawn(TPawn& tmp) {
    type = pawn;
    id = tmp.id;
    coord.set_x(tmp.coord.get_x());
    coord.set_y(tmp.coord.get_y());
    color = tmp.color;
}
void TPawn::move_to(TCoord coord_last) {
    coord.set_x(coord_last.get_x());
    coord.set_y(coord_last.get_y());
}
bool TPawn::check_move(TCoord coord_last) {
    if (color == black) {
        if ((coord.get_y() - 1 == coord_last.get_y() && (coord.get_x() == coord_last.get_x()))) { return 1; }
        else
        {
            if ((coord.get_y() == 7) && (coord.get_y() - 2 == coord_last.get_y() && (coord.get_x() == coord_last.get_x()))) return 1; else return 0;
        }
    }
    else if (color == white) {
        if ((coord.get_y() + 1 == coord_last.get_y() && (coord.get_x() == coord_last.get_x()))) { return 1; }
        else
        {
            if ((coord.get_y() == 1) && (coord.get_y() + 2 == coord_last.get_y() && (coord.get_x() == coord_last.get_x()))) return 1; else return 0;

        }
    }
}
TCoordMass& TPawn::get_list_coord() {
    TCoordMass* mass;
    mass = new TCoordMass;
    if (color == white) {
        if (coord.get_y() == 1) {
            mass->add_coord(coord.get_x(), coord.get_y() + 1);
            mass->add_coord(coord.get_x(), coord.get_y() + 2);
        }
        else {
            if (coord.get_y() == 7) {

            }
            else {
                mass->add_coord(coord.get_x(), coord.get_y() + 1);
            }
        }
    }
    else if (color == black) {
        if (coord.get_y() == 6) {
            mass->add_coord(coord.get_x(), coord.get_y() - 1);
            mass->add_coord(coord.get_x(), coord.get_y() - 2);
        }
        else {
            if (coord.get_y() == 0) {

            }
            else {
                mass->add_coord(coord.get_x(), coord.get_y() - 1);
            }
        }
    }


    //cout << "function: get_list_coord - END" << endl;
    return *mass;
}

//Класс король
class TKing :public TFigure {
public:
    TKing();//конструктор по умолчанию
    TKing(short int _id, TCoord _coord, TFigure_color _color);//конструктор, которым бы будем пользоваться в классе TDeck
    TKing(TKing& tmp);//конструктор копирования
    ~TKing() {};//деструктор

    TCoordMass& get_list_coord();//возвращает список координат, куда фигура может сдвинуться, начальные координаты берет от свего обьекта.
    bool check_move(TCoord coord_last);//возвращает true если фигура может передвинуться на переданные координаты.
    void move_to(TCoord coord_last);//пердвигает фигуру на переданные координаты без проверки(просто меняет свое поле TCoord). 
};
TKing::TKing(short int _id, TCoord _coord, TFigure_color _color) {
    type = king;
    id = _id;
    coord.set_x(_coord.get_x());
    coord.set_y(_coord.get_y());
    color = _color;
}
TKing::TKing(TKing& tmp) {
    type = king;
    id = tmp.id;
    coord.set_x(tmp.coord.get_x());
    coord.set_y(tmp.coord.get_y());
    color = tmp.color;
}
TKing::TKing() {
    id = -1;
    type = king;
    color = no_color;
    coord.set_x(-1);
    coord.set_y(-1);
}
void TKing::move_to(TCoord coord_last) {
    coord.set_x(coord_last.get_x());
    coord.set_y(coord_last.get_y());
}
bool TKing::check_move(TCoord coord_last) {
    if ((abs(coord_last.get_x() - coord.get_x()) <= 1) && (abs(coord_last.get_y() - coord.get_y()) <= 1)) return true;
    else return 0;
};
TCoordMass& TKing::get_list_coord() {
    TCoordMass* mass;
    mass = new TCoordMass;
    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0)continue; else {
                int tmp_x = coord.get_x() + i, tmp_y = coord.get_y() + j;
                if (tmp_x >= 0 && tmp_x < 8 && tmp_y >= 0 && tmp_y < 8) {
                    mass->add_coord(tmp_x, tmp_y);
                }
            }
        }

    }
    return *mass;
}

//Класс королева
class TQueen :public TFigure {
public:
    TQueen(short int _id, TCoord _coord, TFigure_color _color);
    TQueen();
    TQueen(TQueen& tmp);
    ~TQueen() {}
    TCoordMass& get_list_coord();
    bool check_move(TCoord coord_last);
    void move_to(TCoord coord_last);
};
TQueen::TQueen(short int _id, TCoord _coord, TFigure_color _color) {
    type = queen;
    id = _id;
    coord.set_x(_coord.get_x());
    coord.set_y(_coord.get_y());
    color = _color;
}
TQueen::TQueen() {
    id = -1;
    type = queen;
    color = no_color;
    coord.set_x(-1);
    coord.set_y(-1);
}
TQueen::TQueen(TQueen& tmp) {
    type = queen;
    id = tmp.id;
    coord.set_x(tmp.coord.get_x());
    coord.set_y(tmp.coord.get_y());
    color = tmp.color;
}
TCoordMass& TQueen::get_list_coord() {
    TCoordMass* mass;
    mass = new TCoordMass;
    for (int i = 0; i < 8; i++) {
        if (coord.get_x() + i < 8 && coord.get_y() + i < 8) { mass->add_coord(coord.get_x() + i, coord.get_y() + i); }
        if (coord.get_x() + i < 8 && coord.get_y() - i > -1) { mass->add_coord(coord.get_x() + i, coord.get_y() - i); }
        if (coord.get_x() - i > -1 && coord.get_y() + i < 8) { mass->add_coord(coord.get_x() - i, coord.get_y() + i); }
        if (coord.get_x() - i > -1 && coord.get_y() - i > -1) { mass->add_coord(coord.get_x() - i, coord.get_y() - i); }
        if (coord.get_x() + i < 8) { mass->add_coord(coord.get_x() + i, coord.get_y()); }
        if (coord.get_y() + i < 8) { mass->add_coord(coord.get_x(), coord.get_y() + i); }
        if (coord.get_x() - i > -1) { mass->add_coord(coord.get_x() - i, coord.get_y()); }
        if (coord.get_y() - i > -1) { mass->add_coord(coord.get_x(), coord.get_y() - i); }
    }
    return *mass;
}
bool TQueen::check_move(TCoord coord_last) {
    if (coord_last.get_x() == coord.get_x() || coord_last.get_y() == coord.get_y()) {
        return true;
    }
    else if (abs(coord.get_x() - coord_last.get_x()) == abs(coord.get_y() - coord_last.get_y())) {
        return true;
    }
    else { return false; }
}
void TQueen::move_to(TCoord coord_last) {
    coord.set_x(coord_last.get_x());
    coord.set_y(coord_last.get_y());
}

//Класс ладья
class TRook :public TFigure {
public:
    TRook();
    TRook(TRook& tmp);
    TRook(short int _id, TCoord _coord, TFigure_color _color);
    ~TRook() {}
    TCoordMass& get_list_coord();
    bool check_move(TCoord coord_last);
    void move_to(TCoord coord_last);
};
TRook::TRook(TRook& tmp) {
    type = rook;
    id = tmp.id;
    coord.set_x(tmp.coord.get_x());
    coord.set_y(tmp.coord.get_y());
    color = tmp.color;
}
TRook::TRook() {
    id = -1;
    type = rook;
    color = no_color;
    coord.set_x(-1);
    coord.set_y(-1);
}
TRook::TRook(short int _id, TCoord _coord, TFigure_color _color) {
    type = rook;
    id = _id;
    coord.set_x(_coord.get_x());
    coord.set_y(_coord.get_y());
    color = _color;
}
void TRook::move_to(TCoord coord_last) {
    coord.set_x(coord_last.get_x());
    coord.set_y(coord_last.get_y());
}
bool TRook::check_move(TCoord coord_last) {
    if (coord_last.get_x() == coord.get_x() && (coord_last.get_y() > -1) && (coord_last.get_y() != coord.get_y())) return true;
    if (coord_last.get_y() == coord.get_y() && (coord_last.get_x() > -1) && (coord_last.get_x() != coord.get_x())) return true;
    else return false;
};
TCoordMass& TRook::get_list_coord() {
    TCoordMass* mass = new TCoordMass;
    for (int i = coord.get_y() + 1; i <= 7; i++) {
        mass->add_coord(coord.get_x(), i);
    };
    for (int i = coord.get_y() - 1; i >= 0; i--) {
        mass->add_coord(coord.get_x(), i);
    };
    for (int i = coord.get_x() + 1; i <= 7; i++) {
        mass->add_coord(i, coord.get_y());
    };
    for (int i = coord.get_x() - 1; i >= 0; i--) {
        mass->add_coord(i, coord.get_y());
    };
    return *mass;
}

//Класс слон
class TBishop :public TFigure {
public:
    TBishop(short int _id, TCoord _coord, TFigure_color _color);
    TBishop();
    TBishop(TBishop& tmp);
    ~TBishop() {}
    TCoordMass& get_list_coord();
    bool check_move(TCoord coord_last);
    void move_to(TCoord coord_last);
};
TBishop::TBishop(short int _id, TCoord _coord, TFigure_color _color) {
    type = bishop;
    id = _id;
    coord.set_x(_coord.get_x());
    coord.set_y(_coord.get_y());
    color = _color;
}
TBishop::TBishop() {
    id = -1;
    type = bishop;
    color = no_color;
    coord.set_x(-1);
    coord.set_y(-1);
}
TBishop::TBishop(TBishop& tmp) {
    type = bishop;
    id = tmp.id;
    coord.set_x(tmp.coord.get_x());
    coord.set_y(tmp.coord.get_y());
    color = tmp.color;
}
TCoordMass& TBishop::get_list_coord() {
    TCoordMass* mass;
    mass = new TCoordMass;
    for (int i = 0; i < 8; i++) {
        if (coord.get_x() + i < 8 && coord.get_y() + i < 8) { mass->add_coord(coord.get_x() + i, coord.get_y() + i); }
        if (coord.get_x() + i < 8 && coord.get_y() - i > -1) { mass->add_coord(coord.get_x() + i, coord.get_y() - i); }
        if (coord.get_x() - i > -1 && coord.get_y() + i < 8) { mass->add_coord(coord.get_x() - i, coord.get_y() + i); }
        if (coord.get_x() - i > -1 && coord.get_y() - i > -1) { mass->add_coord(coord.get_x() - i, coord.get_y() - i); }
    }
    return *mass;
}
bool TBishop::check_move(TCoord coord_last) {
    if (abs(coord.get_x() - coord_last.get_x()) == abs(coord.get_y() - coord_last.get_y())) {
        return true;
    }
    else { return false; }
}
void TBishop::move_to(TCoord coord_last) {
    coord.set_x(coord_last.get_x());
    coord.set_y(coord_last.get_y());
}

//Класс конь
class TKnight :public TFigure {
public:
    TKnight(short int _id, TCoord _coord, TFigure_color _color);
    TKnight();
    TKnight(TKnight& tmp);
    ~TKnight() {}

    TCoordMass& get_list_coord();
    bool check_move(TCoord coord_last);
    void move_to(TCoord coord_last);
};
TKnight::TKnight(TKnight& tmp) {
    type = knight;
    id = tmp.id;
    coord.set_x(tmp.coord.get_x());
    coord.set_y(tmp.coord.get_y());
    color = tmp.color;
}
TKnight::TKnight() {
    id = -1;
    type = knight;
    color = no_color;
    coord.set_x(-1);
    coord.set_y(-1);
}
TKnight::TKnight(short int _id, TCoord _coord, TFigure_color _color) {
    type = knight;
    id = _id;
    coord.set_x(_coord.get_x());
    coord.set_y(_coord.get_y());
    color = _color;
}
bool TKnight::check_move(TCoord coord_last) {
    TCoord deth_coord(-1, -1);
    if (coord_last == deth_coord) throw invalid_index;
    if ((coord.get_y() - 2 == coord_last.get_y() && coord.get_x() - 1 == coord_last.get_x()) && (coord.get_y() - 2 >= 0 && coord.get_x() - 1 >= 0)) return true;
    if ((coord.get_y() - 2 == coord_last.get_y() && coord.get_x() + 1 == coord_last.get_x()) && (coord.get_y() - 2 >= 0 && coord.get_x() + 1 <= 7)) return true;
    if ((coord.get_y() - 1 == coord_last.get_y() && coord.get_x() + 2 == coord_last.get_x()) && (coord.get_y() - 1 >= 0 && coord.get_x() + 2 <= 7)) return true;
    if ((coord.get_y() + 1 == coord_last.get_y() && coord.get_x() + 2 == coord_last.get_x()) && (coord.get_y() + 1 <= 7 && coord.get_x() + 2 <= 7)) return true;
    if ((coord.get_y() - 1 == coord_last.get_y() && coord.get_x() - 2 == coord_last.get_x()) && (coord.get_y() - 1 >= 0 && coord.get_x() - 2 >= 0)) return true;
    if ((coord.get_y() + 1 == coord_last.get_y() && coord.get_x() - 2 == coord_last.get_x()) && (coord.get_y() + 1 <= 7 && coord.get_x() - 2 >= 0)) return true;
    if ((coord.get_y() + 2 == coord_last.get_y() && coord.get_x() - 1 == coord_last.get_x()) && (coord.get_y() + 2 <= 7 && coord.get_x() - 1 >= 0)) return true;
    if ((coord.get_y() + 2 == coord_last.get_y() && coord.get_x() + 1 == coord_last.get_x()) && (coord.get_y() + 2 <= 7 && coord.get_x() + 1 <= 7)) return true;
    return false;
};
TCoordMass& TKnight::get_list_coord() {
    TCoordMass* mass = new TCoordMass;//перебор вариантов
    if (coord.get_y() - 2 >= 0 && coord.get_x() - 1 >= 0) { mass->add_coord(coord.get_y() - 2, coord.get_x() - 1); }
    if (coord.get_y() - 2 >= 0 && coord.get_x() + 1 <= 7) { mass->add_coord(coord.get_y() - 2, coord.get_x() + 1); }
    if (coord.get_y() - 1 >= 0 && coord.get_x() + 2 <= 7) { mass->add_coord(coord.get_y() - 1, coord.get_x() + 2); }
    if (coord.get_y() + 1 <= 7 && coord.get_x() + 2 <= 7) { mass->add_coord(coord.get_y() + 1, coord.get_x() + 2); }
    if (coord.get_y() - 1 >= 0 && coord.get_x() - 2 >= 0) { mass->add_coord(coord.get_y() - 1, coord.get_x() - 2); }
    if (coord.get_y() + 1 <= 7 && coord.get_x() - 2 >= 0) { mass->add_coord(coord.get_y() + 1, coord.get_x() - 2); }
    if (coord.get_y() + 2 <= 7 && coord.get_x() - 1 >= 0) { mass->add_coord(coord.get_y() + 2, coord.get_x() - 1); }
    if (coord.get_y() + 2 <= 7 && coord.get_x() + 1 <= 7) { mass->add_coord(coord.get_y() + 2, coord.get_x() + 1); }
    return *mass;
};
void TKnight::move_to(TCoord coord_last) {
    coord.set_x(coord_last.get_x());
    coord.set_y(coord_last.get_y());
}

/*Массив фигур, нужен для корректной работы всех механизмов в классе TGame.
Должен содержать в себе перегрузки индексации, = , и другие, которые понадобятся в классе TGame
Он организует быстрый и удобный доступ к фигурам по координатам и по индексу,
если нужны какие-либо еще функции - добавляйте, я расписал лишь основные.
*/
class TFigure_mass {
    TFigure** mass;
    short int count;
public:
    TFigure* operator[](int i);
    TFigure* operator[](TCoord coord);
    void add_figure(TFigure* figure);
    TFigure_mass();
    ~TFigure_mass();
};
TFigure_mass::~TFigure_mass() {};//доделать
TFigure* TFigure_mass::operator[](int i) {
    if (i >= 0 && i < count) return mass[i];
    else throw invalid_index;
}
TFigure* TFigure_mass::operator[](TCoord coord) {
    for (int i = 0; i < count; i++) {
        if (mass[i]->get_coord() == coord) {
            return mass[i];
        }
    }
    throw invalid_index;
}
void TFigure_mass::add_figure(TFigure* figure) {
    TFigure** tmp_mass = new TFigure * [count + 1];
    for (int i = 0; i < count; i++) {
        tmp_mass[i] = mass[i];
    }
    tmp_mass[count] = figure;
    delete[] mass;//добавить отдельный цикл для удаления
    mass = tmp_mass;
    count++;
}
TFigure_mass::TFigure_mass() {
    count = 32;
    mass = new TFigure * [count];
    TCoord coord(0, 1);
    for (short int i = 0; i < 8; i++) {
        coord.set_x(i);
        mass[i] = new TPawn(i, coord, white);
    }
    coord.set_y(0);
    coord.set_x(0);
    mass[8] = new TRook(8, coord, white);
    coord.set_x(7);
    mass[9] = new TRook(9, coord, white);
    coord.set_x(1);
    mass[10] = new TKnight(10, coord, white);
    coord.set_x(6);
    mass[11] = new TKnight(11, coord, white);
    coord.set_x(2);
    mass[12] = new TBishop(12, coord, white);
    coord.set_x(5);
    mass[13] = new TBishop(13, coord, white);
    coord.set_x(3);
    mass[14] = new TQueen(14, coord, white);
    coord.set_x(4);
    mass[15] = new TKing(15, coord, white);

    coord.set_y(6);
    coord.set_x(0);
    for (short int i = 0; i < 8; i++) {
        coord.set_x(i);
        mass[16 + i] = new TPawn(16 + i, coord, black);
    }
    coord.set_y(7);
    coord.set_x(0);
    mass[24] = new TRook(24, coord, black);
    coord.set_x(7);
    mass[25] = new TRook(25, coord, black);
    coord.set_x(1);
    mass[26] = new TKnight(26, coord, black);
    coord.set_x(6);
    mass[27] = new TKnight(27, coord, black);
    coord.set_x(2);
    mass[28] = new TBishop(28, coord, black);
    coord.set_x(5);
    mass[29] = new TBishop(29, coord, black);
    coord.set_x(3);
    mass[30] = new TQueen(30, coord, black);
    coord.set_x(4);
    mass[31] = new TKing(31, coord, black);
}
//конструктор копиррования и деструктор дописать

//класс механик игры, содержит в себе массмв фигур, и методы с обработкой механик игры
class TGame {
    TFigure_mass mass;
    short int move_count;
    TCoordMass history; //доработать, не готово
public:
    TGame();
    TGame(TGame& tmp);
    void move(TCoord first_coord, TCoord last_coord);
    void eatten(TFigure* figure);
    bool check_possibility_move(TCoord fitst_coorf, TCoord last_coord);
    void castling();
    bool end_game();
};
TGame::TGame(TGame& tmp) {
    //mass = tmp.mass; нет перегрузки = для TFigure_mass, добавть.
    move_count = tmp.move_count;
    history = tmp.history;
}
TGame::TGame() {
    move_count = 0;
}

void TGame::move(TCoord first_coord, TCoord last_coord) {
    if (check_possibility_move(first_coord, last_coord)) {
        //history.add_coord(first_coord);
        //history.add_coord(last_coord);
        // в history пока ничего не пишем.
        move_count += 1;
        mass[first_coord]->move_to(last_coord);
        try {
            TFigure* figure = mass[last_coord];
            eatten(figure);
        }
        catch (...) {}
    }
}
void TGame::eatten(TFigure* figure) {
    figure->move_to(TCoord(-1, -1));
}
bool TGame::check_possibility_move(TCoord first_coord, TCoord last_coord) {
    TFigure* figure_1 = mass[first_coord];
    if (figure_1->get_figure_type() != pawn) {
        if (figure_1->check_move(last_coord)) {
            //Тут я проверяю наличие фигур между начальной и конечной координатой
            TCoordMass list_coord = figure_1->get_list_coord();
            int k = list_coord.get_count();
            int difference_x = last_coord.get_x() - first_coord.get_x();
            int difference_y = last_coord.get_y() - first_coord.get_y();
            for (int i = 0; i < k; i++) {
                if (difference_x == 0 && (list_coord[i].get_y() - first_coord.get_y()) / difference_y < 1) {
                    try {
                        TFigure* figure_2 = mass[list_coord[i]];
                        return 0;
                    }
                    catch (...) {
                        continue;
                    }
                }
                if (difference_y == 0 && (list_coord[i].get_x() - first_coord.get_x()) / difference_x < 1) {
                    try {
                        TFigure* figure_2 = mass[list_coord[i]];
                        return 0;
                    }
                    catch (...) {
                        continue;
                    }
                }
                if ((list_coord[i].get_x() - first_coord.get_x()) / difference_x < 1  && 
                    (list_coord[i].get_x() - first_coord.get_x()) / difference_x == 
                    (list_coord[i].get_y() - first_coord.get_y()) / difference_y) {
                    try {
                        TFigure* figure_2 = mass[list_coord[i]];
                        return 0;
                    }
                    catch (...) {
                        continue;
                    }
                }
            }
            //Если мы ничего не вернули во время цикла, то остается только проверить можем мы ли мы съесть фигуру на данной ячейке
            try {
                if (mass[last_coord]->get_figure_color() != figure_1->get_figure_color()) {
                    return 1;
                }
                else { return 0; }
            }
            catch (...) {
                return 1;
            }
        }
    }
    else {
        if (figure_1->check_move(last_coord)) {
            try {
                TFigure* figure_2 = mass[last_coord];
                return 0;
            }
            catch (...) {
                return 1;
            }
        }
        else 
        {
            if (abs(first_coord.get_x() - last_coord.get_x()) == 1 &&
                last_coord.get_y() - first_coord.get_y() == 1 &&
                figure_1->get_figure_color() == white) {
                try {
                    TFigure* figure_2 = mass[last_coord];
                    return 1;
                }
                catch (...) {
                    return 0;
                }
            }
            else
            {
                if (abs(first_coord.get_x() - last_coord.get_x()) == 1 &&
                    last_coord.get_y() - first_coord.get_y() == -1 &&
                    figure_1->get_figure_color() == black) {
                    try {
                        TFigure* figure_2 = mass[last_coord];
                        return 1;
                    }
                    catch (...) {
                        return 0;
                    }
                }
            }
        }
    }

}












//В данный момент в main код для проверки правильности работы вашей фигуры
int main()
{
    //создаем координату
    TCoord s;
    s.set_y(0);
    s.set_x(0);
    //вызывается конструктор TPawn::TPawn(short int _id, TCoord _coord, TFigure_color _color)
    //TPawn t1(1, s, white);
    TKnight t2(1, s, white);
    //меняем координату
    s.set_x(1);
    s.set_y(2);
    //проверка может ли фигура сходить на новую координату
    if (t2.check_move(s)) { cout << "First test: " << "TRUE TEST, check_move return true " << endl; }
    else { cout << "First test: " << "FALSE TEST, check_move return false " << endl; }
    //вывод списка координату куда она может сходить
    //TCoordMass mass_tmp = t1.get_list_coord();
    //mass_tmp.print();
    //cout << mass_tmp << endl;
    //std::cout << "Hello World!\n";
    //
}