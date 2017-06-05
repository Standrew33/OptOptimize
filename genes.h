#ifndef GENES_H
#define GENES_H
#include <QList>

class Genes
{
public:
    Genes();
    int RWaitTPas;
    int RNBus;
    int RRate;
    bool RTake;
    QList<double> RProb;
    QList<double> RFit;
};

#endif // GENES_H
