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
    TH1D* scatteringDistribution = new TH1D("scatteringDistribution", "scatteringDistribution", NUMBER_ANGLE_BINS, LOW_CS_ANGLE, HIGH_CS_ANGLE);

    CrossSection crossSection("inputCS.txt");

    // initialize RNGs
    RandomGenerator radiusGen(0, pow(SAMPLE_RADIUS,2));
    RandomGenerator thetaGen(0, 2*M_PI);
    RandomGenerator lengthGen(-SAMPLE_LENGTH/2, SAMPLE_LENGTH/2);

    RandomGenerator CSThetaGen(MAX_Z/DETECTOR_DISTANCE, -MAX_Z/DETECTOR_DISTANCE);
    RandomGenerator CSPhiGen(0, 2*M_PI);

    RandomGenerator CSSampleGen(0, crossSection.max);

    // initialize detectors
    vector<Detector> detectors;
    double angleStep = (HIGH_CS_ANGLE-LOW_CS_ANGLE)/(NUMBER_ANGLE_BINS-1);
    for(double angle=LOW_CS_ANGLE; angle<=HIGH_CS_ANGLE; angle+=angleStep)
    {
        detectors.push_back(Detector(angle, DETECTOR_RADIUS, DETECTOR_DISTANCE));
    }

    TH3D scatteringDirectionHisto("scatteringDirection", "scatteringDirection",
            100, -1, 1,
            100, -1, 1,
            100, -1, 1);

    TH2D scatteringDirection2D("scatteringDirection2D", "scatteringDirection2D",
            100, 0, M_PI,
            100, 0, 2*M_PI);

    TH3D sampleDistribution("sampleDistribution", "sampleDistribution",
            100, -SAMPLE_RADIUS, SAMPLE_RADIUS,
            100, -SAMPLE_RADIUS, SAMPLE_RADIUS,
            100, -SAMPLE_LENGTH, SAMPLE_LENGTH);

    bool hitDet;

    int totalHits = 0;

    for(int i=0; i<iterations; i++)
    {
        hitDet = false;

        // randomly choose a point in the sample volume
        double sampleRadius = pow(radiusGen.getValue(),0.5);
        double sampleTheta = thetaGen.getValue();

        Coordinates scatteringOrigin(
                sampleRadius*cos(sampleTheta),
                sampleRadius*sin(sampleTheta),
                lengthGen.getValue()
                );

        sampleDistribution.Fill(scatteringOrigin.x, scatteringOrigin.y, scatteringOrigin.z);

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

            // ignore particles scattering from 180-360 degrees
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
                detector.XZProj.Fill(
                        intersectionPoint.x-detector.origin.x,
                        intersectionPoint.z-detector.origin.z
                        );
                detector.YZProj.Fill(
                        intersectionPoint.y-detector.origin.y,
                        intersectionPoint.z-detector.origin.z
                        );

                detector.counts++;

                totalHits++;
                //scatteringDistribution->Fill(detector.angle);
            }
        }

        scatteringDistribution->Fill(toDegrees(scatteringPhi));

        if(i%10000==0)
        {
            cout << "Iteration " << i << "/" << iterations << "\r";
            fflush(stdout);
        }
    }

    // normalize output cross section
    double binSize = (HIGH_CS_ANGLE-LOW_CS_ANGLE)/(2*NUMBER_ANGLE_BINS);
    scatteringDistribution->Scale(crossSection.getIntegral()/((double)iterations*binSize));
    //scatteringDistribution->Scale(crossSection.getIntegral()/((double)totalHits*binSize));

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
        xValues.push_back(scatteringDistribution->GetBinCenter(i));
        yValues.push_back(scatteringDistribution->GetBinContent(i));
    }

    TGraphErrors* outputGraph = new TGraphErrors(xValues.size(), &xValues[0], &yValues[0]);
    outputGraph->SetNameTitle("scatteringDistribution", "scatteringDistribution");
    outputGraph->Write();

    // write out detector counts
    vector<double> detectorAngles;
    vector<double> detectorCounts;

    for(auto& detector : detectors)
    {
        detectorAngles.push_back(detector.angle);

        double normalizationFactor = (crossSection.getIntegral()/(HIGH_CS_ANGLE-LOW_CS_ANGLE))
            /((double)iterations*detector.solidAngleFraction);
        detectorCounts.push_back(detector.counts*normalizationFactor);

        detector.distanceHisto.Write();
        detector.hitMap.Write();
        detector.XZProj.Write();
        detector.YZProj.Write();
    }

    TGraphErrors* detectorsGraph = new TGraphErrors(detectorAngles.size(), &detectorAngles[0], &detectorCounts[0]);
    detectorsGraph->SetNameTitle("detectors","detectors");
    detectorsGraph->Write();

    scatteringDirection2D.Write();
    scatteringDirectionHisto.Write();

    sampleDistribution.Write();

    outputFile->Close();

    return 0;
}
