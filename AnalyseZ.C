#define AnalyseZ_cxx
#include "AnalyseZ.h"
#include <TH2.h>
#include <TStyle.h>
#include <cmath>
#include <iostream>
#include <TEfficiency.h> 
#include <TLorentzVector.h> // Indispensable pour calculer pT et Eta
#include <TCanvas.h>
#include <TGraphAsymmErrors.h>
#include <TF1.h>
#include <TFitResult.h>
#include <TStyle.h>


// Variables globales pour le calcul de l'efficacite 
int N_tot = 0;
int N_pass = 0;

void AnalyseZ::Begin(TTree * /*tree*/) {
   
    //TString option = GetOption();
   N_tot = 0;
   N_pass = 0;

   // Création des histogrammes (30 bins de largeur 0.2 entre -3.0 et 3.0)
   h_eta_tot = new TH1D("h_eta_tot", "Tous les muons", 30, -3.0, 3.0);
   h_eta_pass = new TH1D("h_eta_pass", "Muons selectionnes", 30, -3.0, 3.0);
   //60 bins entre 60 et 120 GeV pour bien encadrer le pic (situé vers 91 GeV)
   h_masse_Z = new TH1D("h_masse_Z", "Masse invariante des muons;m_{#mu#mu} [GeV];Nombre d'evenements", 60, 60.0, 120.0);
   
   // Important pour le calcul correct des incertitudes
   h_eta_tot->Sumw2();
   h_eta_pass->Sumw2();
   h_masse_Z->Sumw2();
}

void AnalyseZ::SlaveBegin(TTree * /*tree*/) {
   TString option = GetOption();
}

bool AnalyseZ::Process(Long64_t entry) {
   
    fReader.SetLocalEntry(entry);
   N_tot++; // Incrementation du denominateur pour l'efficacité globale

   int n_muons = 0;
   bool pass_kinematics = false;
   std::vector<TLorentzVector> muons_event; // Pour stocker les muons de l'événement

   // Boucle sur les particules de l'événement (IDUP contient l'identifiant de la particule)
   for (size_t i = 0; i < IDUP.GetSize(); ++i) {
       
       // Code PID 13 = muon, -13 = anti-muon
       if (std::abs(IDUP[i]) == 13) {
           n_muons++;
           
           // Reconstruction du vecteur cinématique de la particule
           TLorentzVector muon;
           muon.SetPxPyPzE(PX[i], PY[i], PZ[i], E[i]);
           muons_event.push_back(muon);
       }
   }

   // Pré-sélection : exactement deux muons dans l'état final
   if (n_muons != 2) return true;

   // Remplissage de l'efficacité différentielle
   for (size_t i = 0; i < muons_event.size(); ++i) {
       // Dénominateur : on enregistre le eta de tous les muons pré-sélectionnés
       h_eta_tot->Fill(muons_event[i].Eta());

       // Séléction finale : 
       if (muons_event[i].Pt() > 25.0 && std::abs(muons_event[i].Eta()) < 2.4) {
           h_eta_pass->Fill(muons_event[i].Eta());
           pass_kinematics = true; // Valide la sélection finale pour l'événement global
       }
   }

   if (!pass_kinematics) return true;

   N_pass++; // Incrémentation du numérateur si l'événement survit

   // Calcul de la masse invariante
   TLorentzVector Z = muons_event[0] + muons_event[1]; 
   h_masse_Z->Fill(Z.M()); 

   return true;
}

void AnalyseZ::SlaveTerminate(){
}

