//
// Created by sancar koyunlu on 6/25/13.
// Copyright (c) 2013 hazelcast. All rights reserved.


#ifndef HAZELCAST_CountRequest
#define HAZELCAST_CountRequest

#include "RetryableRequest.h"
#include "KeyBasedRequest.h"

namespace hazelcast {
    namespace client {
        namespace multimap {
            class CountRequest : public KeyBasedRequest, public RetryableRequest {
            public:
                CountRequest(const std::string& name, const serialization::Data& key);

                int getClassId() const;
            };
        }
    }
}

#endif //HAZELCAST_CountRequest
