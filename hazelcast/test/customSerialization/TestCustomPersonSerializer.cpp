#include "ObjectDataOutput.h"
#include "ObjectDataInput.h"
#include "TestCustomXSerializable.h"
#include "TestCustomPersonSerializer.h"

namespace hazelcast {
    namespace client {
        namespace test {

            void TestCustomPersonSerializer::write(serialization::ObjectDataOutput & out, const TestCustomPerson& object) {
                out.writeInt(999);
                out.writeUTF(object.getName());
                out.writeInt(999);
            };

            void TestCustomPersonSerializer::read(serialization::ObjectDataInput & in, TestCustomPerson& object) {
                int i = in.readInt();
                assert(i == 999);
                object.setName(in.readUTF());
                i = in.readInt();
                assert(i == 999);
            };

            int TestCustomPersonSerializer::getTypeId() const {
                return 999;
            };

        }
    }
}