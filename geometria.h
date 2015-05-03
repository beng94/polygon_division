#ifndef GEOMETRIA_H_INCLUDED
#define GEOMETRIA_H_INCLUDED
#include <iostream>

class Pont{
 double x, y;
public:
    Pont(double x=0, double y=0): x(x), y(y){};

    double& getX(){ return x; };

    double& getY(){ return y; };

    Pont& operator=(const Pont rhs_p){
        x=rhs_p.x;
        y=rhs_p.y;
        return *this;
    }

    Pont operator+(const Pont rhs_p){
        return Pont(x+rhs_p.x, y+rhs_p.y);
    }

    Pont operator-(const Pont rhs_p){
        return Pont(x-rhs_p.x, y-rhs_p.y);
    }
};

std::ostream& operator<<(std::ostream& os, Pont p);

double tangens(Pont p1, Pont p2);

class Kor{
 Pont P0;
 double r;
public:
    Kor(Pont P0=Pont(), double r=0): P0(P0), r(r){};

    Kor(double x0, double y0, double r): P0(Pont(x0, y0)), r(r){};

    Pont& getP(){ return P0; };

    double& getR(){ return r; };
};

std::ostream& operator<<(std::ostream& os, Kor k);

class Egyenes{
    double m, c;
public:
    Egyenes(double m=0, double c=0): m(m), c(c){};

    Egyenes(Pont p1, Pont p2){
     if(p1.getX()<=p2.getX() && p1.getY()<=p2.getY()){
        m=(p2.getY()-p1.getY())/(p2.getX()-p1.getX());
        c=p1.getY()-p1.getX()*m;
     }

     else if(p1.getX()>=p2.getX() && p1.getY()>=p1.getY()){
        m=(p1.getY()-p2.getY())/(p1.getX()-p2.getX());
        c=p2.getY()-p2.getX()*m;
     }

     else if(p1.getX()<=p2.getX() && p1.getY()>=p2.getY()){
        m=(p1.getY()-p2.getY())/(p1.getX()-p2.getX());
        c=p1.getY()+p1.getX()*((p1.getY()-p2.getY())/(p2.getX()-p1.getX()));
     }

     else{
        m=(p2.getY()-p1.getY())/(p2.getX()-p1.getX());
        c=p2.getY()+p2.getX()*((p2.getY()-p1.getY())/(p1.getX()-p2.getX()));
     }
    }

    double& getC(){ return c; };
    double& getM(){ return m; };
};

std::ostream& operator<<(std::ostream& os, Egyenes e);

double szog(Pont p1, Pont p2);

bool inside(Pont p1, Pont p2, Pont p3, Pont p4);

double distance(Pont p1, Pont p2);

int quarter(Pont p1, Pont p2);

bool meets(Egyenes e, Pont p1, Pont p2);

double kisebb(double x1, double x2);

double nagyobb(double x1, double x2);

bool valid(Pont p1, Pont p2, Pont p3, Pont p4, Pont p5, Pont p6);

bool korbejar(Egyenes e, Pont* pp1, Pont* pp2, Pont p, int dir);

double area(Pont p1, Pont p2, Pont p3, Pont p4);

double triarea(double a, double b, double c);

void koregyenes(Pont& p1, Pont& p2, Egyenes e, Kor k, int& db);

#endif // GEOMETRIA_H_INCLUDED
