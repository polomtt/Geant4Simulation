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

int numero_rivelatori = chain->GetNbranches()/2;

TCanvas* cnv_histo_0[numero_rivelatori];

// int nBranches = tree0->GetEntries();  // Ottieni il numero di branch
// std::cout << "Numero di branch nel TTree: " << nBranches << std::endl;

float bin_number = 500;
float max_bin = 5.;
for(int i=0;i<numero_rivelatori;i++){
    TH1F *h_qlong_ch1 = new TH1F (TString::Format("Eabs_alfa_%d",i),TString::Format("Eabs_alfa_%d",i),bin_number,0,max_bin);
    fill_histo(h_qlong_ch1,chain,TString::Format("Eabs_alfa_%d",i));

    // TH1F *h_qlong_ch2 = new TH1F (file_ch0,file_ch0,bin_number,0,max_bin);
    // fill_histo(h_qlong_ch2,chain,"Eabs_cry_2");
    // h_qlong_ch2->SetLineColor(2);

    // int N_alfa=0;
    // int N_litio=0;
    // int N_alfa_litio=0;

    // Eabs_ions_4 Eabs_alfa_4
    // print_eventi_simultanei(chain,"Eabs_cry_1","Eabs_cry_2",N_alfa,N_litio,N_alfa_litio);

    cnv_histo_0[i]  = new TCanvas(TString::Format("Eabs_alfa_%d",i),TString::Format("Eabs_alfa_%d",i));
    // gPad-> SetLogy();
    h_qlong_ch1->Draw("same histo");
}
// h_qlong_ch2->Draw("same histo");

// float spessore_mat_att;
// float larghezza_alluminio;
// float spessore_alluminio; 

// std::ifstream in_myfile;
// in_myfile.open("dimensioni_sensore");
// in_myfile>>spessore_mat_att;
// in_myfile>>larghezza_alluminio;
// in_myfile>>spessore_alluminio;
// in_myfile.close();

// ofstream myfile;
// myfile.open ("efficenza",ios::app);
// myfile <<spessore_mat_att<<";"<<larghezza_alluminio<<";"<<spessore_alluminio<<";"<<N_alfa<<";"<<N_litio<<";"<<N_alfa_litio<<endl;
// myfile.close();



// cout<<"_*_*_*_*_*_*_*_*_*_*_*_*_*_*_"<<endl;                   
// cout<<"Alfa: "<<N_alfa<<endl;
// cout<<"Li7: "<<N_litio<<endl;
// cout<<"Coincidenza: "<<N_alfa_litio<<endl;
// cout<<"_*_*_*_*_*_*_*_*_*_*_*_*_*_*_"<<endl;

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
    if(ev>0.1)
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
    myfile<<"spessore_mat_att;larghezza_alluminio;spessore_alluminio;N_alfa;N_litio;N_alfa_litio"<<endl;
}
myfile.close();

read_par_online_ana("build/histo_neut");

return 0;
}
