#ifndef PACKET_HPP
#define PACKET_HPP
#include "action.pb.h"
#include "element.hpp"
#include "element.pb.h"
#include "packet.pb.h"
#include <cstdint>
#include <iostream>
#include <sys/types.h>

enum class WhiteboardPacketType : uint32_t {
  // client packet
  createWhiteboard = 1,
  createSession = 2,
  joinSession = 3,
  quitSession = 4,
  addElement = 5,
  modifyElement = 6,
  deleteElement = 7,
  saveWhiteboard = 8,
  registerRequest = 9,
  loginRequest = 10,
  // server packet
  actionResponse = 11,
  broadcast = 12,
  tempIdResponse = 13,

};

// The WhiteboardPacket class provide standard methods to create,
// read, and serialize the packet. This class is not in charge of any state or
// id management,
class WhiteboardPacket {
private:
  uint32_t version;
  WhiteboardPacketType type;
  uint32_t packet_id;
  // uint32_t session_id;


public:
  // WhiteboardPacket(
  //     uint32_t _version = 1,
  //     WhiteboardPacketType _type = WhiteboardPacketType::createWhiteboard,
  //     uint32_t _user_id = 0, uint32_t _session_id = 0)
  //     : version(_version) {}
  //  type(_type), user_id(_user_id),
  // session_id(_session_id) {}
  protobuf::whiteboardPacket packet;
  WhiteboardPacket(uint32_t _version, WhiteboardPacketType _type,
                   uint32_t _packet_id)
      : version(_version), type(_type), packet_id(_packet_id) {}
  WhiteboardPacket(protobuf::whiteboardPacket &_packet) {
    type = static_cast<WhiteboardPacketType>(packet.packet_type());
    std::swap(_packet, packet);
    packet.set_packet_id(packet_id);
  }
  WhiteboardPacket(const protobuf::whiteboardPacket &_packet) {
    type = static_cast<WhiteboardPacketType>(packet.packet_type());
    packet = std::move(_packet);
    packet.set_packet_id(packet_id);
  }
  // ~WhiteboardPacket() { packet.release_action(); }
  // Methods for accessing packet data
  // uint8_t get_sessionId() const { return session_id; }
  // uint8_t get_userId() const { return user_id; }
  // WhiteboardPacketType get_type() const { return type; }
  size_t byte_size() { return packet.ByteSizeLong(); }

  // methods to create packet

  void new_create_whiteboard_request(uint32_t user_id);
  void new_create_session_request(uint32_t user_id, std::string whiteboard_id);
  void new_join_session_request(uint32_t user_id, std::string whiteboard_id);
  void new_quit_session_request(uint32_t user_id, std::string whiteboard_id);
  void new_add_element_request(uint32_t user_id, std::string whiteboard_id,
                               WhiteboardElements _element);
  void new_modify_element_request(uint32_t user_id, std::string whiteboard_id,
                                  WhiteboardElements _orig_element,
                                  WhiteboardElements _element);
  void new_delete_element_request(uint32_t user_id, std::string whiteboard_id,
                                  WhiteboardElements _orig_element);
  void new_login_request(std::string username, std::string password_hash);
  void new_register_request(std::string username, std::string password_hash);
  void new_broadcast(std::vector<protobuf::Element>);
  void new_temp_id_response(bool success, uint32_t user_id);
  void new_action_response(bool success, std::string msg);
  void new_error_request();

  void new_packet(protobuf::PacketAction packet_action);
  // methods to parse/decode packet (protobuf -> C++ class)

  // methods to get packet info
  const uint32_t get_packet_id() const { return packet_id; }
  const WhiteboardPacketType get_packet_type() const { return type; }
  const std::string get_packet_type_name();
  // methods to send packet
  std::string serialize() const;
  void serialize(std::ostream *) const;
  void serialize(::google::protobuf::io::CodedOutputStream *buffer) const;
  void print() const;
};
#endif
