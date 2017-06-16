#include "heavyNeutrino/multilep/interface/PhotonAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

/*
 * Calculating all photon-related variables
 */


PhotonAnalyzer::PhotonAnalyzer(const edm::ParameterSet& iConfig, multilep* multilepAnalyzer):
  multilepAnalyzer(multilepAnalyzer)
{};


void PhotonAnalyzer::beginJob(TTree* outputTree){
  outputTree->Branch("_nPh",                      &_nPh,                      "_nPh/b");
  outputTree->Branch("_phPt",                     &_phPt,                     "_phPt[_nPh]/D");
  outputTree->Branch("_phEta",                    &_phEta,                    "_phEta[_nPh]/D");
  outputTree->Branch("_phPhi",                    &_phPhi,                    "_phPhi[_nPh]/D");
  outputTree->Branch("_phE",                      &_phE,                      "_phE[_nPh]/D");
  outputTree->Branch("_phCutBasedLoose",          &_phCutBasedLoose,          "_phCutBasedLoose[_nPh]/O");
  outputTree->Branch("_phCutBasedMedium",         &_phCutBasedMedium,         "_phCutBasedMedium[_nPh]/O");
  outputTree->Branch("_phCutBasedLoose",          &_phCutBasedLoose,          "_phCutBasedLoose[_nPh]/O");
  outputTree->Branch("_phMva",                    &_phMva,                    "_phMva[_nPh]/D");
  outputTree->Branch("_phChargedIsolation",       &_phChargedIsolation,       "_phChargedIsolation[_nPh]/D");
  outputTree->Branch("_phNeutralHadronIsolation", &_phNeutralHadronIsolation, "_phNeutralHadronIsolation[_nPh]/D");
  outputTree->Branch("_phSigmaIetaIeta",          &_phSigmaIetaIeta,          "_phSigmaIetaIeta[_nPh]/D");
  outputTree->Branch("_phHadronicOverEm",         &_phHadronicOverEm,         "_phHadronicOverEm[_nPh]/D");
  outputTree->Branch("_phPassElectronVeto",       &_phPassElectronVeto,       "_phPassElectronVeto[_nPh]/O");
  outputTree->Branch("_phHasPixelSeed",           &_phHasPixelSeed,           "_phHasPixelSeed[_nPh]/O");
}

bool PhotonAnalyzer::analyze(const edm::Event& iEvent){
  edm::Handle<std::vector<pat::Photon>> photons;                   iEvent.getByToken(multilepAnalyzer->photonToken,                       photons);
  edm::Handle<edm::ValueMap<bool>> photonsCutBasedLoose;           iEvent.getByToken(multilepAnalyzer->photonCutBasedLooseToken,          photonsCutBasedLoose);
  edm::Handle<edm::ValueMap<bool>> photonsCutBasedMedium;          iEvent.getByToken(multilepAnalyzer->photonCutBasedMediumToken,         photonsCutBasedMedium);
  edm::Handle<edm::ValueMap<bool>> photonsCutBasedTight;           iEvent.getByToken(multilepAnalyzer->photonCutBasedTightToken,          photonsCutBasedTight);
  edm::Handle<edm::ValueMap<float>> photonsMva;                    iEvent.getByToken(multilepAnalyzer->photonMvaToken,                    photonsMva);
  edm::Handle<edm::ValueMap<float>> photonsChargedIsolation;       iEvent.getByToken(multilepAnalyzer->photonChargedIsolationToken,       photonsChargedIsolation);
  edm::Handle<edm::ValueMap<float>> photonsNeutralHadronIsolation; iEvent.getByToken(multilepAnalyzer->photonNeutralHadronIsolationToken, photonsNeutralHadronIsolation);
  edm::Handle<edm::ValueMap<float>> photonsPhotonIsolation;        iEvent.getByToken(multilepAnalyzer->photonPhotonIsolationToken,        photonsPhotonIsolation);

  // Loop over photons
  _nPh = 0;
  for(auto photon = photons->begin(); photon != photons->end(); ++photon){
    if(_nPh == nPhoton_max) break;
    const auto photonRef = edm::Ref<std::vector<pat::Photon>>(photons, (photon - photons->begin()));

    if(photon->pt()  < 10)  continue;
    if(photon->eta() > 2.5) continue;

    _phPt[_nPh]                      = photon->pt();
    _phEta[_nPh]                     = photon->eta();
    _phPhi[_nPh]                     = photon->phi();
    _phE[_nPh]                       = photon->energy();
    _phCutBasedLoose[_nPh]           = (*photonsCutBasedLoose)[photonRef];
    _phCutBasedMedium[_nPh]          = (*photonsCutBasedMedium)[photonRef];
    _phCutBasedTight[_nPh]           = (*photonsCutBasedTight)[photonRef];
    _phMva[_nPh]                     = (*photonsMva)[photonRef];
    _phChargedIsolation[_nPh]        = (*photonsChargedIsolation)[photonRef];
    _phNeutralHadronIsolation[_nPh]  = (*photonsNeutralHadronIsolation)[photonRef];
    _phPhotonIsolation[_nPh]         = (*photonsPhotonIsolation)[photonRef];
    _phSigmaIetaIeta[_nPh]           = photon->full5x5_sigmaIetaIeta();
    _phHadronicOverEm[_nPh]          = photon->hadronicOverEm();
    _phPassElectronVeto[_nPh]        = photon->passElectronVeto();
    _phHasPixelSeed[_nPh]            = photon->hasPixelSeed();

    ++_nPh;
  }

  if(multilepAnalyzer->skim == "ttg" and _nPh < 1) return false;
  return true;
}
