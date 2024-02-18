#include <chrono>
#include <iomanip>
#include <iostream>
#include <openssl/evp.h>
#include <openssl/sha.h>

#include <random>
#include <sstream>

const std::string base_url = "https://localhost/whiteboard/";

std::string generateSessionId() {
  // Get current time since epoch in milliseconds
  auto now = std::chrono::system_clock::now().time_since_epoch();
  auto millis =
      std::chrono::duration_cast<std::chrono::milliseconds>(now).count();

  // Generate a SHA-256 hash of the current time to create a session ID
  std::ostringstream oss;
  oss << millis;
  std::string data = oss.str();
  unsigned char hash[SHA256_DIGEST_LENGTH];

  // Create a context for SHA-256 hash computation
  EVP_MD_CTX *ctx = EVP_MD_CTX_new();
  EVP_DigestInit_ex(ctx, EVP_sha256(), nullptr);
  EVP_DigestUpdate(ctx, data.c_str(), data.length());
  EVP_DigestFinal_ex(ctx, hash, nullptr);
  EVP_MD_CTX_free(ctx);

  // Convert the hash to a hexadecimal string
  std::ostringstream session_id;
  for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
    session_id << std::hex << std::setw(2) << std::setfill('0')
               << static_cast<int>(hash[i]);
  }

  return session_id.str();
}

std::string generateTemporaryUserId() {
  // Generate a random temporary user ID
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> dis(1000, 9999);
  return std::to_string(dis(gen));
}

std::string generateSharedWhiteboardUrl(const std::string &session_id) {
  // Generate a shared whiteboard URL using the session ID
  return base_url + session_id;
}