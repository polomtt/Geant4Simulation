void print_graph(const char* filename, const char* str_leg,TLegend* legend,int n_col){
 std::ifstream infile(filename);
 
 float bin =0;
 float counts =0;
 
 TH1F* histo = new TH1F("histo","",150,0,2.5);

 while(infile>>bin){
 infile>>counts;

    int binIndex = histo->FindBin(bin);
    histo->SetBinContent(binIndex,round(counts));

 cout<<bin<<" "<<counts<<endl;
 }




histo->SetLineColor(n_col);
histo->SetLineWidth(2);
histo->SetStats(0);

histo->GetXaxis()->SetTitle("Energy [MeV]");
histo->GetYaxis()->SetTitle("Counts [-]");

histo->GetXaxis()->SetLabelSize(0.04);
histo->GetYaxis()->SetLabelSize(0.04);

histo->GetXaxis()->CenterTitle();
histo->GetYaxis()->CenterTitle();

histo->GetXaxis()->SetTitleSize(0.05);
histo->GetYaxis()->SetTitleSize(0.05);

histo->GetXaxis()->SetRangeUser(0,4);

// legend->AddEntry(histo,TString::Format("#splitline{%s}{Eff %.1f %s}",str_leg,100.*(det_ev/tot_ev),"\u0025"),"f");
legend->AddEntry(histo,TString::Format("%s",str_leg),"l");
legend->SetTextSize(0.04);  

gPad->SetLeftMargin(0.13);    // Imposta la larghezza del margine sinistro
gPad->SetRightMargin(0.05);   // Imposta la larghezza del margine destro
gPad->SetBottomMargin(0.15);  // Imposta la larghezza del margine inferiore
gPad-> SetLogy();
 histo->Draw("same");
}

void start(){

TCanvas* cnv = new TCanvas();

TLegend* legend = new TLegend(0.75,0.7,0.94,0.89);
print_graph("doc","Sample 1",legend,38);
print_graph("doc_2","Sample 2",legend,8);

legend->SetLineColor(kWhite);
legend->Draw("same");
cnv->SaveAs("fig_mis.png");
cnv->SaveAs("fig_mis.pdf");
}
