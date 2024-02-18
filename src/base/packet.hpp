#include "action.pb.h"
#include "element.hpp"
#include "element.pb.h"
#include "packet.pb.h"
#include <cstdint>
#include <iostream>

enum class WhiteboardPacketType : uint32_t {
  // client packet
  createWhiteboard = 1,
  createShareUrl = 2,
  quitWhiteboard = 3,
  addElement = 4,
  modifyElement = 5,
  deleteElement = 6,
  // server packet
  broadcast = 7,
  confirmAction = 8,
  error = 9
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
  whiteboard::whiteboardPacket packet;

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
  void new_create_share_url_request();
  void new_quit_whiteboard_request();
  void new_add_element_request(const whiteboard::AddElementRequest &request);
  void new_modify_element_request();
  void new_delete_element_request();
  void new_broadcast_request();
  void new_confirm_action_request();
  void new_error_request();

  void new_packet(whiteboard::PacketAction packet_action);

  // methods to send packet
  std::string serialize() const;
  void serialize(std::ostream *) const;
  void serialize(::google::protobuf::io::CodedOutputStream *buffer) const;
  void print() const;
};