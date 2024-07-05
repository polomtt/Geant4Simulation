#include <iostream>
#include <stdlib.h>

struct newEvent_t {
	UInt_t		id_ev;
	Float_t		energy;
};
float  fill_histo(TH1F* histo, TTree* br,const char* br_name);

void read_par_online_ana(const char* file_ch0="histo.root"){

TChain* chain = new TChain("MyNtuple");

for(int i=0;i<8;i++){
    chain->Add(TString::Format("%s_t%d.root",file_ch0,i));
}

TTree *tree0   = (TTree*)chain->GetTree();

float bin_number = 1000;
float max_bin = 6.;

TH1F *h_qlong_ch0 = new TH1F (file_ch0,file_ch0,bin_number,0,max_bin);
TH1F *h_qlong_ch1 = new TH1F ("ch1","ch1",bin_number,0,max_bin);
TH1F *h_qlong_tot = new TH1F ("Alfa Am241 energy deposition","Alfa Am241 energy deposition",bin_number,0,max_bin);

h_qlong_tot->GetXaxis()->SetTitle("Energy [MeV]");
h_qlong_tot->GetYaxis()->SetTitle("Counts norm [-]");
h_qlong_tot->GetXaxis()->SetLabelSize(0.05);
h_qlong_tot->GetXaxis()->SetTitleSize(0.05);
h_qlong_tot->GetYaxis()->SetLabelSize(0.05);
h_qlong_tot->GetYaxis()->SetTitleSize(0.05);

float rate_0 = fill_histo(h_qlong_ch0,chain,"Eabs_cry_1");
float rate_1 = fill_histo(h_qlong_ch1,chain,"Eabs_cry_2");
float rate_2 = fill_histo(h_qlong_tot,chain,"Eabs_tot");
float qlong_avg_0 = h_qlong_ch0->GetMean();
float qlong_avg_1 = h_qlong_ch1->GetMean();
float qlong_avg_tot = h_qlong_tot->GetMean();

TCanvas* cnv_histo_0 = new TCanvas();
cnv_histo_0->SetLeftMargin(0.12);
gStyle->SetOptStat(0);
float tot_ev = h_qlong_tot->GetMaximum();
h_qlong_tot->Scale(1.0/tot_ev);
h_qlong_ch1->Scale(1.0/tot_ev);
h_qlong_ch0->Scale(1.0/tot_ev);

h_qlong_tot->Draw("same histo");
h_qlong_ch1->Draw("same histo");
h_qlong_ch0->Draw("same histo");

h_qlong_tot->SetLineColor(8);
h_qlong_ch1->SetLineColor(2);
h_qlong_ch0->SetLineColor(4);

h_qlong_tot->SetLineWidth(2);
h_qlong_ch1->SetLineWidth(2);
h_qlong_ch0->SetLineWidth(2);

auto legend = new TLegend(0.12,0.7,0.70,0.9);
legend->AddEntry(h_qlong_ch0,TString::Format("Silicon, Thick=10um <E>=%.2fMeV",qlong_avg_0),"l");
legend->AddEntry(h_qlong_ch1,TString::Format("Kapton, Thick=25um <E>=%.2fMeV",qlong_avg_1),"l");
legend->AddEntry(h_qlong_tot,TString::Format("E_tot <E>=%.2fMeV",qlong_avg_tot),"l");
legend->Draw();

return;
}

float  fill_histo(TH1F* histo, TTree* br,const char* br_name){
    
    double ev;
    br->SetBranchAddress(br_name,&ev);
    int num_ev=br->GetEntries();
    
    float ene_max=0;
    float ene_min=0.04;
    
    for(int i=0;i<num_ev;i++){
    br->GetEntry(i);
    if(ev>ene_max)
        ene_max=ev;
    if(ev>0.05)
        histo->Fill(ev);
    }
    
return 0;

}

int root_analysis(){ 

read_par_online_ana("histo");
return 0;
}


// int nPeaks=1;
// TSpectrum *s = new TSpectrum(nPeaks);      //num numero di picchi da cercare
// double_t *xPeaks;                      //vettore picchi
// nPeaks = s->Search(h_qlong_ch0);
// xPeaks = s->GetPositionX();
// sort(xPeaks, xPeaks + nPeaks);  //ordino il picchi con energia crescente
// 
// for(int i=0;i<nPeaks;i++){
//     float peak=(xPeaks[i]*bin_number)/max_bin+0.5;
//     
//     cout << "Peak #" << i << " @ channel " << peak << " - - events number: " << events_peak << endl;	
// }


