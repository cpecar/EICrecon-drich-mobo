// Copyright 2022, Thomas Britton
// Subject to the terms in the LICENSE file found in the top-level directory.
//

#pragma once

#include <random>

#include <JANA/JFactoryT.h>
#include <services/geometry/dd4hep/JDD4hep_service.h>
#include <algorithms/calorimetry/CalorimeterClusterMerger.h>
#include <services/log/Log_service.h>
#include <extensions/spdlog/SpdlogExtensions.h>



// Dummy factory for JFactoryGeneratorT
class Association_factory_EcalEndcapPInsertMergedClustersAssociations : public JFactoryT<edm4eic::MCRecoClusterParticleAssociation> {

public:
    //------------------------------------------
    // Constructor
    Association_factory_EcalEndcapPInsertMergedClustersAssociations(){
        SetTag("EcalEndcapPInsertMergedClustersAssociations");
    }
};



class Cluster_factory_EcalEndcapPInsertMergedClusters : public JFactoryT<edm4eic::Cluster>, CalorimeterClusterMerger {

public:
    //------------------------------------------
    // Constructor
    Cluster_factory_EcalEndcapPInsertMergedClusters(){
        SetTag("EcalEndcapPInsertMergedClusters");
    }

    //------------------------------------------
    // Init
    void Init() override{
        auto app = GetApplication();
        //-------- Configuration Parameters ------------
        m_input_tag="EcalEndcapPInsertClusters";
        m_inputAssociations_tag="EcalEndcapPInsertClustersAssociations";

        std::string tag=this->GetTag();
        std::shared_ptr<spdlog::logger> m_log = app->GetService<Log_service>()->logger(tag);

        app->SetDefaultParameter("EEMC:EcalEndcapPInsertMergedClusters:input_tag",      m_input_tag, "Name of input collection to use");
        app->SetDefaultParameter("EEMC:EcalEndcapPInsertMergedClusters:inputAssociations_tag",      m_inputAssociations_tag, "Name of input associations collection to use");

        // Get log level from user parameter or default
        std::string log_level_str = "info";
        auto pm = app->GetJParameterManager();
        pm->SetDefaultParameter(tag + ":LogLevel", log_level_str, "verbosity: trace, debug, info, warn, err, critical, off");
        m_log->set_level(eicrecon::ParseLogLevel(log_level_str));

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
        m_inputClusters=event->Get<edm4eic::Cluster>(m_input_tag);
        m_inputAssociations=event->Get<edm4eic::MCRecoClusterParticleAssociation>(m_inputAssociations_tag); 

        // Call Process for generic algorithm
        AlgorithmProcess();

        //outputs
        // Hand owner of algorithm objects over to JANA
        Set(m_outputClusters);
        event->Insert(m_outputAssociations, "EcalEndcapPInsertMergedClustersAssociations");
        m_outputClusters.clear(); // not really needed, but better to not leave dangling pointers around
        m_outputAssociations.clear();
    }

private:
    // Name of input data type (collection)
    std::string              m_input_tag;
    std::string              m_inputAssociations_tag;
};
