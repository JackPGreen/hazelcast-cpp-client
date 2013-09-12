//
// Created by sancar koyunlu on 9/12/13.
// Copyright (c) 2013 hazelcast. All rights reserved.


#include "CollectionAddAllRequest.h"
#include "Data.h"
#include "CollectionPortableHook.h"
#include "PortableWriter.h"

namespace hazelcast {
    namespace client {
        namespace collection {
            CollectionAddAllRequest::CollectionAddAllRequest(const std::string& name, const std::vector<serialization::Data>& valueList)
            : CollectionRequest(name)
            , valueList(valueList) {

            }

            int CollectionAddAllRequest::getClassId() const {
                return CollectionPortableHook::COLLECTION_ADD_ALL;
            }

            void CollectionAddAllRequest::writePortable(serialization::PortableWriter& writer) const {
                CollectionRequest::writePortable(writer);
                serialization::ObjectDataOutput & output = writer.getRawDataOutput();
                output.writeInt(valueList.size());
                for (int i = 0; i < valueList.size(); i++) {
                    valueList[i].writeData(output);
                }
            }
        }
    }
}