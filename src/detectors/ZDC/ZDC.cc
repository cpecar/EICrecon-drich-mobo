// Copyright 2022, David Lawrence
// Subject to the terms in the LICENSE file found in the top-level directory.
//
//

#include <Evaluator/DD4hepUnits.h>
#include <JANA/JApplication.h>
#include <math.h>
#include <string>

#include "algorithms/calorimetry/ImagingTopoClusterConfig.h"
#include "extensions/jana/JOmniFactoryGeneratorT.h"
#include "factories/calorimetry/CalorimeterClusterRecoCoG_factory.h"
#include "factories/calorimetry/CalorimeterHitDigi_factory.h"
#include "factories/calorimetry/CalorimeterHitReco_factory.h"
#include "factories/calorimetry/CalorimeterIslandCluster_factory.h"
#include "factories/calorimetry/CalorimeterTruthClustering_factory.h"
#include "factories/calorimetry/HEXPLIT_factory.h"
#include "factories/calorimetry/ImagingTopoCluster_factory.h"

extern "C" {
    void InitPlugin(JApplication *app) {

        using namespace eicrecon;

        InitJANAPlugin(app);

        // LYSO part of the ZDC
        app->Add(new JOmniFactoryGeneratorT<CalorimeterHitDigi_factory>(
          "EcalFarForwardZDCRawHits", {"EcalFarForwardZDCHits"}, {"EcalFarForwardZDCRawHits"},
          {
            .tRes = 0.0 * dd4hep::ns,
            .capADC = 32768,
            .dyRangeADC = 2000 * dd4hep::MeV,
            .pedMeanADC = 400,
            .pedSigmaADC = 3.2,
            .resolutionTDC = 10 * dd4hep::picosecond,
            .corrMeanScale = "1.0",
            .readout = "EcalFarForwardZDCHits",
          },
          app   // TODO: Remove me once fixed
        ));
        app->Add(new JOmniFactoryGeneratorT<CalorimeterHitReco_factory>(
          "EcalFarForwardZDCRecHits", {"EcalFarForwardZDCRawHits"}, {"EcalFarForwardZDCRecHits"},
          {
            .capADC = 32768,
            .dyRangeADC = 2000. * dd4hep::MeV,
            .pedMeanADC = 400,
            .pedSigmaADC = 3.2,
            .resolutionTDC = 10 * dd4hep::picosecond,
            .thresholdFactor = 4.0,
            .thresholdValue = 0.0,
            .sampFrac = "1.0",
            .readout = "EcalFarForwardZDCHits",
          },
          app   // TODO: Remove me once fixed
        ));
        app->Add(new JOmniFactoryGeneratorT<CalorimeterTruthClustering_factory>(
          "EcalFarForwardZDCTruthProtoClusters", {"EcalFarForwardZDCRecHits", "EcalFarForwardZDCHits"}, {"EcalFarForwardZDCTruthProtoClusters"},
          app   // TODO: Remove me once fixed
        ));
        app->Add(new JOmniFactoryGeneratorT<CalorimeterIslandCluster_factory>(
          "EcalFarForwardZDCIslandProtoClusters", {"EcalFarForwardZDCRecHits"}, {"EcalFarForwardZDCIslandProtoClusters"},
          {
            .sectorDist = 5.0 * dd4hep::cm,
            .localDistXY = {50 * dd4hep::cm, 50 * dd4hep::cm},
            .dimScaledLocalDistXY = {50.0*dd4hep::mm, 50.0*dd4hep::mm},
            .splitCluster = true,
            .minClusterHitEdep = 0.1 * dd4hep::MeV,
            .minClusterCenterEdep = 3.0 * dd4hep::MeV,
            .transverseEnergyProfileMetric = "globalDistEtaPhi",
            .transverseEnergyProfileScale = 1.,
          },
          app   // TODO: Remove me once fixed
        ));

        app->Add(
          new JOmniFactoryGeneratorT<CalorimeterClusterRecoCoG_factory>(
             "EcalFarForwardZDCTruthClusters",
            {"EcalFarForwardZDCTruthProtoClusters",        // edm4eic::ProtoClusterCollection
             "EcalFarForwardZDCHits"},                     // edm4hep::SimCalorimeterHitCollection
            {"EcalFarForwardZDCTruthClusters",             // edm4eic::Cluster
             "EcalFarForwardZDCTruthClusterAssociations"}, // edm4eic::MCRecoClusterParticleAssociation
            {
              .energyWeight = "log",
              .sampFrac = 1.0,
              .logWeightBase = 3.6,
              .longitudinalShowerInfoAvailable = true,
              .enableEtaBounds = false
            },
            app   // TODO: Remove me once fixed
          )
        );

        app->Add(
          new JOmniFactoryGeneratorT<CalorimeterClusterRecoCoG_factory>(
             "EcalFarForwardZDCClusters",
            {"EcalFarForwardZDCIslandProtoClusters",  // edm4eic::ProtoClusterCollection
             "EcalFarForwardZDCHits"},                // edm4hep::SimCalorimeterHitCollection
            {"EcalFarForwardZDCClusters",             // edm4eic::Cluster
             "EcalFarForwardZDCClusterAssociations"}, // edm4eic::MCRecoClusterParticleAssociation
            {
              .energyWeight = "log",
              .sampFrac = 1.0,
              .logWeightBase = 6.2,
              .longitudinalShowerInfoAvailable = true,
              .enableEtaBounds = false,
            },
            app   // TODO: Remove me once fixed
          )
        );

        app->Add(new JOmniFactoryGeneratorT<CalorimeterHitDigi_factory>(
          "HcalFarForwardZDCRawHits", {"HcalFarForwardZDCHits"}, {"HcalFarForwardZDCRawHits"},
          {
            .tRes = 0.0 * dd4hep::ns,
            .capADC = 32768,
            .dyRangeADC = 800 * dd4hep::MeV,
            .pedMeanADC = 400,
            .pedSigmaADC = 10,
            .resolutionTDC = 10 * dd4hep::picosecond,
            .corrMeanScale = "1.0",
            .readout = "HcalFarForwardZDCHits",
          },
          app   // TODO: Remove me once fixed
        ));

        app->Add(new JOmniFactoryGeneratorT<CalorimeterHitReco_factory>(
          "HcalFarForwardZDCRecHits", {"HcalFarForwardZDCRawHits"}, {"HcalFarForwardZDCRecHits"},
          {
            .capADC = 32678,
            .dyRangeADC = 800. * dd4hep::MeV,
            .pedMeanADC = 400,
            .pedSigmaADC = 10,
            .resolutionTDC = 10 * dd4hep::picosecond,
            .thresholdFactor = 0.0,
            .thresholdValue = -100.0,
            .sampFrac = "1.0",
            .readout = "HcalFarForwardZDCHits",
            .layerField = "layer",
            .sectorField = "system",
          },
          app   // TODO: Remove me once fixed
        ));

        app->Add(new JOmniFactoryGeneratorT<HEXPLIT_factory>(
          "HcalFarForwardZDCSubcellHits", {"HcalFarForwardZDCRecHits"}, {"HcalFarForwardZDCSubcellHits"},
          {
            .MIP = 472. * dd4hep::keV,
            .Emin_in_MIPs=0.5,
            .tmax=269 * dd4hep::ns,
          },
          app   // TODO: Remove me once fixed
        ));

        double side_length=31.3 * dd4hep::mm;
        app->Add(new JOmniFactoryGeneratorT<ImagingTopoCluster_factory>(
            "HcalFarForwardZDCImagingProtoClusters", {"HcalFarForwardZDCSubcellHits"}, {"HcalFarForwardZDCImagingProtoClusters"},
            {
                .neighbourLayersRange = 1,
                .localDistXY = {0.76*side_length, 0.76*side_length*sin(M_PI/3)},
                .layerDistXY = {0.76*side_length, 0.76*side_length*sin(M_PI/3)},
                .layerMode=eicrecon::ImagingTopoClusterConfig::ELayerMode::xy,
                .sectorDist = 10.0 * dd4hep::cm,
                .minClusterHitEdep = 100.0 * dd4hep::keV,
                .minClusterCenterEdep = 3.0 * dd4hep::MeV,
                .minClusterEdep = 11.0 * dd4hep::MeV,
                .minClusterNhits = 100,
            },
            app   // TODO: Remove me once fixed
        ));

        app->Add(new JOmniFactoryGeneratorT<CalorimeterIslandCluster_factory>(
          "HcalFarForwardZDCIslandProtoClusters", {"HcalFarForwardZDCSubcellHits"}, {"HcalFarForwardZDCIslandProtoClusters"},
          {
            .sectorDist = 1.5 * dd4hep::cm,
            .localDistXY = {0.9*side_length, 0.76*side_length*sin(M_PI/3)},
            .splitCluster = false,
            .minClusterHitEdep = 100.0 * dd4hep::keV,
            .minClusterCenterEdep = 1.0 * dd4hep::MeV,
            // .transverseEnergyProfileMetric = "globalDistEtaPhi",
            // .transverseEnergyProfileScale = 1.,
          },
          app
        ));

        app->Add(new JOmniFactoryGeneratorT<CalorimeterClusterRecoCoG_factory>(
           "HcalFarForwardZDCClusters",
          {"HcalFarForwardZDCImagingProtoClusters",  // edm4eic::ProtoClusterCollection
           "HcalFarForwardZDCHits"},                // edm4hep::SimCalorimeterHitCollection
          {"HcalFarForwardZDCClusters",             // edm4eic::Cluster
           "HcalFarForwardZDCClusterAssociations"}, // edm4eic::MCRecoClusterParticleAssociation
          {
            .energyWeight = "log",
            .sampFrac = 0.0203,
            .logWeightBaseCoeffs={5.0,0.65,0.31},
            .logWeightBase_Eref=50*dd4hep::GeV,
            .longitudinalShowerInfoAvailable = true,
          },
          app   // TODO: Remove me once fixed
        ));

        app->Add(new JOmniFactoryGeneratorT<CalorimeterTruthClustering_factory>(
          "HcalFarForwardZDCTruthProtoClusters", {"HcalFarForwardZDCRecHits", "HcalFarForwardZDCHits"}, {"HcalFarForwardZDCTruthProtoClusters"},
          app   // TODO: Remove me once fixed
        ));

        //Clusters with the baseline algorithm (no HEXPLIT)
        app->Add(new JOmniFactoryGeneratorT<CalorimeterIslandCluster_factory>(
          "HcalFarForwardZDCIslandProtoClustersBaseline", {"HcalFarForwardZDCRecHits"}, {"HcalFarForwardZDCIslandProtoClustersBaseline"},
          {
            .sectorDist = 5.0 * dd4hep::cm,
            .localDistXY = {50 * dd4hep::cm, 50 * dd4hep::cm},
            .dimScaledLocalDistXY = {50.0*dd4hep::mm, 50.0*dd4hep::mm},
            .splitCluster = true,
            .minClusterHitEdep = 0.1 * dd4hep::MeV,
            .minClusterCenterEdep = 3.0 * dd4hep::MeV,
            .transverseEnergyProfileMetric = "globalDistEtaPhi",
            .transverseEnergyProfileScale = 1.,
          },
          app   // TODO: Remove me once fixed
        ));

        app->Add(new JOmniFactoryGeneratorT<CalorimeterClusterRecoCoG_factory>(
             "HcalFarForwardZDCTruthClusters",
            {"HcalFarForwardZDCTruthProtoClusters",        // edm4eic::ProtoClusterCollection
             "HcalFarForwardZDCHits"},                     // edm4hep::SimCalorimeterHitCollection
            {"HcalFarForwardZDCTruthClusters",             // edm4eic::Cluster
             "HcalFarForwardZDCTruthClusterAssociations"}, // edm4eic::MCRecoClusterParticleAssociation
            {
              .energyWeight = "log",
              .sampFrac = 1.0,
              .logWeightBase = 3.6,
              .longitudinalShowerInfoAvailable = true,
              .enableEtaBounds = false
            },
            app   // TODO: Remove me once fixed
          )
        );

        app->Add(new JOmniFactoryGeneratorT<CalorimeterClusterRecoCoG_factory>(
             "HcalFarForwardZDCClustersBaseline",
            {"HcalFarForwardZDCIslandProtoClustersBaseline",  // edm4eic::ProtoClusterCollection
             "HcalFarForwardZDCHits"},                // edm4hep::SimCalorimeterHitCollection
            {"HcalFarForwardZDCClustersBaseline",             // edm4eic::Cluster
             "HcalFarForwardZDCClusterAssociationsBaseline"}, // edm4eic::MCRecoClusterParticleAssociation
            {
              .energyWeight = "log",
              .sampFrac = 0.0203,
              .logWeightBase = 6.2,
              .longitudinalShowerInfoAvailable = true,
              .enableEtaBounds = false,
            },
            app   // TODO: Remove me once fixed
          )
        );
    }
}
