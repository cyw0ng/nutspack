set(daemon_grpc_proto "${CMAKE_CURRENT_BINARY_DIR}/fileSrv.proto")

set(daemon_proto_src "${CMAKE_CURRENT_BINARY_DIR}/fileSrv.pb.cc")
set(daemon_proto_hdr "${CMAKE_CURRENT_BINARY_DIR}/fileSrv.pb.h")
set(daemon_grpc_src "${CMAKE_CURRENT_BINARY_DIR}/fileSrv.grpc.pb.cc")
set(daemon_grpc_hdr "${CMAKE_CURRENT_BINARY_DIR}/fileSrv.grpc.pb.h")

find_package(Threads REQUIRED)

add_custom_command(
      OUTPUT "${daemon_grpc_src}" "${daemon_grpc_hdr}" "${daemon_proto_src}" "${daemon_proto_hdr}"
      COMMAND "protoc"
      ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${CMAKE_CURRENT_BINARY_DIR}"
        --plugin=protoc-gen-grpc="/usr/bin/grpc_cpp_plugin"
        "${daemon_grpc_proto}"
      DEPENDS "${daemon_grpc_proto}")

add_executable(daemon
    daemon.cc
    ${daemon_proto_src}
    ${daemon_grpc_src}
)

link_directories(/usr/lib/x86_64-linux-gnu/)

target_link_libraries(daemon
  protobuf
  grpc++
  grpc
  grpc++_reflection
)