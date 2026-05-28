#pragma once

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <atomic>
#include <cstdint>
#include <cstring>
#include <string>
#include <thread>
#include <vector>


class mavlink_multicast_forwarder
{
public:
  struct Config
  {
    int listen_port = 15000;
    std::string multicast_group = "239.10.0.1";
    int multicast_port = 5000;
    int ttl = 1;
    int socket_buffer_size =  16 * 1024 * 1024;
  };

public:
  mavlink_multicast_forwarder() : buffer_(MaxPacketSize)
  {
  }

  ~mavlink_multicast_forwarder()
  {
    Stop();
  }

  bool Start(const Config& config)
  {
    if (running_) {
      return true;
    }

    config_ = config;

    if (!OpenSockets()) {
      CloseSockets();
      return false;
    }

    running_ = true;
    worker_ = std::thread(&mavlink_multicast_forwarder::Run,this);
    return true;
  }

  void Stop()
  {
    if (!running_) {
      return;
    }
    running_ = false;
    CloseSockets();
    if (worker_.joinable()) {
      worker_.join();
    }
  }

  bool IsRunning() const
  {
    return running_;
  }

private:
  static constexpr int MaxPacketSize = 65535;

private:
  bool OpenSockets()
  {
    listen_fd_ =
      ::socket(AF_INET, SOCK_DGRAM, 0);

    if (listen_fd_ < 0) {
      return false;
    }

    int reuse = 1;
    ::setsockopt(listen_fd_,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));
    ::setsockopt(listen_fd_,SOL_SOCKET,SO_RCVBUF,&config_.socket_buffer_size,sizeof(config_.socket_buffer_size));

    sockaddr_in listen_addr{};

    listen_addr.sin_family = AF_INET;
    listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    listen_addr.sin_port = htons(static_cast<uint16_t>(config_.listen_port));

    if (::bind(listen_fd_,reinterpret_cast<sockaddr*>(&listen_addr), sizeof(listen_addr)) < 0)
    {
      return false;
    }

    send_fd_ = ::socket(AF_INET, SOCK_DGRAM, 0);

    if (send_fd_ < 0) {
      return false;
    }

    ::setsockopt(send_fd_,SOL_SOCKET,SO_SNDBUF, &config_.socket_buffer_size,sizeof(config_.socket_buffer_size));

    unsigned char ttl = static_cast<unsigned char>(config_.ttl);

    ::setsockopt(send_fd_,IPPROTO_IP,IP_MULTICAST_TTL,&ttl,sizeof(ttl));

    unsigned char loopback = 0;

    ::setsockopt(send_fd_,IPPROTO_IP,IP_MULTICAST_LOOP,&loopback,sizeof(loopback));

    return true;
  }

  void CloseSockets()
  {
    if (listen_fd_ >= 0)
    {
      ::close(listen_fd_);
      listen_fd_ = -1;
    }

    if (send_fd_ >= 0)
    {
      ::close(send_fd_);
      send_fd_ = -1;
    }
  }

  void Run()
  {
    sockaddr_in mcast_addr{};

    mcast_addr.sin_family = AF_INET;

    mcast_addr.sin_port = htons(static_cast<uint16_t>(config_.multicast_port));

    if (::inet_pton(AF_INET,config_.multicast_group.c_str(),&mcast_addr.sin_addr) != 1)
    {
      running_ = false;
      return;
    }

    while (running_)
    {
      ssize_t n = ::recvfrom(listen_fd_,buffer_.data(),buffer_.size(), 0,nullptr,nullptr);
      if (n <= 0) {
        continue;
      }

      ::sendto(send_fd_,buffer_.data(),static_cast<size_t>(n), 0,reinterpret_cast<sockaddr*>(&mcast_addr),
        sizeof(mcast_addr));
    }
  }

private:
  Config config_;
  int listen_fd_ = -1;
  int send_fd_ = -1;
  std::atomic_bool running_{false};
  std::thread worker_;
  std::vector<uint8_t> buffer_;
};

