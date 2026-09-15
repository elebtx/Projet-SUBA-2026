//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Sep 10 15:31:47 2026 by ROOT version 6.40.04
// from TTree Events/LHE events
// found on file: nominal.root
//////////////////////////////////////////////////////////

#ifndef AnalyseZ_h
#define AnalyseZ_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

// Headers needed by this particular selector
#include <vector>
#include <TH1D.h>



class AnalyseZ : public TSelector {
public :
   TTreeReader     fReader;  ///<!the tree reader
   TTree          *fChain = 0;   ///<!pointer to the analyzed TTree or TChain

   TH1D *h_eta_tot = nullptr;  // Dénominateur : Tous les muons des événements pré-sélectionnés
   TH1D *h_eta_pass = nullptr; // Numérateur : Muons passant la sélection finale
   TH1D *h_masse_Z = nullptr;  // Histogramme pour la masse invariante du Z
   
   // Readers to access the data (delete the ones you do not need).
   TTreeReaderValue<Long64_t> EventNumber = {fReader, "EventNumber"};
   TTreeReaderValue<Int_t> NUP = {fReader, "NUP"};
   TTreeReaderValue<Int_t> IDPRUP = {fReader, "IDPRUP"};
   TTreeReaderValue<Double_t> XWGTUP = {fReader, "XWGTUP"};
   TTreeReaderValue<Double_t> SCALUP = {fReader, "SCALUP"};
   TTreeReaderValue<Double_t> AQEDUP = {fReader, "AQEDUP"};
   TTreeReaderValue<Double_t> AQCDUP = {fReader, "AQCDUP"};
   TTreeReaderArray<int> IDUP = {fReader, "IDUP"};
   TTreeReaderArray<int> ISTUP = {fReader, "ISTUP"};
   TTreeReaderArray<int> MOTH1 = {fReader, "MOTH1"};
   TTreeReaderArray<int> MOTH2 = {fReader, "MOTH2"};
   TTreeReaderArray<int> ICOL1 = {fReader, "ICOL1"};
   TTreeReaderArray<int> ICOL2 = {fReader, "ICOL2"};
   TTreeReaderArray<double> PX = {fReader, "PX"};
   TTreeReaderArray<double> PY = {fReader, "PY"};
   TTreeReaderArray<double> PZ = {fReader, "PZ"};
   TTreeReaderArray<double> E = {fReader, "E"};
   TTreeReaderArray<double> M = {fReader, "M"};
   TTreeReaderArray<double> VTIMUP = {fReader, "VTIMUP"};
   TTreeReaderArray<double> SPINUP = {fReader, "SPINUP"};


   AnalyseZ(TTree * /*tree*/ =0) { }
   ~AnalyseZ() override { }
   Int_t   Version() const override { return 2; }
   void    Begin(TTree *tree) override;
   void    SlaveBegin(TTree *tree) override;
   void    Init(TTree *tree) override;
   bool    Notify() override;
   bool    Process(Long64_t entry) override;
   Int_t   GetEntry(Long64_t entry, Int_t getall = 0) override { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   void    SetOption(const char *option) override { fOption = option; }
   void    SetObject(TObject *obj) override { fObject = obj; }
   void    SetInputList(TList *input) override { fInput = input; }
   TList  *GetOutputList() const override { return fOutput; }
   void    SlaveTerminate() override;
   void    Terminate() override;

   ClassDefOverride(AnalyseZ,0);

};

#endif

#ifdef AnalyseZ_cxx
void AnalyseZ::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the reader is initialized.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.

   fReader.SetTree(tree);
}

bool AnalyseZ::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be for a new TTree in a TChain.
   // It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return true;
}


#endif // #ifdef AnalyseZ_cxx
