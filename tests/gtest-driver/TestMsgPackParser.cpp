#include <cppoam/MsgPackParser.h>
#include <cppoam/ProgramRef.h>

#include <gtest/gtest.h>

TEST(TestMsgPackParser, Parse_1plus2)
{
  msgpack::sbuffer buffer;
  msgpack::packer<msgpack::sbuffer> pk(&buffer);

  // OAM program for expression: '1 + 2'
  pk.pack_array(2); {
    pk.pack_int(0);
    pk.pack_array(2); {
      pk.pack_array(1); {
        std::string ffi = "core.add";
        pk.pack_str(ffi.size());
        pk.pack_str_body(ffi.data(), ffi.size());
      }
      pk.pack_array(2); {
        pk.pack_array(18); {
          pk.pack_int(2);
          pk.pack_int(5);
          pk.pack_int(8);
          pk.pack_int(1);
          pk.pack_int(8);
          pk.pack_int(2);
          pk.pack_int(4);
          pk.pack_int(0);
          pk.pack_int(1);
          pk.pack_array(2); {
            pk.pack_int(0);
            pk.pack_int(1);
          }
          pk.pack_int(2);
          pk.pack_int(2);
          pk.pack_int(1);
          pk.pack_int(1);
          pk.pack_int(2);
          pk.pack_int(3);
          pk.pack_int(0);
          pk.pack_int(0);
        }
        pk.pack_array(5); {
          pk.pack_int(2);
          pk.pack_int(1);
          pk.pack_int(11);
          pk.pack_int(0);
          pk.pack_array(2); {
            pk.pack_int(0);
            pk.pack_int(1);
          }
        }
      }
    }
  }

  msgpack::unpacker pac;
  pac.reserve_buffer(buffer.size());
  memcpy(pac.buffer(), buffer.data(), buffer.size());
  pac.buffer_consumed(buffer.size());

  msgpack::object_handle handle;
  pac.next(handle);

  std::ostringstream ostr;
  ostr << handle.get();

  ASSERT_EQ("[0, [[\"core.add\"], ["
              "[2, 5, 8, 1, 8, 2, 4, 0, 1, [0, 1], 2, 2, 1, 1, 2, 3, 0, 0], "
              "[2, 1, 11, 0, [0, 1]]"
            "]]]", ostr.str());

  cppoam::MsgPackParser parser(std::move(handle));

  auto type = parser.parseMessageType();
  EXPECT_EQ(cppoam::MsgPackParser::ByteCode, type);

  cppoam::ProgramRef program = parser.parseProgram();

  EXPECT_EQ(1, program.getNumFFI());
  EXPECT_EQ("core.add", program.getFFI(0));

  EXPECT_EQ(2, program.getNumFunctions());
  EXPECT_EQ(5, program.getFunction(0).getNumInstructions());
  EXPECT_EQ(1, program.getFunction(1).getNumInstructions());
}

TEST(TestMsgPackParser, Evaluate_1parallel2)
{
  msgpack::sbuffer buffer;
  msgpack::packer<msgpack::sbuffer> pk(&buffer);

  // OAM program for expression: '1 | 2'
  pk.pack_array(2); {
    pk.pack_int(0);
    pk.pack_array(2); {
      pk.pack_array(0); {
      }
      pk.pack_array(1); {
        pk.pack_array(9); {
          pk.pack_int(0);
          pk.pack_int(3);
          pk.pack_int(8);
          pk.pack_int(1);
          pk.pack_int(8);
          pk.pack_int(2);
          pk.pack_int(0);
          pk.pack_int(1);
          pk.pack_int(0);
        }
      }
    }
  }

  msgpack::unpacker pac;
  pac.reserve_buffer(buffer.size());
  memcpy(pac.buffer(), buffer.data(), buffer.size());
  pac.buffer_consumed(buffer.size());

  msgpack::object_handle handle;
  pac.next(handle);

  std::ostringstream ostr;
  ostr << handle.get();

  ASSERT_EQ("[0, [[], [[0, 3, 8, 1, 8, 2, 0, 1, 0]]]]", ostr.str());

  cppoam::MsgPackParser parser(std::move(handle));
  EXPECT_EQ(cppoam::MsgPackParser::ByteCode, parser.parseMessageType());

  cppoam::ProgramRef program = parser.parseProgram();
  EXPECT_EQ(0, program.getNumFFI());
  EXPECT_EQ(1, program.getNumFunctions());
  EXPECT_EQ(3, program.getFunction(0).getNumInstructions());
}
