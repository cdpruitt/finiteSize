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

    string outputFile1XName = "output1X.root";
    string outputFile2XName = "output2X.root";
    string outputFile10XName = "output10X.root";

    TFile* outputFile1X = new TFile(outputFile1XName.c_str(),"READ");
    TFile* outputFile2X = new TFile(outputFile2XName.c_str(),"READ");
    TFile* outputFile10X = new TFile(outputFile10XName.c_str(),"READ");

    string inputGraphName = "inputCS";
    string outputGraphName = "scatteringDistribution";

    string detectorsGraphName = "detectors";

    TGraphErrors* inputGraph = (TGraphErrors*)outputFile1X->Get(inputGraphName.c_str());
    TGraphErrors* outputGraph = (TGraphErrors*)outputFile1X->Get(outputGraphName.c_str());
    TGraphErrors* detectors1XGraph = (TGraphErrors*)outputFile1X->Get(detectorsGraphName.c_str());
    TGraphErrors* detectors2XGraph = (TGraphErrors*)outputFile2X->Get(detectorsGraphName.c_str());
    TGraphErrors* detectors10XGraph = (TGraphErrors*)outputFile10X->Get(detectorsGraphName.c_str());


    //inputGraph->SetLineWidth(2);
    //inputGraph->SetLineColor(kBlue);

    inputGraph->SetMarkerSize(1);
    inputGraph->SetMarkerStyle(20);
    inputGraph->SetMarkerColor(kBlack);

    outputGraph->SetMarkerSize(1);
    outputGraph->SetMarkerStyle(20);
    outputGraph->SetMarkerColor(kBlue);

    detectors1XGraph->SetMarkerSize(1);
    detectors1XGraph->SetMarkerStyle(20);
    detectors1XGraph->SetMarkerColor(kRed-7);

    detectors2XGraph->SetMarkerSize(1);
    detectors2XGraph->SetMarkerStyle(20);
    detectors2XGraph->SetMarkerColor(kRed-3);

    detectors10XGraph->SetMarkerSize(1);
    detectors10XGraph->SetMarkerStyle(20);
    detectors10XGraph->SetMarkerColor(kRed+2);

    TMultiGraph* mg = new TMultiGraph();

    mg->Add(inputGraph, "p");
    mg->Add(outputGraph, "p");
    mg->Add(detectors1XGraph, "p");
    mg->Add(detectors2XGraph, "p");
    mg->Add(detectors10XGraph, "p");

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

    mg->GetYaxis()->SetRangeUser(0,50);

    TLegend *legend = new TLegend(0.7,0.7,0.8,0.8);
    legend->SetTextSize(0.03);
    legend->AddEntry(inputGraph,"Input CS","l");
    legend->AddEntry(outputGraph,"Output CS","p");
    legend->AddEntry(detectors1XGraph,"Detector CS, 1X","p");
    legend->AddEntry(detectors2XGraph,"Detector CS, 2X","p");
    legend->AddEntry(detectors10XGraph,"Detector CS, 10X","p");

    legend->Draw();

    c->Update();
}
