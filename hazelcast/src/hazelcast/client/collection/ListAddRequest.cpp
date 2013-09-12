//
// Created by sancar koyunlu on 9/12/13.
// Copyright (c) 2013 hazelcast. All rights reserved.


#include "ListAddRequest.h"
#include "CollectionPortableHook.h"
#include "PortableWriter.h"

namespace hazelcast {
    namespace client {
        namespace list {
            ListAddRequest::ListAddRequest(const std::string& name, const serialization::Data& valueList, int index)
            : collection::CollectionAddRequest(name, valueList)
            , index(index) {

            }

            int ListAddRequest::getClassId() const {
                return collection::CollectionPortableHook::LIST_ADD;
            }

            void ListAddRequest::writePortable(serialization::PortableWriter& writer) const {
                writer.writeInt("i", index);
                collection::CollectionAddRequest::writePortable(writer);
            }
        }
    }
}

