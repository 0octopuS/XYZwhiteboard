#include "packet.hpp"
#include "action.pb.h"
#include "packet.pb.h"
#include <cstdint>
#include <cstdio>
#include <iostream>

std::vector<std::string> packet_action_name{"",
                                            "CreateWhiteBoardRequest",
                                            "CreateSessionRequest",
                                            "JoinSessionRequest",
                                            "QuitSessionRequest",
                                            "AddElementRequest",
                                            "ModifyElementRequest",
                                            "DeleteElement",
                                            "SaveWhiteboardRequest",
                                            "ActionResponse",
                                            "BroadCast",
                                            "TempIDResponse"};
void WhiteboardPacket::serialize(std::ostream *buffer) const {
  packet.SerializeToOstream(buffer);
}

void WhiteboardPacket::serialize(
    ::google::protobuf::io::CodedOutputStream *buffer) const {

  buffer->WriteVarint32(packet.ByteSizeLong());
  packet.SerializeToCodedStream(buffer);
  buffer->WriteRaw("\n", 1);
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

void WhiteboardPacket::new_packet(protobuf::PacketAction packet_action) {
  packet.set_version(version);
  packet.set_packet_id(packet_id);
  // packet.set_packet_type(type);
  // packet.set_session_id(session_id);
  protobuf::PacketAction *action;
  action = packet.mutable_action();
  action->Swap(&packet_action);
  if (action->has_createwhiteboard()) {
    packet.set_packet_type(
        static_cast<uint32_t>(WhiteboardPacketType::createWhiteboard));
  } else if (action->has_createsession()) {
    packet.set_packet_type(
        static_cast<uint32_t>(WhiteboardPacketType::createSession));
  } else if (action->has_joinsession()) {
    packet.set_packet_type(
        static_cast<uint32_t>(WhiteboardPacketType::joinSession));

  } else if (action->has_quitsession()) {
    packet.set_packet_type(
        static_cast<uint32_t>(WhiteboardPacketType::quitSession));

  } else if (action->has_addelement()) {
    packet.set_packet_type(
        static_cast<uint32_t>(WhiteboardPacketType::addElement));

  } else if (action->has_modifyelement()) {
    packet.set_packet_type(
        static_cast<uint32_t>(WhiteboardPacketType::modifyElement));
  } else if (action->has_savewhiteboard()) {
    packet.set_packet_type(
        static_cast<uint32_t>(WhiteboardPacketType::saveWhiteboard));

  } else if (action->has_actionresponse()) {
    packet.set_packet_type(
        static_cast<uint32_t>(WhiteboardPacketType::actionResponse));
  } else if (action->has_broadcast()) {
    packet.set_packet_type(
        static_cast<uint32_t>(WhiteboardPacketType::broadcast));

  } else if (action->has_tempidresponse()) {
    packet.set_packet_type(
        static_cast<uint32_t>(WhiteboardPacketType::tempIdResponse));
  } else {
    printf("Unknown packet type!");
  }

#ifndef NDEBUG
  auto type_int = packet.packet_type();
  printf(">>> WhiteboardPacket::new_packet %d %s\n", type_int,
         packet_action_name[type_int].c_str());
#endif
}

const std::string WhiteboardPacket::get_packet_type_name() {
  return packet_action_name[static_cast<uint32_t>(type)];
}
void WhiteboardPacket::print() const {
  std::cout << "----------------------------------\n";
  std::cout << "   Packet type " << packet_action_name[packet.packet_type()]
            << std::endl;
  std::cout << "----------------------------------\n";
}