#include <iostream>
#include <stdlib.h>
#include <cmath>

float  fill_histo(TH1F* histo, TTree* br);
void smear_gaussian(TH1F* histo,TH1F* histo_sme);


void read_par_online_ana(const char* file_ch0="histo.root"){

TChain* chain = new TChain("MyNtuple");

for(int i=0;i<15;i++){
    chain->Add(TString::Format("%s_t%d.root",file_ch0,i));
}

TTree *tree0   = (TTree*)chain->GetTree();

float bin_number = 10000;
float max_bin = 100.;

TH1F *h_qlong_ch0 = new TH1F (file_ch0,file_ch0,bin_number,0,max_bin);
TH1F *h_qlong_ch0_smear = new TH1F ("sme","sme",bin_number,0,max_bin);
float rate_0 = fill_histo(h_qlong_ch0,chain);
float qlong_avg_0 = h_qlong_ch0->GetMean();

smear_gaussian(h_qlong_ch0,h_qlong_ch0_smear);

TCanvas* cnv = new TCanvas();
h_qlong_ch0->SetTitle(file_ch0);
h_qlong_ch0->GetXaxis()->SetTitle("Energy [keV]");
h_qlong_ch0->GetYaxis()->SetTitle("Counts [-]");
h_qlong_ch0->Draw();
h_qlong_ch0_smear->SetLineColor(kRed);
h_qlong_ch0_smear->Draw("same");
cnv->SaveAs(TString::Format("%s.png",file_ch0));

return;
}

float  fill_histo(TH1F* histo, TTree* br){
    
    double ev;
    br->SetBranchAddress("Eabs",&ev);
    int num_ev=br->GetEntries();
    
    float ene_max=0;
    float ene_min=0.04;
    
    for(int i=0;i<num_ev;i++){
    br->GetEntry(i);
    histo->Fill(ev*1000.);
    }
    
return 0;

}

void smear_gaussian(TH1F* histo,TH1F* histo_sme){

    TRandom3 randomGen(0);

    for(int i=0;i<(histo->GetNbinsX());i++){
        int count = histo->GetBinContent(i);

        float energy = histo->GetBinCenter(i);
        float fano_factor = 0.115;
        float energy_res_exp = 1./sqrt((energy*1000)/3.6);
        // float sigma = 0.02;
        float sigma = sqrt(energy_res_exp*energy*energy_res_exp*energy+0.200*3.6*0.200*3.6);
        
        for(int j=0;j<count;j++)
        {
            histo_sme->Fill(randomGen.Gaus(energy,sigma));
        }
    }
}

void start(){ 
read_par_online_ana("histo_gamma");
}

