# Install script for directory: /Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/cmake

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "libprotobuf-lite" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0_mac/Debug/libprotobuf-lited.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobuf-lited.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobuf-lited.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobuf-lited.a")
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0_mac/Release/libprotobuf-lite.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobuf-lite.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobuf-lite.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobuf-lite.a")
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0_mac/MinSizeRel/libprotobuf-lite.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobuf-lite.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobuf-lite.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobuf-lite.a")
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0_mac/RelWithDebInfo/libprotobuf-lite.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobuf-lite.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobuf-lite.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobuf-lite.a")
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "libprotobuf" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0_mac/Debug/libprotobufd.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobufd.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobufd.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobufd.a")
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0_mac/Release/libprotobuf.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobuf.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobuf.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobuf.a")
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0_mac/MinSizeRel/libprotobuf.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobuf.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobuf.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobuf.a")
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0_mac/RelWithDebInfo/libprotobuf.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobuf.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobuf.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotobuf.a")
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "libprotoc" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0_mac/Debug/libprotocd.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotocd.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotocd.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotocd.a")
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0_mac/Release/libprotoc.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotoc.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotoc.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotoc.a")
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0_mac/MinSizeRel/libprotoc.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotoc.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotoc.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotoc.a")
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0_mac/RelWithDebInfo/libprotoc.a")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotoc.a" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotoc.a")
      execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libprotoc.a")
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protoc" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0_mac/Debug/protoc")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/protoc" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/protoc")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/protoc")
      endif()
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0_mac/Release/protoc")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/protoc" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/protoc")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/protoc")
      endif()
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0_mac/MinSizeRel/protoc")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/protoc" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/protoc")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/protoc")
      endif()
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0_mac/RelWithDebInfo/protoc")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/protoc" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/protoc")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/protoc")
      endif()
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "any.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/any.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "any.pb.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/any.pb.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "api.pb.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/api.pb.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "arena.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/arena.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "arenastring.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/arenastring.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler" TYPE FILE RENAME "code_generator.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/compiler/code_generator.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler" TYPE FILE RENAME "command_line_interface.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/compiler/command_line_interface.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler/cpp" TYPE FILE RENAME "cpp_generator.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/compiler/cpp/cpp_generator.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler/csharp" TYPE FILE RENAME "csharp_generator.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/compiler/csharp/csharp_generator.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler/csharp" TYPE FILE RENAME "csharp_names.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/compiler/csharp/csharp_names.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler/csharp" TYPE FILE RENAME "csharp_options.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/compiler/csharp/csharp_options.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler" TYPE FILE RENAME "importer.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/compiler/importer.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler/java" TYPE FILE RENAME "java_generator.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/compiler/java/java_generator.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler/java" TYPE FILE RENAME "java_names.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/compiler/java/java_names.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler/javanano" TYPE FILE RENAME "javanano_generator.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/compiler/javanano/javanano_generator.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler/js" TYPE FILE RENAME "js_generator.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/compiler/js/js_generator.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler/objectivec" TYPE FILE RENAME "objectivec_generator.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/compiler/objectivec/objectivec_generator.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler/objectivec" TYPE FILE RENAME "objectivec_helpers.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/compiler/objectivec/objectivec_helpers.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler" TYPE FILE RENAME "parser.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/compiler/parser.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler" TYPE FILE RENAME "plugin.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/compiler/plugin.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler" TYPE FILE RENAME "plugin.pb.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/compiler/plugin.pb.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler/python" TYPE FILE RENAME "python_generator.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/compiler/python/python_generator.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler/ruby" TYPE FILE RENAME "ruby_generator.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/compiler/ruby/ruby_generator.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "descriptor.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/descriptor.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "descriptor.pb.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/descriptor.pb.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "descriptor_database.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/descriptor_database.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "duration.pb.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/duration.pb.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "dynamic_message.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/dynamic_message.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "empty.pb.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/empty.pb.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "extension_set.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/extension_set.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "field_mask.pb.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/field_mask.pb.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "generated_enum_reflection.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/generated_enum_reflection.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "generated_enum_util.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/generated_enum_util.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "generated_message_reflection.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/generated_message_reflection.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "generated_message_util.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/generated_message_util.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/io" TYPE FILE RENAME "coded_stream.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/io/coded_stream.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/io" TYPE FILE RENAME "gzip_stream.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/io/gzip_stream.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/io" TYPE FILE RENAME "printer.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/io/printer.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/io" TYPE FILE RENAME "strtod.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/io/strtod.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/io" TYPE FILE RENAME "tokenizer.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/io/tokenizer.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/io" TYPE FILE RENAME "zero_copy_stream.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/io/zero_copy_stream.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/io" TYPE FILE RENAME "zero_copy_stream_impl.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/io/zero_copy_stream_impl.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/io" TYPE FILE RENAME "zero_copy_stream_impl_lite.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/io/zero_copy_stream_impl_lite.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "map.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/map.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "map_entry.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/map_entry.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "map_entry_lite.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/map_entry_lite.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "map_field.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/map_field.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "map_field_inl.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/map_field_inl.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "map_field_lite.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/map_field_lite.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "map_type_handler.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/map_type_handler.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "message.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/message.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "message_lite.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/message_lite.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "metadata.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/metadata.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "reflection.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/reflection.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "reflection_ops.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/reflection_ops.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "repeated_field.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/repeated_field.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "service.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/service.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "source_context.pb.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/source_context.pb.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "struct.pb.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/struct.pb.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "atomic_sequence_num.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/atomic_sequence_num.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "atomicops.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/atomicops.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "atomicops_internals_arm64_gcc.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/atomicops_internals_arm64_gcc.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "atomicops_internals_arm_gcc.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/atomicops_internals_arm_gcc.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "atomicops_internals_arm_qnx.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/atomicops_internals_arm_qnx.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "atomicops_internals_atomicword_compat.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/atomicops_internals_atomicword_compat.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "atomicops_internals_generic_gcc.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/atomicops_internals_generic_gcc.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "atomicops_internals_macosx.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/atomicops_internals_macosx.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "atomicops_internals_mips_gcc.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/atomicops_internals_mips_gcc.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "atomicops_internals_pnacl.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/atomicops_internals_pnacl.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "atomicops_internals_power.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/atomicops_internals_power.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "atomicops_internals_solaris.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/atomicops_internals_solaris.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "atomicops_internals_tsan.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/atomicops_internals_tsan.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "atomicops_internals_x86_gcc.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/atomicops_internals_x86_gcc.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "atomicops_internals_x86_msvc.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/atomicops_internals_x86_msvc.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "bytestream.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/bytestream.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "callback.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/callback.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "casts.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/casts.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "common.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/common.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "fastmem.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/fastmem.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "hash.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/hash.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "logging.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/logging.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "macros.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/macros.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "mutex.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/mutex.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "once.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/once.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "platform_macros.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/platform_macros.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "port.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/port.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "scoped_ptr.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/scoped_ptr.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "shared_ptr.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/shared_ptr.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "singleton.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/singleton.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "status.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/status.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "stl_util.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/stl_util.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "stringpiece.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/stringpiece.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "template_util.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/template_util.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/stubs" TYPE FILE RENAME "type_traits.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/stubs/type_traits.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "text_format.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/text_format.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "timestamp.pb.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/timestamp.pb.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "type.pb.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/type.pb.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "unknown_field_set.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/unknown_field_set.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/util" TYPE FILE RENAME "field_comparator.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/util/field_comparator.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/util" TYPE FILE RENAME "field_mask_util.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/util/field_mask_util.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/util" TYPE FILE RENAME "json_util.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/util/json_util.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/util" TYPE FILE RENAME "message_differencer.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/util/message_differencer.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/util" TYPE FILE RENAME "time_util.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/util/time_util.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/util" TYPE FILE RENAME "type_resolver.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/util/type_resolver.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/util" TYPE FILE RENAME "type_resolver_util.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/util/type_resolver_util.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "wire_format.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/wire_format.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "wire_format_lite.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/wire_format_lite.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "wire_format_lite_inl.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/wire_format_lite_inl.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-headers" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "wrappers.pb.h" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/wrappers.pb.h")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-protos" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "descriptor.proto" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/descriptor.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-protos" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "any.proto" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/any.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-protos" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "api.proto" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/api.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-protos" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "duration.proto" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/duration.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-protos" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "empty.proto" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/empty.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-protos" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "field_mask.proto" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/field_mask.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-protos" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "source_context.proto" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/source_context.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-protos" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "struct.proto" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/struct.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-protos" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "timestamp.proto" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/timestamp.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-protos" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "type.proto" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/type.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-protos" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf" TYPE FILE RENAME "wrappers.proto" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/wrappers.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-protos" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/google/protobuf/compiler" TYPE FILE RENAME "plugin.proto" FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0/src/google/protobuf/compiler/plugin.proto")
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-export" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/protobuf/protobuf-targets.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/protobuf/protobuf-targets.cmake"
         "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0_mac/CMakeFiles/Export/lib/cmake/protobuf/protobuf-targets.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/protobuf/protobuf-targets-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/protobuf/protobuf-targets.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/protobuf" TYPE FILE FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0_mac/CMakeFiles/Export/lib/cmake/protobuf/protobuf-targets.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/protobuf" TYPE FILE FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0_mac/CMakeFiles/Export/lib/cmake/protobuf/protobuf-targets-debug.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/protobuf" TYPE FILE FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0_mac/CMakeFiles/Export/lib/cmake/protobuf/protobuf-targets-minsizerel.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/protobuf" TYPE FILE FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0_mac/CMakeFiles/Export/lib/cmake/protobuf/protobuf-targets-relwithdebinfo.cmake")
  endif()
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/protobuf" TYPE FILE FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0_mac/CMakeFiles/Export/lib/cmake/protobuf/protobuf-targets-release.cmake")
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "protobuf-export" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/protobuf" TYPE DIRECTORY FILES "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0_mac/lib/cmake/protobuf/" REGEX "/protobuf\\-targets\\.cmake$" EXCLUDE)
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Users/jrsnail_cm/Work/u2project_logic/DependenceProjs/protobuf-3.0.0_mac/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
