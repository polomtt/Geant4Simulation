#include <iostream>
#include <stdlib.h>

struct newEvent_t {
	UInt_t		id_ev;
	Float_t		energy;
};
float  fill_histo(TH1F* histo, TTree* br);

void read_par_online_ana(const char* file_ch0="histo.root",const char* file_out="build/efficienza"){

TChain* chain = new TChain("MyNtuple");

for(int i=0;i<15;i++){
    chain->Add(TString::Format("%s_t%d.root",file_ch0,i));
}

TTree *tree0   = (TTree*)chain->GetTree();

float bin_number = 500;
float max_bin = 5.;

TH1F *h_qlong_ch0 = new TH1F (file_ch0,file_ch0,bin_number,0,max_bin);
float rate_0 = fill_histo(h_qlong_ch0,chain);
float qlong_avg_0 = h_qlong_ch0->GetMean();

TCanvas* cnv_histo_0 = new TCanvas();
gPad-> SetLogy();
h_qlong_ch0->Draw("same histo");

// int binmax = h_qlong_ch0->GetMaximumBin(); 
// double peak = h_qlong_ch0->GetXaxis()->GetBinCenter(binmax);
float det_ev = h_qlong_ch0->Integral();
float tot_ev = 100000;


double spess_b10, spess_li6;

std::ifstream in_myfile;
in_myfile.open("dimension_act_mat");
in_myfile>>spess_b10;
in_myfile>>spess_li6;
in_myfile.close();

ofstream myfile;
myfile.open (file_out,ios::app);
myfile <<spess_b10<<";"<<spess_li6<<";"<<det_ev<<";"<<tot_ev<<endl;
myfile.close();

h_qlong_ch0->SetTitle(TString::Format("hist_b10_%.1fum_li6_%.1fum",spess_b10,spess_li6));
cnv_histo_0->SaveAs(TString::Format("hist_b10_%.1fum_li6_%.1fum.png",spess_b10,spess_li6));

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

const char* file_out="efficienza";

std::ifstream infile(file_out);

// Temporary buffer
std::string temp;

// Get the input from the input file until EOF
std::getline(infile, temp);

bool insert_head_text = temp.empty();
infile.close();

ofstream myfile;
myfile.open (file_out,ios::app);
if (insert_head_text){
    myfile<<"spess_b10[um];spess_li6[um];det_ev;tot_ev"<<endl;
}
myfile.close();
read_par_online_ana("histo_neut",file_out);
return 0;
}
