//
//  PortableReader.h
//  Server
//
//  Created by sancar koyunlu on 1/10/13.
//  Copyright (c) 2013 sancar koyunlu. All rights reserved.
//

#ifndef HAZELCAST_PORTABLE_READER
#define HAZELCAST_PORTABLE_READER

#include "ClassDefinition.h"
#include "BufferedDataInput.h"
#include "FieldDefinition.h"
#include "PortableSerializer.h"
#include "HazelcastException.h"

#include <iostream>
#include <string>
#include <memory>

using namespace std;

namespace hazelcast {
    namespace client {
        namespace serialization {

            class Portable;

            class BufferObjectDataInput;

            typedef unsigned char byte;

            class PortableReader {
                template<typename T>
                friend void operator >>(PortableReader& portableReader, T data);

            public:

                PortableReader(SerializationContext *serializationContext, BufferedDataInput& input, boost::shared_ptr<ClassDefinition> cd);

                PortableReader& operator [](std::string& fieldName);

                void readingFromDataInput();

                int readInt();

                long readLong();

                bool readBoolean();

                byte readByte();

                char readChar();

                double readDouble();

                float readFloat();

                short readShort();

                string readUTF();

                std::vector<byte> readByteArray();

                std::vector<char> readCharArray();

                std::vector<int> readIntArray();

                std::vector<long> readLongArray();

                std::vector<double> readDoubleArray();

                std::vector<float> readFloatArray();

                std::vector<short> readShortArray();

                template<typename T>
                void read(BufferedDataInput& dataInput, T& object, int factoryId, int classId, int dataVersion) {

//                    PortableFactory const *portableFactory;
//                    if (portableFactories.count(factoryId) != 0) {
//                        portableFactory = portableFactories.at(factoryId);
//                    } else {
//                        throw hazelcast::client::HazelcastException("Could not find PortableFactory for factoryId: " + hazelcast::client::util::to_string(factoryId));
//                    }
//
//                    std::auto_ptr<Portable> p(portableFactory->create(classId));
//                    if (p.get() == NULL) {
//                        throw hazelcast::client::HazelcastException("Could not create Portable for class-id: " + hazelcast::client::util::to_string(factoryId));
//                    }

                    boost::shared_ptr<ClassDefinition> cd;
//                    if (context->getVersion() == dataVersion) {
                    cd = context->lookup(factoryId, classId); // using serializationContext.version
                    PortableReader reader(context, dataInput, cd);
                    readPortable(reader, object);
//                    } else {
//                        cd = context->lookup(factoryId, classId, dataVersion); // registered during read
//                        PortableReader reader(this, dataInput, cd, PortableReader::MORPHING);
//                        p->readPortable(reader);
//                    }
                };

                template<typename T>
                void readPortable(T& portable) {
                    if (!cd->isFieldDefinitionExists(lastFieldName))
                        throw hazelcast::client::HazelcastException("UnknownFieldException" + lastFieldName);

                    bool isNull = input->readBoolean();
                    if (isNull) {
                        return;
                    }
                    read(*input, portable, cd->getFactoryId(), cd->getClassId(), cd->getVersion());
                };

                template<typename T>
                std::vector< T > readPortableArray() {
                    if (!cd->isFieldDefinitionExists(lastFieldName))
                        throw hazelcast::client::HazelcastException("UnknownFieldException" + lastFieldName);
                    int len = input->readInt();
                    std::vector< T > portables(len);
                    if (len > 0) {
                        int offset = input->position();
                        for (int i = 0; i < len; i++) {
                            int start = input->readInt(offset + i * sizeof (int));
                            input->position(start);
                            FieldDefinition fd = cd->get(lastFieldName);
                            read(*input, portables[i], fd.getFactoryId(), fd.getClassId(), cd->getVersion());
                        }
                    }
                    return portables;
                };

            private:

                int getPosition(std::string&);

                int getPosition(FieldDefinition *);

                int offset;
                bool raw;
                bool readingPortable;
                SerializationContext *context;
                boost::shared_ptr<ClassDefinition> cd;
                BufferedDataInput *input;
                std::string lastFieldName;
            };

            template<typename T>
            inline void operator >>(PortableReader& portableReader, T& data) {
                //TODO i probably need to add more here
                //........
                //TODO is base of portable(may be and dataSerializable) ???? NOT SURE
                portableReader.readingFromDataInput();
                portableReader.readPortable(data);
            };

        }
    }
}
#endif /* HAZELCAST_PORTABLE_READER */
