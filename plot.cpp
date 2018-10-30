{
    TStyle * style = (TStyle*)gROOT->FindObject("graphStyle");

    if(!style)      
    {
        style = new TStyle("graphStyle","graphStyle");
    }

    style->SetOptStat(0);
    style->SetOptTitle(0);    
    style->SetPalette(1,0);
    style->SetCanvasColor(10);      
    style->SetCanvasBorderMode(0);    
    style->SetFrameLineWidth(3);
    style->SetFrameFillColor(10);
    style->SetPadColor(10);
    style->SetHistLineWidth(3);
    style->SetHistLineColor(kBlue);
    style->SetFuncWidth(3);
    style->SetFuncColor(kRed);
    style->SetLabelColor(kBlack,"xyz");
    style->SetTitleSize(0.06,"xyz");
    style->SetTitleFillColor(10);
    style->SetTitleTextColor(kBlack);
    style->SetEndErrorSize(0);

    gROOT->SetStyle("graphStyle");
    gROOT->ForceStyle();

    TCanvas* c = new TCanvas("c1","",1200,1200);

    // Pad dimensions and margins
    gPad->SetPad(0.005, 0.995, 0.995, 0.005);
    gPad->SetLeftMargin(0.15);
    gPad->SetRightMargin(0.01);
    gPad->SetTopMargin(0.03);
    gPad->SetBottomMargin(0.15);
    gPad->SetTicky(2);

    string outputFileName = "output.root";

    TFile* outputFile = new TFile(outputFileName.c_str(),"READ");
    
    string inputGraphName = "inputCS";
    string outputGraphName = "scatteringDistribution";
    string detectorsGraphName = "detectors";

    TGraphErrors* inputGraph = (TGraphErrors*)outputFile->Get(inputGraphName.c_str());
    TGraphErrors* outputGraph = (TGraphErrors*)outputFile->Get(outputGraphName.c_str());
    TGraphErrors* detectorsGraph = (TGraphErrors*)outputFile->Get(detectorsGraphName.c_str());

    inputGraph->SetLineWidth(2);
    inputGraph->SetLineColor(kBlue);

    outputGraph->SetMarkerSize(1);
    outputGraph->SetMarkerStyle(20);
    outputGraph->SetMarkerColor(kRed);

    detectorsGraph->SetMarkerSize(1);
    detectorsGraph->SetMarkerStyle(20);
    detectorsGraph->SetMarkerColor(kOrange);

    TMultiGraph* mg = new TMultiGraph();

    mg->Add(inputGraph, "l");
    mg->Add(outputGraph, "p");
    mg->Add(detectorsGraph, "p");

    mg->Draw("a");

    mg->GetXaxis()->SetTitle("Angle (degrees, CM)");
    mg->GetXaxis()->SetTitleSize(0.05);
    mg->GetXaxis()->SetTitleFont(2);
    mg->GetXaxis()->SetTitleOffset(1.4);
    mg->GetXaxis()->CenterTitle();

    mg->GetXaxis()->SetLabelOffset(0.01);
    mg->GetXaxis()->SetLabelSize(0.05);
    mg->GetXaxis()->SetLabelFont(2);

    mg->GetXaxis()->SetNdivisions(10);
    mg->GetXaxis()->SetTickLength(0.03);

    // Y-axis parameters
    mg->GetYaxis()->SetTitle("#frac{d#sigma}{d#Omega} (mb/sr)");
    mg->GetYaxis()->SetTitleSize(0.06);
    mg->GetYaxis()->SetTitleFont(2);
    mg->GetYaxis()->SetTitleOffset(1.1);
    mg->GetYaxis()->CenterTitle();

    mg->GetYaxis()->SetLabelOffset(0.01);
    mg->GetYaxis()->SetLabelSize(0.05);

    mg->GetYaxis()->SetLabelFont(2);
    mg->GetYaxis()->SetNdivisions(10);
    mg->GetYaxis()->SetTickLength(0.02);

    mg->GetYaxis()->SetRangeUser(0,12);

    TLegend *legend = new TLegend(0.7,0.7,0.8,0.8);
    legend->SetTextSize(0.03);
    legend->AddEntry(inputGraph,"Input CS","l");
    legend->AddEntry(outputGraph,"Output CS","p");
    legend->AddEntry(detectorsGraph,"Detector CS","p");

    legend->Draw();

    c->Update();
}
