#include <iostream>
#include <stdlib.h>

struct newEvent_t {
	UInt_t		id_ev;
	Float_t		energy;
};
float  fill_histo(TH1F* histo, TTree* br);

void read_par_online_ana(const char* file_ch0="histo.root"){

TChain* chain = new TChain("MyNtuple");

for(int i=0;i<25;i++){
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

int binmax = h_qlong_ch0->GetMaximumBin(); 
double peak = h_qlong_ch0->GetXaxis()->GetBinCenter(binmax);
float det_ev = h_qlong_ch0->Integral();
float tot_ev = 100000;


double distance_from_hole, radius_hole_B, radius_hole_L, pixel_size;

std::ifstream in_myfile;
in_myfile.open("hole_dimension");
in_myfile>>distance_from_hole;
in_myfile>>radius_hole_B;
in_myfile>>radius_hole_L;
in_myfile>>pixel_size;
in_myfile.close();

ofstream myfile;
myfile.open ("efficenza",ios::app);
myfile <<distance_from_hole<<";"<<radius_hole_B<<";"<<radius_hole_L<<";"<<pixel_size<<";"<<det_ev<<";"<<tot_ev<<endl;
myfile.close();

std::cout<<det_ev<<endl;

h_qlong_ch0->SetTitle(TString::Format("hisot_%.1f_%.1f_%.1f",distance_from_hole,radius_hole,pixel_size));
cnv_histo_0->SaveAs(TString::Format("histo_%.1f_%.1f_%.1f.png",distance_from_hole,radius_hole,pixel_size));
h_qlong_ch0->SaveAs(TString::Format("histo_%.1f_%.1f_%.1f.root",distance_from_hole,radius_hole,pixel_size));

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

std::ifstream infile("efficenza");

// Temporary buffer
std::string temp;

// Get the input from the input file until EOF
std::getline(infile, temp);

bool insert_head_text = temp.empty();
infile.close();


ofstream myfile;
myfile.open ("efficenza",ios::app);
if (insert_head_text){
    myfile<<"distance_from_hole;radius_hole_b;radius_hole_Li;pixel_size;det_ev;tot_ev"<<endl;
}
myfile.close();
read_par_online_ana("histo_neut");
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


