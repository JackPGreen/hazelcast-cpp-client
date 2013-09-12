//
// Created by sancar koyunlu on 9/12/13.
// Copyright (c) 2013 hazelcast. All rights reserved.


#include "CollectionSizeRequest.h"
#include "CollectionPortableHook.h"

namespace hazelcast {
    namespace client {
        namespace collection {

            CollectionSizeRequest::CollectionSizeRequest(const std::string& name)
            : CollectionRequest(name) {

            }

            int CollectionSizeRequest::getClassId() const {
                return CollectionPortableHook::COLLECTION_SIZE;
            }


        }
    }
}