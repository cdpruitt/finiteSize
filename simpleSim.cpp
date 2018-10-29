#include "simpleSim.h"
#include "TH1.h"
#include "TFile.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TCanvas.h"

#include "CrossSection.h"
#include "RandomGenerator.h"
#include "Detector.h"
#include "mathFunctions.h"

#include <cmath>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    int iterations = stoi(argv[1]);

    TFile* outputFile = new TFile("output.root", "RECREATE");
    TH1D* sampledDistribution = new TH1D("sampledDistribution", "sampledDistribution", NUMBER_ANGLE_BINS, LOW_CS_ANGLE, HIGH_CS_ANGLE);

    CrossSection crossSection("inputCS.txt");

    // initialize RNGs
    RandomGenerator radiusGen(0, SAMPLE_RADIUS);
    RandomGenerator thetaGen(0, 2*M_PI);
    RandomGenerator lengthGen(-SAMPLE_LENGTH/2, SAMPLE_LENGTH/2);

    RandomGenerator CSThetaGen(MAX_Z/DETECTOR_DISTANCE, -MAX_Z/DETECTOR_DISTANCE);
    RandomGenerator CSPhiGen(toRadians(LOW_CS_ANGLE), toRadians(HIGH_CS_ANGLE));

    RandomGenerator CSSampleGen(0, crossSection.max);

    // initialize detectors
    vector<Detector> detectors;
    for(int i=10; i<180; i+=3)
    {
        detectors.push_back(Detector(i, DETECTOR_RADIUS, DETECTOR_DISTANCE));
    }

    TH3I scatteringDirectionHisto("scatteringDirection", "scatteringDirection",
            100, -1, 1,
            100, -1, 1,
            100, -1, 1);

    TH2I scatteringDirection2D("scatteringDirection2D", "scatteringDirection2D",
            100, 0, M_PI,
            100, 0, 2*M_PI);

    bool hitDet;

    for(int i=0; i<iterations; i++)
    {
        hitDet = false;

        // randomly choose a point in the sample volume
        double sampleRadius = radiusGen.getValue();
        double sampleTheta = thetaGen.getValue();

        Coordinates scatteringOrigin(
                sampleRadius*cos(toRadians(sampleTheta)),
                sampleRadius*sin(toRadians(sampleTheta)),
                lengthGen.getValue()
                );

        // sample the cross section distribution to create a trajectory
        double scatteringPhi = -1;

        while(true)
        {
            double tentativePhi = CSPhiGen.getValue();
            double CSSample = CSSampleGen.getValue();

            if(crossSection.getValue(toDegrees(tentativePhi))>CSSample)
            {
                scatteringPhi = tentativePhi;
                break;
            }
        }

        double scatteringTheta = acos(CSThetaGen.getValue());

        scatteringDirection2D.Fill(scatteringTheta, scatteringPhi);

        Coordinates scatteringDirection(
                sin(scatteringTheta)*cos(scatteringPhi),
                sin(scatteringTheta)*sin(scatteringPhi),
                cos(scatteringTheta)
                );

        scatteringDirectionHisto.Fill(
                scatteringDirection.x,
                scatteringDirection.y,
                scatteringDirection.z);

        // calculate point of intersection of scattering trajectory and detector plane
        for(auto& detector : detectors)
        {
            Coordinates lineToDetector = detector.origin - scatteringOrigin;
            double scalar = lineToDetector.dot(detector.origin)/scatteringDirection.dot(detector.origin);

            // don't let the particle scatter "backwards"
            if(scalar<0)
            {
                continue;
            }

            Coordinates intersectionPoint =
                scatteringDirection.scale(scalar) + scatteringOrigin;

            // test if hit detector
            if(intersectionPoint.distance(detector.origin) < detector.radius)
            {
                detector.distanceHisto.Fill(intersectionPoint.distance(detector.origin));
                detector.hitMap.Fill(
                        intersectionPoint.x-detector.origin.x,
                        intersectionPoint.y-detector.origin.y,
                        intersectionPoint.z-detector.origin.z
                        );
                detector.XYProj.Fill(
                        intersectionPoint.x-detector.origin.x,
                        intersectionPoint.y-detector.origin.y
                        );
                detector.ZYProj.Fill(
                        intersectionPoint.z-detector.origin.z,
                        intersectionPoint.y-detector.origin.y
                        );

                detector.counts++;

                hitDet = true;
            }
        }

        sampledDistribution->Fill(toDegrees(scatteringPhi));
    }

    // normalize output cross section
    double binSize = (HIGH_CS_ANGLE-LOW_CS_ANGLE)/NUMBER_ANGLE_BINS;
    sampledDistribution->Scale(crossSection.getIntegral()/((double)iterations*binSize));

    TGraphErrors* inputGraph = new TGraphErrors(
            crossSection.data.getNumberOfPoints(),
            &crossSection.data.getXValues()[0],
            &crossSection.data.getYValues()[0]
            );
    inputGraph->SetNameTitle("inputCS", "inputCS");
    inputGraph->Write();

    // output as graph
    vector<double> xValues;
    vector<double> yValues;

    for(int i=1; i<=NUMBER_ANGLE_BINS; i++)
    {
        xValues.push_back(sampledDistribution->GetBinCenter(i));
        yValues.push_back(sampledDistribution->GetBinContent(i));
    }

    TGraphErrors* outputGraph = new TGraphErrors(xValues.size(), &xValues[0], &yValues[0]);
    outputGraph->SetNameTitle("sampledDistribution", "sampledDistribution");
    outputGraph->Write();

    // write out detector counts
    vector<double> detectorAngles;
    vector<double> detectorCounts;

    for(auto& detector : detectors)
    {
        detectorAngles.push_back(detector.angle);

        double normalizationFactor = (crossSection.getIntegral()/(HIGH_CS_ANGLE-LOW_CS_ANGLE))/((double)iterations*detector.solidAngleFraction);
        detectorCounts.push_back(detector.counts*normalizationFactor);

        detector.distanceHisto.Write();
        detector.hitMap.Write();
        detector.XYProj.Write();
        detector.ZYProj.Write();
    }

    TGraphErrors* detectorsGraph = new TGraphErrors(detectorAngles.size(), &detectorAngles[0], &detectorCounts[0]);
    detectorsGraph->SetNameTitle("detectors","detectors");
    detectorsGraph->Write();

    scatteringDirection2D.Write();
    scatteringDirectionHisto.Write();

    outputFile->Close();

    return 0;
}
