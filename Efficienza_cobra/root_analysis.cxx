#include <iostream>
#include <stdlib.h>
#include <cmath>

struct newEvent_t {
	UInt_t		id_ev;
	Float_t		energy;
};
float  fill_histo(TH1F* histo, TTree* br);

void read_par_online_ana(const char* file_ch0="histo.root",float energy=0,float ev_sim=0){

TChain* chain = new TChain("MyNtuple");

for(int i=0;i<8;i++){
    chain->Add(TString::Format("%s_t%d.root",file_ch0,i));
}

TTree *tree0   = (TTree*)chain->GetTree();

float bin_number = 320*3;
float max_bin = 320.;

TH1F *h_qlong_ch0 = new TH1F (file_ch0,file_ch0,bin_number,0,max_bin);
float rate_0 = fill_histo(h_qlong_ch0,chain);
float qlong_avg_0 = h_qlong_ch0->GetMean();

int binmax = h_qlong_ch0->FindBin(energy); 

float det_ev = h_qlong_ch0->Integral(binmax-5,binmax+5);

ofstream myfile;
myfile.open ("efficienza",ios::app);
myfile <<energy<<","<<det_ev<<","<<ev_sim<<endl;
myfile.close();

TCanvas* cnv = new TCanvas();
h_qlong_ch0->SetTitle(TString::Format("Ene:%.0f keV -- Ev_sim:%.0f -- Ev_pk:%.0f",energy,ev_sim,det_ev));
h_qlong_ch0->GetXaxis()->SetTitle("Energy [keV]");
h_qlong_ch0->GetYaxis()->SetTitle("Counts [-]");
h_qlong_ch0->Draw();
cnv->SaveAs(TString::Format("histo_ene_%.0fkeV.png",energy));

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

int root_analysis(float energy,float ev_sim){ 

std::ifstream infile("efficienza");
std::string temp;
std::getline(infile, temp);
bool insert_head_text = temp.empty();
infile.close();
ofstream myfile;
myfile.open ("efficienza",ios::app);
if (insert_head_text){
    myfile<<"energy[keV],ev_rivelati,ev_simulati"<<endl;
}
myfile.close();
read_par_online_ana("histo_gamma",energy,ev_sim);
return 0;
}

void start()

