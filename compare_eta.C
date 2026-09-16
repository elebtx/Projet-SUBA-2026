#include <TFile.h>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>

void compare_eta() {
    gStyle->SetOptStat(0);

    TFile *f7 = TFile::Open("eta_muon_7TeV.root");
    TFile *f13 = TFile::Open("eta_muon_13TeV.root");

    TH1D *h7 = (TH1D*)f7->Get("h_etaMuon");
    TH1D *h13 = (TH1D*)f13->Get("h_etaMuon");
    h7->SetStats(0);
    h13->SetStats(0);
    h7->GetListOfFunctions()->Clear();
    h13->GetListOfFunctions()->Clear();

    h7->Scale(1.0 / h7->Integral());
    h13->Scale(1.0 / h13->Integral());

    TCanvas *c1 = new TCanvas("c1", "Comparaison eta muons", 800, 600);

    h7->SetTitle("Distribution normalisee de #eta des muons;#eta_{#mu};Fraction d'evenements");
    h7->SetLineWidth(2);
    h13->SetLineWidth(2);

    h7->SetLineColor(kBlue);
    h13->SetLineColor(kRed);

    h7->Draw("HIST");
    h13->Draw("HIST SAME");

    TLegend *leg = new TLegend(0.65, 0.75, 0.88, 0.88);
    leg->AddEntry(h7, "7 TeV", "l");
    leg->AddEntry(h13, "13 TeV", "l");
    leg->Draw();

    c1->SaveAs("compare_eta.png");
}



