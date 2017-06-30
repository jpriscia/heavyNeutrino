#ifndef GEN_ANALYZER_H
#define GEN_ANALYZER_H
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"

#include "heavyNeutrino/multilep/plugins/multilep.h"

#include "TTree.h"

class multilep;

class GenAnalyzer {
  private:
    static const unsigned gen_nL_max = 20;
    static const unsigned gen_nPh_max = 10;
   
    int    _ttgEventType;
    int    _zgEventType;
    float  _nTrueInt;

    //generator level MET
    double   _gen_met;
    double   _gen_metPhi;
    //Generator photons
    unsigned _gen_nPh;
    double   _gen_phPt[gen_nPh_max];
    double   _gen_phEta[gen_nPh_max];
    double   _gen_phPhi[gen_nPh_max];
    double   _gen_phE[gen_nPh_max];
    int      _gen_phMomPdg[gen_nPh_max];
    bool     _gen_phIsPrompt[gen_nPh_max];
    //Generator leptons
    unsigned _gen_nL;
    double   _gen_lPt[gen_nL_max];
    double   _gen_lEta[gen_nL_max];
    double   _gen_lPhi[gen_nL_max];
    double   _gen_lE[gen_nL_max];
    unsigned _gen_lFlavor[gen_nL_max];
    int      _gen_lCharge[gen_nL_max];
    int      _gen_lMomPdg[gen_nL_max];
    bool     _gen_lIsPrompt[gen_nL_max];

    //Functions to find the mother of a gen particle
    const reco::GenParticle* getMother(const reco::GenParticle&, const std::vector<reco::GenParticle>&);
    const int                getMotherPdgId(const reco::GenParticle&, const std::vector<reco::GenParticle>&);
    void                     getMotherList(const reco::GenParticle&, const std::vector<reco::GenParticle>&, std::vector<int>&);
    bool                     inMotherList(std::vector<int>& list, int i);
    int                      ttgEventType(const std::vector<reco::GenParticle>& genParticles, double ptCut, double etaCut);

    multilep* multilepAnalyzer;

  public:
    GenAnalyzer(const edm::ParameterSet& iConfig, multilep* vars);
    ~GenAnalyzer();

    void beginJob(TTree* outputTree);
    void analyze(const edm::Event&);
};

#endif
