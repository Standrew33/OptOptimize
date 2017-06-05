#ifndef POPULATION_H
#define POPULATION_H
#include "genes.h"
#include <QAxObject>
#include <QtGui>
#include <cmath>

class Population
{
private:
    Genes Gen;
    QList<Genes> RGenes;
    QList<Genes> BGenes;
    double RMaxMinList[5];
    double RSumFit[2];
    double RPMinList[1];
    void RIntegral(int Index, double P, double T, int Id, int RSTimeBus, double RETimeBus);
    double RPasRate(int RStartRate, double REndRate, int RStation);

public:
    Population();
    double RListRate[6][1080];
    void RGeneric(int N);

    void RFitness(int RBus);
    void RFindMax();
    double RFindMin();

    void Selection();
    void Crossing(double RSizeNP);
    void Mutation(double RSizeMut);

    void RSExcel();
};

#endif // POPULATION_H
