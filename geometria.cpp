#include "geometria.h"
#include <math.h>
#include <stdlib.h>

   std::ostream& operator<<(std::ostream& os, Pont p){
        os<<p.getX()<<" "<<p.getY();
        return os;
    }

    std::ostream& operator<<(std::ostream& os, Kor k){
        os<<k.getP()<<" "<<k.getR();
        return os;
    }

    std::ostream& operator<<(std::ostream& os, Egyenes e){
        os<<e.getM()<<" "<<e.getC();
        return os;
    }

    double szog(Pont p1, Pont p2){
        return abs((p1.getY()-p2.getY())/(p1.getX()-p2.getX()));
    }

    int quarter(Pont p1, Pont p2){
        if(p2.getX()<=p1.getX() && p2.getY()>=p1.getY())
            return 2;
        if(p2.getX()>=p1.getX() && p2.getY()>=p1.getY())
            return 1;
        if(p2.getX()>=p1.getX() && p2.getY()<=p1.getY())
            return 4;
        return 3;
    }

    double kisebb(double x1, double x2){
        if(x1>=x2)
            return x2;
        return x1;
    }

    double nagyobb(double x1, double x2){
        if(x1>=x2)
            return x1;
        return x2;
    }

    double distance(Pont p1, Pont p2){
        return sqrt((p1.getX()-p2.getX())*(p1.getX()-p2.getX())+(p1.getY()-p2.getY())*(p1.getY()-p2.getY()));
    }

    bool meets(Egyenes e, Pont p1, Pont p2){
        Egyenes e2(p1, p2);
        double x = ((e.getC()-e2.getC()) / (e2.getM()-e.getM()));
        return (x >= kisebb(p1.getX(), p2.getX())) && (x <= nagyobb(p1.getX(), p2.getX()));
    }

    bool inside(Pont p1, Pont p2, Pont p3, Pont p4){
        int A=quarter(p2, p1);
        int C=quarter(p2, p3);
        int D=quarter(p2, p4);
        if(A==1){
            if(C==1){
                if(szog(p2, p1) < szog(p2, p3)){
                    if(D!=1)
                        return true;
                    return (szog(p2, p4) < szog(p2, p1)) || (szog(p2, p4) > szog(p2, p3));
                }
                if(D==1)
                    return (szog(p2, p4) < szog(p2, p1)) && (szog(p2, p4) > szog(p2, p3));
                return false;
            }
            if(C==2){
                if(D>=3)
                    return true;
                if(D==2)
                    return szog(p2, p4) < szog(p2, p3);
                return szog(p2, p4) < szog(p2, p1);
            }
            if(C==3){
                if(D==1)
                    return szog(p2, p4) < szog(p2, p1);
                if(D==2)
                    return false;
                if(D==3)
                    return szog(p2, p4) > szog(p2, p3);
                return true;
            }
            if(C==4){
                if(D==2 || D==3)
                    return false;
                if(D==1)
                    return szog(p2, p4) < szog(p2, p1);
                return szog(p2, p4) < szog(p2, p3);
            }
        }
        if(A==2){
            if(C==1){
                if(D>=3)
                    return false;
                if(D==1)
                    return szog(p2, p4) > szog(p2, p3);
                return szog(p2, p4) > szog(p2, p1);
            }
            if(C==2){
                if(szog(p2, p1) < szog(p2, p3)){
                    if(D==2)
                        return (szog(p2, p4) > szog(p2, p1)) && (szog(p2, p4) < szog(p2, p3));
                    return false;
                }
                if(D!=2)
                    return true;
                return (szog(p2, p4) > szog(p2, p1)) || (szog(p2, p4) < szog(p2, p3));
            }
            if(C==3){
                if(D==1 || D==4)
                    return true;
                if(D==2)
                    return szog(p2, p4) > szog(p2, p1);
                return szog(p2, p4) > szog(p2, p3);
            }
            if(C==4){
                if(D==1)
                    return true;
                if(D==3)
                    return false;
                if(D==2)
                    return szog(p2, p4) < szog(p2, p3);
                return szog(p2, p4) > szog(p2, p1);
            }
        }
        if(A==3){
            if(C==1){
                if(D==2)
                    return true;
                if(D==4)
                    return false;
                if(D==1)
                    return szog(p2, p4) > szog(p2, p3);
                return szog(p2, p4) < szog(p2, p1);
            }
            if(C==2){
                if(D==1 || D==4)
                    return false;
                if(D==2)
                    return szog(p2, p4) < szog(p2, p3);
                return szog(p2, p4) < szog(p2, p1);
            }
            if(C==3){
                if(szog(p2, p1) > szog(p2, p3)){
                    if(D==3)
                        return (szog(p2, p4) < szog(p2, p1)) && (szog(p2, p4) > szog(p2, p3));
                    return false;
                }
                if(D!=3)
                    return true;
                return (szog(p2, p4) < szog(p2, p1)) || (szog(p2, p4) > szog(p2, p3));
            }
            if(C==4){
                if(D<=2)
                    return true;
                if(D==3)
                    return szog(p2, p4) < szog(p2, p1);
                return szog(p2, p4) < szog(p2, p3);
            }
        }
        if(A==4){
            if(C==1){
                if(D==2 || D==3)
                    return true;
                if(D==1)
                    return szog(p2, p4) > szog(p2, p3);
                return szog(p2, p4) > szog(p2, p1);
            }
            if(C==2){
                if(D==3)
                    return true;
                if(D==1)
                    return false;
                if(D==2)
                    return szog(p2, p4) < szog(p2, p3);
                return szog(p2, p4) > szog(p2, p1);
            }
            if(C==3){
                if(D<=2)
                    return false;
                if(D==3)
                    return szog(p2, p4) > szog(p2, p3);
                return szog(p2, p4) > szog(p2, p1);
            }
            if(C==4){
                if(szog(p2, p1) > szog(p2, p3)){
                    if(D!=4)
                        return true;
                    return (szog(p2, p4) > szog(p2, p1)) || (szog(p2, p4) < szog(p2, p3));
                }
                if(D==4)
                    return (szog(p2, p4) > szog(p2, p1)) && (szog(p2, p4) < szog(p2, p3));
                return false;
            }
        }
    }

    bool valid(Pont p1, Pont p2, Pont p3, Pont p4, Pont p5, Pont p6){
        if((!inside(p1, p2, p3, p6)))
            return false;
        if(!(inside(p2, p3, p4, p5)))
            return false;
        Egyenes e1(p3, p5);
        if(!(korbejar(e1, &p4, &p4+1, p5, 1)))
            return false;
        Egyenes e2(p2, p6);
        if(!(korbejar(e1, &p1, &p1-1, p6, -1)))
            return false;
        return true;
    }

    bool korbejar(Egyenes e, Pont* pp1, Pont* pp2, Pont p, int dir){
        /*while((*pp2)!=p && (*pp1)!=p){
            if(meets(e, *pp1, *pp1))
                return false;
            pp1 += dir;
            pp2 += dir;
        }*/
        return true;
    }

    double triarea(double a, double b, double c){
        double r = a + b + c;
        return r*(r-a)*(r-b)*(r-c);
    }

    double area(Pont p1, Pont p2, Pont p3, Pont p4){
        double p1p2 = distance(p1, p2);
        double p1p4 = distance(p1, p4);
        double p2p3 = distance(p2, p3);
        double p2p4 = distance(p2, p4);
        double p3p4 = distance (p3, p4);
        return triarea(p1p2, p1p4, p2p4) + triarea(p2p4, p3p4, p2p3);
    }

    void koregyenes(Pont& p1, Pont& p2, Egyenes e, Kor k, int& db){
        double D = (-2*k.getP().getX()+2*e.getC()*e.getM()-2*e.getM()*k.getP().getY())*
        (-2*k.getP().getX()+2*e.getC()*e.getM()-2*e.getM()*k.getP().getY())-
        4*(1+e.getM()*e.getM())*(-2*e.getC()*k.getP().getY()+e.getC()*e.getC()+k.getP().getY()*k.getP().getY()-k.getR()*k.getR());
        double x1, y1, x2, y2;
        if(D==0){
            db=1;
            x1=(2*k.getP().getX()-2*e.getC()*e.getM()+2*e.getM()*k.getP().getY())/(2+2*e.getM()*e.getM());
            y1=e.getM()*x1+e.getC();
            p1=Pont(x1, y1);
        }
        else if(D>0){
            db=2;
            x1=(2*k.getP().getX()-2*e.getC()*e.getM()+2*e.getM()*k.getP().getY()+sqrt(D))/(2+2*e.getM()*e.getM());
            x2=(2*k.getP().getX()-2*e.getC()*e.getM()+2*e.getM()*k.getP().getY()-sqrt(D))/(2+2*e.getM()*e.getM());
            y1=e.getM()*x1+e.getC();
            y2=e.getM()*x2+e.getC();
            p1=Pont(x1, y1);
            p2=Pont(x2, y2);
        }
        else{
            db=0;
        }
    }
