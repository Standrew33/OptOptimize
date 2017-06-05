#include "population.h"

Population::Population()
{
    RGenes.clear();
    BGenes.clear();
}

void Population::RGeneric(int N)
{ 
    if (BGenes.length() == 0)
    {
        for (int i = 0; i < N; i++)
        {
            BGenes.append(Gen);
            BGenes[i].RRate = qrand() % 29 + 2;
            BGenes[i].RTake = false;
            for (int j = 0; j <= 4; j++)
                BGenes[i].RFit.append(0);
        }
    }
    else
    {
        for (int i = BGenes.length(); i < N; i++)
        {
            BGenes.append(Gen);
            BGenes[i].RRate = qrand() % 29 + 2;
            for (int j = 0; j <= 4; j++)
                BGenes[i].RFit.append(0);
        }
    }
    for (int i = 0; i < N; i++)
    {
        BGenes[i].RTake = false;
    }
}

void Population::RSExcel()
{
    QAxObject* excel = new QAxObject("Excel.Application", 0);
    QAxObject* workbooks = excel->querySubObject("Workbooks");
    QAxObject* workbook = workbooks->querySubObject("Open(const QString&)",
                                                    "C:\\Work\\data.xlsx");
    QAxObject* sheets = workbook->querySubObject("Worksheets");
    QAxObject* sheet = sheets->querySubObject("Item(int)", 1);

    for (int j = 1; j <= 6; j++)
        for (int i = 1; i <= 1080; i++)
        {
            QAxObject* cell = sheet->querySubObject("Cells(int,int)", i, j);
            QVariant value = cell->property("Value");
            RListRate[j - 1][i - 1] = value.toDouble();
        }

    workbook->dynamicCall("Close()");
    excel->dynamicCall("Quit()");
}

void Population::RFitness(int RBus)
{
    for (int q = 0; q <= BGenes.length() - 1; q++)
    {
        for (int j = 0; j <= 4; j++)
            BGenes[q].RFit[j] = 0;
        BGenes[q].RWaitTPas = 0;
        BGenes[q].RNBus = 0;

        for (int i = 1; i <= 6; i++)
        {
            int RTotalTime = (i - 1) * BGenes[q].RRate;
            double RTimeBus = 0;
            while (RTimeBus + RTotalTime <= 1080)
            {
                for (int j = 1; j <= RBus; j++)
                {
                    int TMin = BGenes[q].RRate;
                    int TMax = BGenes[q].RRate + 2;
                    RTimeBus = qrand() % (TMax - TMin) + TMin;
                    if (RTimeBus + RTotalTime > 1080)
                        break;

                    double T = RTimeBus - BGenes[q].RRate;
                    double P = RPasRate(RTotalTime, RTimeBus + RTotalTime, i);

                    if (RTotalTime <= 60)
                        RIntegral(0, P, T, q, RTotalTime, RTimeBus);
                    else if (RTotalTime <= 240)
                       RIntegral(1, P, T, q, RTotalTime, RTimeBus);
                    else if (RTotalTime <= 660)
                        RIntegral(2, P, T, q, RTotalTime, RTimeBus);
                    else if (RTotalTime <= 780)
                        RIntegral(3, P, T, q, RTotalTime, RTimeBus);
                    else
                        RIntegral(4, P, T, q, RTotalTime, RTimeBus);

                    BGenes[q].RWaitTPas += T;
                    if (i == 1)
                        BGenes[q].RNBus += 1;
                    RTotalTime += RTimeBus;
                }
            }
        }
        for (int j = 0; j <= 4; j++)
            BGenes[q].RFit[j] = fabs(BGenes[q].RWaitTPas * BGenes[q].RFit[j] + BGenes[q].RNBus);
    }
}

void Population::RIntegral(int Index, double P, double T, int Id, int RSTimeBus, double RETimeBus)
{
    BGenes[Id].RFit[Index] += P * T * RETimeBus - P * T * RSTimeBus;
}

double Population::RPasRate(int RStartRate, double REndRate, int RStation)
{
    double P = 0;
    for (int Row = RStartRate; Row <= REndRate - 1; Row++)
        P += RListRate[RStation - 1][Row];

    return P;
}

void Population::RFindMax()
{
    /*for (int i = 0; i <= 4; i++)
    {
        RMaxMinList[i] = 0;
        int Id = -1;
        for (int j = 0; j <= BGenes.length() - 1; j++)
        {
            if (BGenes[j].RFit[i] > RMaxMinList[i] && BGenes[j].RTake == false)
            {
                if (Id != -1)
                    BGenes[Id].RTake = false;
                RMaxMinList[i] = BGenes[j].RFit[i];
                BGenes[j].RTake = true;
                Id = j;
            }
        }
        RMaxMinList[i] += 10000;
        for (int j = 0; j <= BGenes.length() - 1; j++)
            BGenes[j].RFit[i] = RMaxMinList[i] - BGenes[j].RFit[i];
    }*/
}

void Population::Selection()
{
    for (int i = 0; i <= BGenes.length() - 1; i++)
        BGenes[i].RTake = false;

    double array[5];
    for (int j = 0; j <= 4; j++)
    {
        double Sum = 0;
        int Id = -1;
        RMaxMinList[j] = 999999999999;
        for (int i = 0; i <= BGenes.length() - 1; i++)
            Sum += BGenes[i].RFit[j];

        for (int i = 0; i <= BGenes.length() - 1; i++)
        {
            if (BGenes[i].RProb.length() != 5)
                BGenes[i].RProb.append(0);
            BGenes[i].RProb[j] = BGenes[i].RFit[j] / Sum;
            if (BGenes[i].RProb[j] < RMaxMinList[j] && BGenes[i].RTake == false)
            {
                if (Id != -1)
                    BGenes[Id].RTake = false;
                RMaxMinList[j] = BGenes[i].RProb[j];
                BGenes[i].RTake = true;
                Id = i;
                array[j] = Id;
            }
        }
    }
    for (int i = 0; i <= 4; i++)
        RGenes.append(BGenes[array[i]]);
    BGenes.clear();
}

void Population::Crossing(double RSizeNP)
{
    double RNChrom = 0;
    int Num = -1;
    for (int i = 0; i <= 4; i++)
    {
        RNChrom = qRound(RSizeNP) / 10 - i;
        for (int j = i + 1; j <= 4; j++)
            for (int k = 0; k <= RNChrom + 1; k++)
            {
                if (BGenes.length() >= RSizeNP)
                        break;
                BGenes.append(RGenes[i]);
                Num += 1;
                int RParrent = qrand() % 2;
                if (RParrent == 1)
                    BGenes[Num].RRate = RGenes[j].RRate;
            }
    }
    RGenes.clear();
}

void Population::Mutation(double RSizeMut)
{
    int rnd = rand() % BGenes.length();
    for (int i = 0; i <= qRound(RSizeMut); i++)
    {
        while (BGenes[rnd].RTake == false)
            rnd = rand() % BGenes.length();
        BGenes[rnd].RRate = rand() % 29 + 2;
        BGenes[rnd].RTake = false;
    }
}

double Population::RFindMin()
{
    double RMin = 999999999999;
    for (int j = 0; j <= 4; j++)
    {
        for (int i = 0; i <= BGenes.length() - 1; i++)
        {
            if (BGenes[i].RFit[j] < RMin)
                RMin = BGenes[i].RFit[j];
        }
    }

    return fabs(RMin);
}
