// Copyright 2022, David Lawrence
// Subject to the terms in the LICENSE file found in the top-level directory.
//

#ifndef _ProtoCLuster_factory_EcalBarrelScFiIslandProtoClusters_h_
#define _ProtoCLuster_factory_EcalBarrelScFiIslandProtoClusters_h_

#include <random>

#include <JANA/JFactoryT.h>
#include <services/geometry/dd4hep/JDD4hep_service.h>
#include <algorithms/calorimetry/CalorimeterIslandCluster.h>
#include <services/log/Log_service.h>
#include <extensions/spdlog/SpdlogExtensions.h>

class ProtoCluster_factory_EcalBarrelScFiProtoClusters : public JFactoryT<edm4eic::ProtoCluster>, CalorimeterIslandCluster {

public:
    //------------------------------------------
    // Constructor
    ProtoCluster_factory_EcalBarrelScFiProtoClusters(){
        SetTag("EcalBarrelScFiProtoClusters");
        m_log = japp->GetService<Log_service>()->logger(GetTag());
    }

    //------------------------------------------
    // Init
    void Init() override{
        auto app = GetApplication();
        m_input_tag = "EcalBarrelScFiMergedHits";

        m_splitCluster=false;               // from ATHENA reconstruction.py
        m_minClusterHitEdep=1.0 * MeV;    // from ATHENA reconstruction.py
        m_minClusterCenterEdep=10.0 * MeV; // from ATHENA reconstruction.py

        // neighbour checking distances
        m_sectorDist=5.0 * cm;             // ?
        u_localDistXY={};     //{this, "localDistXY", {}};
        u_localDistXZ={30*mm, 30*mm};     //{this, "localDistXZ", {}};  n.b. 30*mm, 30*mm came from comment Maria Z. put into PR.
        u_localDistYZ={};     //{this, "localDistYZ", {}};
        u_globalDistRPhi={};  //{this, "globalDistRPhi", {}};
        u_globalDistEtaPhi={};//{this, "globalDistEtaPhi", {}};
        u_dimScaledLocalDistXY={};// from ATHENA reconstruction.py

        app->SetDefaultParameter("BEMC:EcalBarrelScFiProtoClusters:input_tag", m_input_tag, "Name of input collection to use");
        app->SetDefaultParameter("BEMC:EcalBarrelScFiProtoClusters:splitCluster",             m_splitCluster);
        app->SetDefaultParameter("BEMC:EcalBarrelScFiProtoClusters:minClusterHitEdep",  m_minClusterHitEdep);
        app->SetDefaultParameter("BEMC:EcalBarrelScFiProtoClusters:minClusterCenterEdep",     m_minClusterCenterEdep);
        app->SetDefaultParameter("BEMC:EcalBarrelScFiProtoClusters:sectorDist",   m_sectorDist);
        app->SetDefaultParameter("BEMC:EcalBarrelScFiProtoClusters:localDistXY",   u_localDistXY);
        app->SetDefaultParameter("BEMC:EcalBarrelScFiProtoClusters:localDistXZ",   u_localDistXZ);
        app->SetDefaultParameter("BEMC:EcalBarrelScFiProtoClusters:localDistYZ",  u_localDistYZ);
        app->SetDefaultParameter("BEMC:EcalBarrelScFiProtoClusters:globalDistRPhi",    u_globalDistRPhi);
        app->SetDefaultParameter("BEMC:EcalBarrelScFiProtoClusters:globalDistEtaPhi",    u_globalDistEtaPhi);
        app->SetDefaultParameter("BEMC:EcalBarrelScFiProtoClusters:dimScaledLocalDistXY",    u_dimScaledLocalDistXY);
        m_geoSvc = app->template GetService<JDD4hep_service>();

        AlgorithmInit(m_log);
    }

    //------------------------------------------
    // ChangeRun
    void ChangeRun(const std::shared_ptr<const JEvent> &event) override{
        AlgorithmChangeRun();
    }

    //------------------------------------------
    // Process
    void Process(const std::shared_ptr<const JEvent> &event) override{
        // Prefill inputs
        hits = event->Get<edm4eic::CalorimeterHit>(m_input_tag);

        // Call Process for generic algorithm
        AlgorithmProcess();

        // Hand owner of algorithm objects over to JANA
        Set(protoClusters);
        protoClusters.clear(); // not really needed, but better to not leave dangling pointers around
    }
};

#endif // _ProtoCLuster_factory_EcalBarrelScFiIslandProtoClusters_h_
