 #include <iostream>
#include <stdlib.h>

struct newEvent_t {
	UInt_t		id_ev;
	Float_t		energy;
};
float  fill_histo(TH1F* histo, TTree* br, const char* detector_name);
void print_eventi_simultanei(TTree* br, const char* detector_name1, const char* detector_name2);

void read_par_online_ana(const char* file_ch0="histo.root"){

TChain* chain = new TChain("MyNtuple");

for(int i=0;i<8;i++){
    chain->Add(TString::Format("%s_t%d.root",file_ch0,i));
}

TTree *tree0   = (TTree*)chain->GetTree();

float bin_number = 500;
float max_bin = 5.;

TH1F *h_qlong_ch1 = new TH1F (file_ch0,file_ch0,bin_number,0,max_bin);
float rate_1 = fill_histo(h_qlong_ch1,chain,"Eabs_cry_1");
float qlong_avg_0 = h_qlong_ch1->GetMean();

TH1F *h_qlong_ch2 = new TH1F (file_ch0,file_ch0,bin_number,0,max_bin);
float rate_2 = fill_histo(h_qlong_ch2,chain,"Eabs_cry_2");
float qlong_avg_2 = h_qlong_ch2->GetMean();
h_qlong_ch2->SetLineColor(2);

print_eventi_simultanei(chain,"Eabs_cry_1","Eabs_cry_2");

TCanvas* cnv_histo_0 = new TCanvas();
// gPad-> SetLogy();
h_qlong_ch1->Draw("same histo");
h_qlong_ch2->Draw("same histo");

float det_ev_1 = h_qlong_ch1->Integral();
float det_ev_2 = h_qlong_ch2->Integral();
float tot_ev = 100;
double distance_from_hole, radius_hole, pixel_size;

std::ifstream in_myfile;
in_myfile.open("hole_dimension");
in_myfile>>distance_from_hole;
// in_myfile>>radius_hole;
in_myfile>>pixel_size;
in_myfile.close();

ofstream myfile;
myfile.open ("efficenza",ios::app);
myfile <<distance_from_hole<<";"<<radius_hole<<";"<<pixel_size<<";"<<det_ev_1<<";"<<tot_ev<<endl;
myfile.close();
return;
}

void print_eventi_simultanei(TTree* br, const char* detector_name1, const char* detector_name2){
    double ev1;
    double ev2;
    br->SetBranchAddress(detector_name1,&ev1);
    br->SetBranchAddress(detector_name2,&ev2);
    int num_ev=br->GetEntries();
    
   
    for(int i=0;i<num_ev;i++){
    br->GetEntry(i);
    cout<<i<<" "<<ev1<<" "<<ev2<<endl;
    }
    

}

float  fill_histo(TH1F* histo, TTree* br, const char* detector_name){
    
    double ev;
    br->SetBranchAddress(detector_name,&ev);
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
    myfile<<"spess_mat_att;radius_hole;pixel_size;det_ev;tot_ev"<<endl;
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


