#ifndef ESTRUCTURAS_REPAIR_H
#define ESTRUCTURAS_REPAIR_H

struct TendencyParameters{
    int mmi, mmd, ppi, ppd, mp, pm;
    
    TendencyParameters(int mmi_, int mmd_, int ppi_, int ppd_, int mp_, int pm_)
        : mmi(mmi_), mmd(mmd_), ppi(ppi_), ppd(ppd_), mp(mp_), pm(pm_){}
} typedef TendencyParameters;

#endif /* ESTRUCTURAS_REPAIR_H */

