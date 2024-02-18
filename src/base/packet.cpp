#include "packet.hpp"
#include "action.pb.h"
#include "packet.pb.h"
#include <cstdio>
#include <iostream>

void WhiteboardPacket::serialize(std::ostream *buffer) const {
  packet.SerializeToOstream(buffer);
}

void WhiteboardPacket::serialize(
    ::google::protobuf::io::CodedOutputStream *buffer) const {

  buffer->WriteVarint32(packet.ByteSizeLong());
  packet.SerializeToCodedStream(buffer);
}
std::string WhiteboardPacket::serialize() const {

  std::string serialized_message;
  if (!packet.SerializeToString(&serialized_message)) {
    // Handle serialization failure
    std::cerr << "Failed to serialize WhiteboardPacket" << std::endl;
    return "";
  }
#ifndef NDEBUG
  printf(">>> WhiteboardPacket::serialize()\n");
  printf(">>> size of msg: %zu\n", serialized_message.size());
#endif
  return serialized_message;
}

void WhiteboardPacket::new_packet(whiteboard::PacketAction packet_action) {
#ifndef NDEBUG
  printf(">>> WhiteboardPacket::new_packet\n");
#endif
  // packet.set_version(version);
  // packet.set_packet_type(type);
  // packet.set_session_id(session_id);
  if (packet_action.has_createwhiteboard()) {
#ifndef NDEBUG
    printf(">>> TYPE: create whiteboard\n");
#endif
    packet.set_packet_type(
        static_cast<uint32_t>(WhiteboardPacketType::createWhiteboard));
    whiteboard::PacketAction *action = packet.mutable_action();
    action->Swap(&packet_action);
  }
}

void WhiteboardPacket::print() const {
  std::cout << "   Packet type " << packet.packet_type() << std::endl;
  std::cout << "   User id " << packet.user_id() << std::endl;
}