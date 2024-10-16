#include <iostream>
#include <stdlib.h>

void fill_histo(TH1F* histo, TTree* br, const char* detector_name);
void print_eventi_simultanei(TTree* br, const char* detector_name1, const char* detector_name2, int & N_alfa, int & N_litio,int & N_alfa_litio);

//                     _ 
//  _ __ ___  __ _  __| |
// | '__/ _ \/ _` |/ _` |
// | | |  __/ (_| | (_| |
// |_|  \___|\__,_|\__,_|
                      
void read_par_online_ana(const char* file_ch0="histo.root"){

TChain* chain = new TChain("MyNtuple");

for(int i=0;i<8;i++){
    chain->Add(TString::Format("%s_t%d.root",file_ch0,i));
}

TTree *tree0   = (TTree*)chain->GetTree();

int numero_rivelatori = chain->GetNbranches();

TCanvas* cnv_histo_0[numero_rivelatori];
TCanvas* cnv_histo_1[numero_rivelatori];

float ev_alfa[numero_rivelatori];
float ev_litio[numero_rivelatori];

int nBranches = chain->GetNbranches();  // Ottieni il numero di branch
std::cout << "Numero di branch nel TTree: " << nBranches << std::endl;

float bin_number = 500;
float max_bin = 5.;
for(int i=0;i<numero_rivelatori/2;i++){
    TH1F *h_qlong_alfa = new TH1F (TString::Format("Eabs_alfa_%d",i),TString::Format("Eabs_alfa_%d",i),bin_number,0,max_bin);
    fill_histo(h_qlong_alfa,chain,TString::Format("Eabs_alfa_%d",i));
    cnv_histo_0[i]  = new TCanvas(TString::Format("Eabs_det_%d",i),TString::Format("Eabs_det_%d",i));
    h_qlong_alfa->Draw();
    ev_alfa[i]=h_qlong_alfa->Integral();
}

for(int i=0;i<numero_rivelatori/2.;i++){
    // TH1F *h_qlong_alfa = new TH1F (TString::Format("Eabs_alfa_%d",i),TString::Format("Eabs_alfa_%d",i),bin_number,0,max_bin);
    // fill_histo(h_qlong_alfa,chain,TString::Format("Eabs_alfa_%d",i));

    TH1F *h_qlong_ion = new TH1F (TString::Format("Eabs_ions_%d",i),TString::Format("Eabs_ions_%d",i),bin_number,0,max_bin);
    fill_histo(h_qlong_ion,chain,TString::Format("Eabs_ions_%d",i));

    cnv_histo_0[i]  = new TCanvas(TString::Format("Eabs_det2_%d",i),TString::Format("Eabs_det2_%d",i));
    // gPad-> SetLogy();
    h_qlong_ion->SetLineColor(kRed);
    h_qlong_ion->Draw("same histo");
    ev_litio[i]=h_qlong_ion->Integral();
}

ofstream myfile;
myfile.open ("efficenza",ios::app);
for(int j=0;j<numero_rivelatori/2;j++){
    cout<<j<<"\t"<<ev_alfa[j]<<"\t"<<ev_litio[j]<<endl;
    myfile<<j<<";"<<ev_alfa[j]<<";"<<ev_litio[j]<<endl;
}

myfile.close();

return;
}

//             _       _   
//  _ __  _ __(_)_ __ | |_ 
// | '_ \| '__| | '_ \| __|
// | |_) | |  | | | | | |_ 
// | .__/|_|  |_|_| |_|\__|
// |_|                     

void print_eventi_simultanei(TTree* br, const char* detector_name1, const char* detector_name2,int & N_alfa, int & N_litio,int & N_alfa_litio){
    double ev1;
    double ev2;
    br->SetBranchAddress(detector_name1,&ev1);
    br->SetBranchAddress(detector_name2,&ev2);
    int num_ev=br->GetEntries();
    
    cout<<"Num ev: "<<num_ev<<endl;

    for(int i=0;i<num_ev;i++){
        br->GetEntry(i);
        if(ev1>0.1)
            N_alfa++;
        if(ev2>0)
            N_litio++;
        if(ev1>0 && ev2>0)
            N_alfa_litio++;
    }
    
}

/*   __ _ _ _     _     _     _        
//  / _(_) | |   | |__ (_)___| |_ ___  
// | |_| | | |   | '_ \| / __| __/ _ \ 
// |  _| | | |   | | | | \__ \ || (_) |
// |_| |_|_|_|___|_| |_|_|___/\__\___/ 
//          |_____|                    
*/

void  fill_histo(TH1F* histo, TTree* br, const char* detector_name){
    
    double ev;
    br->SetBranchAddress(detector_name,&ev);
    int num_ev=br->GetEntries();

    for(int i=0;i<num_ev;i++){
    br->GetEntry(i);
    if(ev>0.001)
        histo->Fill(ev);
    }
    return;
}

/*                  _       
//  _ __ ___   __ _(_)_ __  
// | '_ ` _ \ / _` | | '_ \ 
// | | | | | | (_| | | | | |
// |_| |_| |_|\__,_|_|_| |_|
*/                  
int root_analysis(){ 

std::ifstream infile("efficenza");
std::string temp;
std::getline(infile, temp);
bool insert_head_text = temp.empty();
infile.close();
ofstream myfile;
myfile.open ("efficenza",ios::app);
if (insert_head_text){
    myfile<<"id;alfa;litio"<<endl;
}
myfile.close();

read_par_online_ana("histo_neut");

return 0;
}
