// SPDX-License-Identifier: LGPL-3.0-or-later
// Copyright (C) 2022 Wouter Deconinck

#pragma once

#include <JANA/JEvent.h>
#include <edm4eic/InclusiveKinematicsCollection.h>
#include <cstddef>
#include <memory>
#include <string>
#include <typeindex>
#include <utility>
#include <vector>

#include "algorithms/reco/InclusiveKinematicsJB.h"
#include "extensions/jana/JChainMultifactoryT.h"
#include "extensions/spdlog/SpdlogMixin.h"

namespace eicrecon {

    class InclusiveKinematicsJB_factory :
            public JChainMultifactoryT<>,
            public SpdlogMixin {

    public:

        explicit InclusiveKinematicsJB_factory(
            std::string tag,
            const std::vector<std::string>& input_tags,
            const std::vector<std::string>& output_tags)
        : JChainMultifactoryT<>(std::move(tag), input_tags, output_tags) {

            DeclarePodioOutput<edm4eic::InclusiveKinematics>(GetOutputTags()[0]);

        }

        /** One time initialization **/
        void Init() override;

        /** Event by event processing **/
        void Process(const std::shared_ptr<const JEvent> &event) override;

    protected:
        InclusiveKinematicsJB m_inclusive_kinematics_algo;

    };

} // eicrecon
