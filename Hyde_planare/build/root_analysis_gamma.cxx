#include <iostream>
#include <stdlib.h>

struct newEvent_t {
	UInt_t		id_ev;
	Float_t		energy;
};
float  fill_histo(TH1F* histo, TTree* br);

void read_par_online_ana(const char* file_ch="histo.root"){

const char* file_ch0 = "histo_gamma";
TChain* chain = new TChain("MyNtuple");

for(int i=0;i<8;i++){
    chain->Add(TString::Format("%s_t%d.root",file_ch0,i));
}

TTree *tree0   = (TTree*)chain->GetTree();

float bin_number = 10000.0;
float max_bin = 3.0;

TH1F *h_qlong_ch0 = new TH1F (file_ch0,file_ch0,bin_number,0,max_bin);
float rate_0 = fill_histo(h_qlong_ch0,chain);
float qlong_avg_0 = h_qlong_ch0->GetMean();

TCanvas* cnv_histo_0 = new TCanvas();
gPad-> SetLogy();
h_qlong_ch0->Draw("same histo");


int binmax = h_qlong_ch0->GetMaximumBin(); 
double peak = h_qlong_ch0->GetXaxis()->GetBinCenter(binmax);
float events_peak = h_qlong_ch0->Integral(0,10000);

h_qlong_ch0->SetTitle(TString::Format("Peak pos:%f --  Integral:%.0f",binmax*max_bin/(bin_number),events_peak));
cnv_histo_0->SaveAs("histo.png");

ofstream myfile;
myfile.open ("efficenza_gamma",ios::app);
myfile <<events_peak<<endl;
myfile.close();

return;
}

float  fill_histo(TH1F* histo, TTree* br){
    
    double ev;
    br->SetBranchAddress("Eabs",&ev);
    int num_ev=br->GetEntries();
    
    float ene_max=0;
    
    for(int i=0;i<num_ev;i++){
    br->GetEntry(i);
    if(ev>ene_max)
        ene_max=ev;
    histo->Fill(ev);
    }
    
return 0;

}

int root_analysis_gamma(){
    read_par_online_ana("histo_t0.root");
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


