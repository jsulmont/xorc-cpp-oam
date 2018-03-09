#include <iostream>
#include <tuple>
#include <vector>

#include <msgpack.hpp>

#include <cppoam/MsgPackParser.h>
#include <cppoam/ProgramRef.h>

bool readTestInputFromStdin(msgpack::object_handle &handle) {
  msgpack::unpacker unp;
  unp.reserve_buffer(800);

  const char *bufferBegin = unp.buffer();

  do {
    *unp.buffer() = std::cin.get();
    unp.buffer_consumed(1);
  } while (!unp.next(handle));

  return (unp.buffer() - bufferBegin) > 1;
}

void sendTestResultToStdout(int result) {
  using Response_t = std::tuple<
    std::vector<std::vector<int>>, // values
    std::vector<int>,              // coeffects
    std::vector<int>               // killed coeffects
  >;

  Response_t response {{{0, result}}, {}, {}};
  msgpack::pack(std::cout, response);
}

int main(int argc, char **argv) {
  msgpack::object_handle handle;

  while (readTestInputFromStdin(handle)) {
    cppoam::MsgPackParser parser(std::move(handle));

    if (parser.parseMessageType() == cppoam::MsgPackParser::Program) {
      ProgramRef program = parser.parseProgram();

      // TODO

      int res = 3;
      sendTestResultToStdout(res);
    }
  }

  return 0;
}
