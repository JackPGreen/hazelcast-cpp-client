//
// Created by sancar koyunlu on 9/6/13.
// Copyright (c) 2013 hazelcast. All rights reserved.



#ifndef HAZELCAST_LocalTargetCallableRequest
#define HAZELCAST_LocalTargetCallableRequest

#include "Address.h"
#include "ObjectDataOutput.h"
#include "hazelcast/client/executor/DataSerializableHook.h"
#include "IdentifiedDataSerializableRequest.h"
#include <string>

namespace hazelcast {
    namespace client {
        namespace executor {
            template <typename Callable>
            class LocalTargetCallableRequest : public impl::IdentifiedDataSerializableRequest {
            public:
                LocalTargetCallableRequest(const std::string &name, Callable &callable)
                :name(name)
                , callable(callable) {

                }

                int getFactoryId() const {
                    return DataSerializableHook::F_ID;
                }

                int getClassId() const {
                    return DataSerializableHook::LOCAL_TARGET_CALLABLE_REQUEST;
                }

                void writeData(serialization::ObjectDataOutput &writer) const {
                    writer.writeUTF(name);
                    writer.writeObject<Callable>(&callable);
                }

            private:
                const std::string &name;
                Callable &callable;
            };
        }
    }
}

#endif //HAZELCAST_LocalTargetCallableRequest
