void read_G4_tree(const char *name_file, const char *file_out, int flag_save = 0, const char *output_file = "output.dat")
{
    int numBins = 1000;
    double minX = 0, maxX = 1.5;

    TFile *infile = new TFile(name_file,"READ");
    TTree *tree = (TTree*)infile->Get("results");
    tree->Print();

    double var;

    int counter;

    //TBranch *inbranch0 = tree->GetBranch("LaBr3");
    TBranch *inbranch0 = tree->GetBranch("ene");
    inbranch0->SetAddress(&var);

    TH1F *h_total = new TH1F("h_total","Total",numBins,minX,maxX);
    
    // histogram filling
    for (int i=0; i<inbranch0->GetEntries(); i++)
    {
        inbranch0->GetEntry(i);

        if (var > 0.) 
        {
            h_total->Fill(var);
            counter++;
        }

	}

    cout << endl;

    cout << counter << endl;
    
    TCanvas *c0 = new TCanvas("c0"); // draw all spectra
    h_total->Draw();
    h_total->SetLineColor(2);

    if (flag_save == 1)
    {
        ofstream myfile;
        myfile.open(output_file);
        
        for(int k = 1; k <= h_total->GetNbinsX(); k++)
        //for(int k = 1; k <= bins_psd; k++)
        {
            //float center = h1D->GetBinCenter(k);
            //float content = h1D->GetBinContent(k);

            float center = h_total->GetBinCenter(k);
            float content = h_total->GetBinContent(k);

            myfile << center << "\t" << content << endl;
        }
        
        myfile.close();
    }
    c0->SaveAs(file_out);
}

void start(){
    read_G4_tree("build/histo_t0.root","fig_t0.root");
    read_G4_tree("build/histo_t1.root","fig_t1.root");
}