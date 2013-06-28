//
//  PortableReader.cpp
//  Server
//
//  Created by sancar koyunlu on 1/10/13.
//  Copyright (c) 2013 sancar koyunlu. All rights reserved.
//
#include "SerializationContext.h"
#include "PortableReader.h"

namespace hazelcast {
    namespace client {
        namespace serialization {

            PortableReader::PortableReader(SerializationContext *serializationContext, BufferedDataInput& input, ClassDefinition *cd)
            : input(input)
            , context(serializationContext)
            , cd(cd)
            , readingPortable(false)
            , raw(false)
            , offset(input.position()) {

            };

            PortableReader & PortableReader::operator [](const char *fieldName) {
//            PortableReader & PortableReader::operator [](const std::string& fieldName) {
                if (raw) {
                    throw hazelcast::client::HazelcastException("Cannot call [] operation after reading  directly from stream(without [])");
                }
                input.position(getPosition(fieldName));
                const FieldDefinition& fd = cd->get(fieldName);
                currentFactoryId = fd.getFactoryId();
                currentClassId = fd.getClassId();
                readingPortable = true;
                return *this;
            }

            void PortableReader::readingFromDataInput() {
                if (readingPortable) {
                    readingPortable = false;
                } else if (!raw) {
                    input.position(offset + cd->getFieldCount() * 4);
                    int pos = input.readInt();
                    input.position(pos);
                    raw = true;
                }
            };


            int PortableReader::skipBytes(int i) {
                throw hazelcast::client::HazelcastException("Not supported");
            };

            void PortableReader::readFully(std::vector<byte>& bytes) {
                input.readFully(bytes);
            };

            int PortableReader::readInt() {
                return input.readInt();
            };

            long PortableReader::readLong() {
                return input.readLong();
            };

            bool PortableReader::readBoolean() {
                return input.readBoolean();
            };

            byte PortableReader::readByte() {
                return input.readByte();
            };

            char PortableReader::readChar() {
                return input.readChar();
            };

            double PortableReader::readDouble() {
                return input.readDouble();
            };

            float PortableReader::readFloat() {
                return input.readFloat();
            };

            short PortableReader::readShort() {
                return input.readShort();
            };

            string PortableReader::readUTF() {
                return input.readUTF();
            };

            std::vector <byte> PortableReader::readByteArray() {
                return input.readByteArray();
            };

            std::vector<char> PortableReader::readCharArray() {
                return input.readCharArray();
            };

            std::vector<int> PortableReader::readIntArray() {
                return input.readIntArray();
            };

            std::vector<long> PortableReader::readLongArray() {
                return input.readLongArray();
            };

            std::vector<double> PortableReader::readDoubleArray() {
                return input.readDoubleArray();
            };

            std::vector<float> PortableReader::readFloatArray() {
                return input.readFloatArray();
            };

            std::vector<short> PortableReader::readShortArray() {
                return input.readShortArray();
            };

//            int PortableReader::getPosition(const std::string& fieldName) {
            int PortableReader::getPosition(const char *fieldName) {
                if (raw) {
                    throw hazelcast::client::HazelcastException("Cannot read Portable fields after getRawDataInput() is called!");
                }
//                if (!cd->isFieldDefinitionExists(fieldName.c_str()))
                if (!cd->isFieldDefinitionExists(fieldName))
                    throw hazelcast::client::HazelcastException("PortableReader::getPosition : unknownField " + std::string(fieldName));
                input.position(offset + cd->get(fieldName).getIndex() * sizeof (int));
                return input.readInt();
            };

        }
    }
}