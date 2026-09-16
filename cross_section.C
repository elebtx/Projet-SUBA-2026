#include <TCanvas.h>
#include <TGraphErrors.h>

void cross_section() {

    const int n = 4;

    double energy[n] = {7, 8, 13, 14};
    double sigma[n] = {368.6, 415.2, 638.2, 681.1};

    double err_energy[n] = {0, 0, 0, 0};
    double err_sigma[n] = {0.24, 0.27, 0.44, 0.44};

    TCanvas *c1 = new TCanvas("c1", "Section efficace", 800, 600);

    TGraphErrors *gr = new TGraphErrors(n, energy, sigma, err_energy, err_sigma);

    gr->SetTitle("Section efficace en fonction de l'energie de collision;#sqrt{s} [TeV];#sigma [pb]");
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(1.2);

    gr->Draw("AP");

    c1->SaveAs("cross_section.png");
}
