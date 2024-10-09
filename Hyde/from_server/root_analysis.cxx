#include <iostream>
#include <stdlib.h>

struct newEvent_t {
	UInt_t		id_ev;
	Float_t		energy;
};
float  fill_histo(TH1F* histo, TTree* br);

void read_par_online_ana(const char* file_ch0,TLegend* legend,const char* str_leg,int n_col){

TChain* chain = new TChain("MyNtuple");

for(int i=0;i<25;i++){
    chain->Add(TString::Format("%s_t%d.root",file_ch0,i));
}

TTree *tree0   = (TTree*)chain->GetTree();

float bin_number = 5000;
float max_bin = 5.;

TH1F *h_qlong_ch0 = new TH1F (file_ch0,"Histo Hyde2 :: Ev_sim =1e7 \n R=2.25um & D=2.5um",bin_number,0,max_bin);
float rate_0 = fill_histo(h_qlong_ch0,chain);
float qlong_avg_0 = h_qlong_ch0->GetMean();

// TCanvas* cnv_histo_0 = new TCanvas();
gPad-> SetLogy();
h_qlong_ch0->Draw("same histo");

int binmax = h_qlong_ch0->GetMaximumBin(); 
double peak = h_qlong_ch0->GetXaxis()->GetBinCenter(binmax);
float det_ev = h_qlong_ch0->Integral();
float tot_ev = 10000000;

h_qlong_ch0->SetLineColor(n_col);
h_qlong_ch0->SetLineWidth(2);
h_qlong_ch0->SetStats(0);

h_qlong_ch0->GetXaxis()->SetTitle("Energy [MeV]");
h_qlong_ch0->GetYaxis()->SetTitle("Counts [-]");

legend->AddEntry(h_qlong_ch0,TString::Format("%s Eff %.1f %s",str_leg,100.*(det_ev/tot_ev),"\u0025"),"f");
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
    if(ev>ene_max)
        ene_max=ev;
    if(ev>0.05)
        histo->Fill(ev);
    }
return 0;
}

int root_analysis(){ 
TCanvas* cnv = new TCanvas();

TLegend* legend = new TLegend(0.55,0.7,0.90,0.9);

read_par_online_ana("buco_pieno/histo_neut",legend,"Cavita' piena",2);
read_par_online_ana("buco_semivuoto/histo_neut",legend,"500nm B10",4);

// read_par_online_ana("buco_tutto_pieno/histo_neut",legend,"Cavita' piena",2);
// read_par_online_ana("buco_mezzo_pieno/histo_neut",legend,"500nm B10",4);
legend->Draw("same");
return 0;
}