void AnalyseZ::Terminate(){

   // Evite les divisions par 0
   if (N_tot == 0) return;
   
   // Calcul de l'efficacité globale 
   double eff = (double)N_pass / N_tot;
   
   // Incertitude 1 : Approximation binomiale classique
   double err_binom = std::sqrt(eff * (1.0 - eff) / N_tot);
   
   // Incertitude 2 : Méthode de Clopper-Pearson (intervalle à 1 sigma)
   double cp_lower = TEfficiency::ClopperPearson(N_tot, N_pass, 0.68, false);
   double cp_upper = TEfficiency::ClopperPearson(N_tot, N_pass, 0.68, true);
   
   std::cout << "\n=== RESULTATS DE LA SELECTION ===" << std::endl;
   std::cout << "Evenements totaux (N_tot)   : " << N_tot << std::endl;
   std::cout << "Evenements retenus (N_pass) : " << N_pass << std::endl;
   std::cout << "Efficacite globale (eps)    : " << eff << std::endl;
   std::cout << "Incertitude Binomiale       : +/- " << err_binom << std::endl;
   std::cout << "Incertitude Clopper-Pearson : -" << (eff - cp_lower) << " / +" << (cp_upper - eff) << std::endl;
   std::cout << "=================================\n" << std::endl;

   // Affiche les paramètres des fits sur les graphiques
   gStyle->SetOptFit(1111);

   // === TRACÉ ET FIT DE L'EFFICACITÉ DIFFÉRENTIELLE ===
   TCanvas *c1 = new TCanvas("c1", "Efficacite Differentielle", 800, 600);
   c1->SetGrid(); 

   // l'option "cp" indique de calculer l'incertitude de Clopper-Pearson
   TGraphAsymmErrors *gr_eff = new TGraphAsymmErrors(h_eta_pass, h_eta_tot, "cp");
   
   // Esthétique du graphique
   gr_eff->SetTitle("Efficacite de la selection en fonction de #eta;#eta ;Efficacite #epsilon");
   gr_eff->SetMarkerStyle(20);        
   gr_eff->SetMarkerColor(kBlue+1);
   gr_eff->SetLineColor(kBlue+1);
   
   // A = Dessine les axes, P = Dessine les points
   gr_eff->Draw("AP"); 
   
   // Ajustement de l'axe Y pour bien voir l'efficacité (choisi ad hoc)
   gr_eff->GetYaxis()->SetRangeUser(0.70, 1.1);

   
   // "pol2" correspond à la fonction p0 + p1*x + p2*x^2
   TF1 *fit_eff = new TF1("fit_eff", "pol2", -2.4, 2.4); 
   fit_eff->SetLineColor(kRed);
   
   std::cout << "\n--- Fit de l'efficacite ---" << std::endl;
  
   TFitResultPtr r_eff = gr_eff->Fit("fit_eff", "R S");  // L'option "R" limite le fit à l'intervall défini ci-dessus et l'option "S" sauvegarde les résultats pour récupérer le Chi2

   // Évaluation du fit 
   if (r_eff == 0) {
       double chi2_eff = r_eff->Chi2();
       double ndf_eff = r_eff->Ndf();
       std::cout << "Qualite du fit Efficacite (Chi2/ndf) = " 
                 << chi2_eff << " / " << ndf_eff << " = " << chi2_eff/ndf_eff << std::endl;
   }

   c1->SaveAs("Efficacite_Eta_nominal_fit.pdf");


   // === TRACÉ ET FIT DE LA MASSE INVARIANTE ===
   TCanvas *c2 = new TCanvas("c2", "Masse Invariante Z", 800, 600);
   
   // Définition de la fonction Breit-Wigner Relativiste
   TF1 *bw = new TF1("bw", "[0] * ([1]*[1] * [2]*[2]) / ( (x*x - [1]*[1])*(x*x - [1]*[1]) + [1]*[1]*[2]*[2] )", 70.0, 110.0);
   bw->SetParameters(5000, 91.2, 2.5); // Initialisation: Norm, m_Z, Gamma_Z
   bw->SetParNames("Norm", "m_Z", "#Gamma_Z");
   bw->SetLineColor(kRed);

   std::cout << "\n--- Fit de la masse invariante (Breit-Wigner) ---" << std::endl;
   // Lancement de du fit réel : modification des paramètres pour que le fit corresponde parfaitement aux données 
   TFitResultPtr r = h_masse_Z->Fit("bw", "S"); // L'option "S" sauvegarde et renvoi les résultats du fit dans la variable r
   
   h_masse_Z->SetTitle("Masse invariante des deux muons;m_{#mu#mu} [GeV];Nombre d'evenements");
   h_masse_Z->SetMarkerStyle(20);
   h_masse_Z->SetLineColor(kBlack);
   h_masse_Z->Draw("E"); 
   
   c2->SaveAs("Fit_Masse_Z.pdf");

   // Évaluation du fit 
   if (r == 0) {
       double chi2 = r->Chi2();
       double ndf = r->Ndf();
       std::cout << "\nQualite du fit (Chi2/ndf) = " << chi2 << " / " << ndf << " = " << chi2/ndf << std::endl;
   }
}