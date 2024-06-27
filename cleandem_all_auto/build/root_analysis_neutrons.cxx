#include <iostream>
#include <stdlib.h>

struct newEvent_t {
	UInt_t		id_ev;
	Float_t		energy;
};
float  fill_histo(TH1F* histo, TTree* br);

void read_par_online_ana(const char* file_ch0="output.root",int energy=0){

TFile *infile = new TFile(file_ch0);

TTree *tree0   = (TTree*)infile->Get("results");

float bin_number = 250.0;
float max_bin = 5;

TH1F *h_qlong_ch0 = new TH1F (file_ch0,file_ch0,bin_number,0,max_bin);
float rate_0 = fill_histo(h_qlong_ch0,tree0);
float qlong_avg_0 = h_qlong_ch0->GetMean();

TCanvas* cnv_histo_0 = new TCanvas();
// gPad-> SetLogy();
h_qlong_ch0->Draw("same histo");
// auto legend_histo_0 = new TLegend(0.3,0.7,0.9,0.9);
// legend_histo_0 ->AddEntry(h_qlong_ch0,TString::Format("Qlong_{avg}:%f \n Entries:%.0f",qlong_avg_0,h_qlong_ch0->GetEntries()),"lp");


int binmax = h_qlong_ch0->GetMaximumBin(); 
double peak = h_qlong_ch0->GetXaxis()->GetBinCenter(binmax);
float events_peak = h_qlong_ch0->Integral(0,bin_number);

float end_point=-1;

for(int j=bin_number;j>0;j--){
    double help = h_qlong_ch0->GetBinContent(j);
    if (help>0){
        end_point=j*max_bin/(bin_number);
        break;
    }
}

h_qlong_ch0->SetTitle(TString::Format("Energy:%d --  Integral:%.0f",energy,events_peak));
cnv_histo_0->SaveAs("histo.png");

ofstream myfile;
myfile.open ("efficenza",ios::app);
myfile <<energy<<";"<<events_peak<<";"<<end_point<<endl;
myfile.close();

return;
}

float  fill_histo(TH1F* histo, TTree* tree){
    
    double ev;
   TBranch *br = tree->GetBranch("stilbene"); 
//     TBranch *br = tree->GetBranch("CeBr3");
    br->SetAddress(&ev);
    int num_ev=br->GetEntries();
    
    float ene_max=0;
    
    for(int i=0;i<num_ev;i++){
    br->GetEntry(i);
    if(ev>ene_max)
        ene_max=ev;
    if(ev>0.1)
        histo->Fill(ev);
    }
    
return 0;

}

int root_analysis_neutrons(){ 
    ofstream myfile;
    myfile.open ("efficenza",ios::out);
    myfile <<"energy;eff;end_point"<<endl;
    myfile.close();
//     for(int e=500;e<9001;e=e+500)
//         read_par_online_ana(TString::Format("histo_for_efficency_neutron/histo_%d_keV.root",e),e);
    
    read_par_online_ana("/home/matteo/INFN/simulazioni/cleandem_all_auto/build/output_U238_neutron_cleandem_foucus_centro_POS_A_CeBr_1000000000.root",100);
    return 0;
}


