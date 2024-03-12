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
  JoinSession = 3,
  quitSession = 4,
  addElement = 5,
  modifyElement = 6,
  deleteElement = 7,
  saveWhiteboard = 8,
  // server packet
  actionResponse = 9,
  broadcast = 10,
  tempIdResponse = 11
};

// The WhiteboardPacket class provide standard methods to create,
// read, and serialize the packet. This class is not in charge of any state or
// id management,
class WhiteboardPacket {
private:
  uint32_t version;
  // WhiteboardPacketType type;
  // uint32_t user_id;
  // uint32_t session_id;
  protobuf::whiteboardPacket packet;

public:
  // WhiteboardPacket(
  //     uint32_t _version = 1,
  //     WhiteboardPacketType _type = WhiteboardPacketType::createWhiteboard,
  //     uint32_t _user_id = 0, uint32_t _session_id = 0)
  //     : version(_version) {}
  //  type(_type), user_id(_user_id),
  // session_id(_session_id) {}
  WhiteboardPacket(uint32_t _version = 1) : version(_version) {}
  // ~WhiteboardPacket() { packet.release_action(); }
  // Methods for accessing packet data
  // uint8_t get_sessionId() const { return session_id; }
  // uint8_t get_userId() const { return user_id; }
  // WhiteboardPacketType get_type() const { return type; }
  size_t byte_size() { return packet.ByteSizeLong(); }

  // methods to create packet

  void new_create_whiteboard_request(uint32_t user_id);
  void new_create_session_request(uint32_t user_id);
  void new_quit_session_request(uint32_t user_id);
  void new_add_element_request(WhiteboardElements _element);
  void new_modify_element_request();
  void new_delete_element_request();
  void new_broadcast_request();
  void new_action_response(bool success, std::string msg);
  void new_error_request();

  void new_packet(protobuf::PacketAction packet_action);
  // methods to parse/decode packet (protobuf -> C++ class)

  // methods to send packet
  std::string serialize() const;
  void serialize(std::ostream *) const;
  void serialize(::google::protobuf::io::CodedOutputStream *buffer) const;
  void print() const;
};