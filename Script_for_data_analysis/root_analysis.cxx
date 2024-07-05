#include <iostream>
#include <stdlib.h>

struct newEvent_t {
	UInt_t		id_ev;
	Float_t		energy;
};
float  fill_histo(TH1F* histo, TTree* br);

void read_par_online_ana(const char* file_ch0="histo.root",float max_bin=8,const char* file_to_save_h="h.png"){

TChain* chain = new TChain("MyNtuple");

for(int i=0;i<15;i++){
    chain->Add(TString::Format("%s_t%d.root",file_ch0,i));
}

TTree *tree0   = (TTree*)chain->GetTree();

float bin_number = 1000;

TH1F *h_qlong_ch0 = new TH1F (file_ch0,file_ch0,bin_number,0,max_bin);
float ene_max = fill_histo(h_qlong_ch0,chain);
float qlong_avg_0 = h_qlong_ch0->GetMean();

// gPad-> SetLogy();
h_qlong_ch0->Draw("same histo");

int binmax = h_qlong_ch0->GetMaximumBin(); 
double peak = h_qlong_ch0->GetXaxis()->GetBinCenter(binmax);
float det_ev = h_qlong_ch0->Integral();
float tot_ev = 1e6;

double max_bin_for_plot = h_qlong_ch0->FindLastBinAbove(1);

cout<<max_bin_for_plot<<endl;

h_qlong_ch0->GetXaxis()->SetTitle("Energia depositata [MeV]");
h_qlong_ch0->GetYaxis()->SetTitle("Conteggi [-]");
// h_qlong_ch0->GetXaxis()->SetRange(0,max_bin_for_plot+50);
// h_qlong_ch0->SetLineWidth(2);

h_qlong_ch0->SetTitle(TString::Format("Ev rivelati %.0f -- Ev simulati %.0f -- Eff intrinseca %.2e",det_ev,tot_ev,det_ev/tot_ev));
h_qlong_ch0->SaveAs("histo.root");

ofstream myfile;
myfile.open ("histo_simulato.csv");
myfile << "#time;bin;time_acq\n";
for(int k=0;k<bin_number;k++){
  myfile <<h_qlong_ch0->GetBinCenter(k)<<";"<<h_qlong_ch0->GetBinContent(k)<<";1\n";
}
  myfile.close();


return;
}

float  fill_histo(TH1F* histo, TTree* br){
    
    double ev;
    br->SetBranchAddress("Eabs",&ev);
    int num_ev=br->GetEntries();
    
    //pass from MeV to kev
    // ev = ev*1000;

    float ene_max=0;
    float ene_min=0.04;
    
    for(int i=0;i<num_ev;i++){
    br->GetEntry(i);
    if(ev>ene_max)
        ene_max=ev;
    if(ev>0.005)
        histo->Fill(ev);
    }
    
return ene_max;

}


void root_analysis_alpha(){
// read_par_online_ana("simulation_results/histo_alfa",2.5,"histo_perov_alpha.png");
TCanvas* cnv_histo_0 = new TCanvas();

read_par_online_ana("histo_alfa_1000000ev_planar",6.,"histo_perov_alpha_planar.png");
//read_par_online_ana("histo_alfa_100000ev_iso",6.,"histo_perov_alpha_iso.png");
cnv_histo_0->SaveAs("histo.png");
}




