// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: SnapshotRsp.proto

#ifndef PROTOBUF_SnapshotRsp_2eproto__INCLUDED
#define PROTOBUF_SnapshotRsp_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 3000000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 3000000 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message_lite.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
// @@protoc_insertion_point(includes)

// Internal implementation detail -- do not call these.
void protobuf_AddDesc_SnapshotRsp_2eproto();
void protobuf_AssignDesc_SnapshotRsp_2eproto();
void protobuf_ShutdownFile_SnapshotRsp_2eproto();

class SnapshotRsp;
class SnapshotRsp_MovableSnapshot;

// ===================================================================

class SnapshotRsp_MovableSnapshot : public ::google::protobuf::MessageLite /* @@protoc_insertion_point(class_definition:SnapshotRsp.MovableSnapshot) */ {
 public:
  SnapshotRsp_MovableSnapshot();
  virtual ~SnapshotRsp_MovableSnapshot();

  SnapshotRsp_MovableSnapshot(const SnapshotRsp_MovableSnapshot& from);

  inline SnapshotRsp_MovableSnapshot& operator=(const SnapshotRsp_MovableSnapshot& from) {
    CopyFrom(from);
    return *this;
  }

  static const SnapshotRsp_MovableSnapshot& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const SnapshotRsp_MovableSnapshot* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(SnapshotRsp_MovableSnapshot* other);

  // implements Message ----------------------------------------------

  inline SnapshotRsp_MovableSnapshot* New() const { return New(NULL); }

  SnapshotRsp_MovableSnapshot* New(::google::protobuf::Arena* arena) const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const SnapshotRsp_MovableSnapshot& from);
  void MergeFrom(const SnapshotRsp_MovableSnapshot& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  void DiscardUnknownFields();
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(SnapshotRsp_MovableSnapshot* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _arena_ptr_;
  }
  inline ::google::protobuf::Arena* MaybeArenaPtr() const {
    return _arena_ptr_;
  }
  public:

  ::std::string GetTypeName() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional fixed64 userId = 1;
  void clear_userid();
  static const int kUserIdFieldNumber = 1;
  ::google::protobuf::uint64 userid() const;
  void set_userid(::google::protobuf::uint64 value);

  // optional fixed32 heroId = 2;
  void clear_heroid();
  static const int kHeroIdFieldNumber = 2;
  ::google::protobuf::uint32 heroid() const;
  void set_heroid(::google::protobuf::uint32 value);

  // optional fixed32 type = 3;
  void clear_type();
  static const int kTypeFieldNumber = 3;
  ::google::protobuf::uint32 type() const;
  void set_type(::google::protobuf::uint32 value);

  // optional bytes nickname = 4;
  void clear_nickname();
  static const int kNicknameFieldNumber = 4;
  const ::std::string& nickname() const;
  void set_nickname(const ::std::string& value);
  void set_nickname(const char* value);
  void set_nickname(const void* value, size_t size);
  ::std::string* mutable_nickname();
  ::std::string* release_nickname();
  void set_allocated_nickname(::std::string* nickname);

  // optional fixed32 hp = 5;
  void clear_hp();
  static const int kHpFieldNumber = 5;
  ::google::protobuf::uint32 hp() const;
  void set_hp(::google::protobuf::uint32 value);

  // optional float speed = 6;
  void clear_speed();
  static const int kSpeedFieldNumber = 6;
  float speed() const;
  void set_speed(float value);

  // optional float effDistance = 7;
  void clear_effdistance();
  static const int kEffDistanceFieldNumber = 7;
  float effdistance() const;
  void set_effdistance(float value);

  // optional bool alive = 8;
  void clear_alive();
  static const int kAliveFieldNumber = 8;
  bool alive() const;
  void set_alive(bool value);

  // optional float x = 9;
  void clear_x();
  static const int kXFieldNumber = 9;
  float x() const;
  void set_x(float value);

  // optional float y = 10;
  void clear_y();
  static const int kYFieldNumber = 10;
  float y() const;
  void set_y(float value);

  // optional float vx = 11;
  void clear_vx();
  static const int kVxFieldNumber = 11;
  float vx() const;
  void set_vx(float value);

  // optional float vy = 12;
  void clear_vy();
  static const int kVyFieldNumber = 12;
  float vy() const;
  void set_vy(float value);

  // optional fixed64 timestamp = 13;
  void clear_timestamp();
  static const int kTimestampFieldNumber = 13;
  ::google::protobuf::uint64 timestamp() const;
  void set_timestamp(::google::protobuf::uint64 value);

  // @@protoc_insertion_point(class_scope:SnapshotRsp.MovableSnapshot)
 private:

  ::google::protobuf::internal::ArenaStringPtr _unknown_fields_;
  ::google::protobuf::Arena* _arena_ptr_;

  bool _is_default_instance_;
  ::google::protobuf::uint64 userid_;
  ::google::protobuf::uint32 heroid_;
  ::google::protobuf::uint32 type_;
  ::google::protobuf::internal::ArenaStringPtr nickname_;
  ::google::protobuf::uint32 hp_;
  float speed_;
  float effdistance_;
  bool alive_;
  float x_;
  float y_;
  float vx_;
  float vy_;
  ::google::protobuf::uint64 timestamp_;
  mutable int _cached_size_;
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_SnapshotRsp_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_SnapshotRsp_2eproto();
  #endif
  friend void protobuf_AssignDesc_SnapshotRsp_2eproto();
  friend void protobuf_ShutdownFile_SnapshotRsp_2eproto();

  void InitAsDefaultInstance();
  static SnapshotRsp_MovableSnapshot* default_instance_;
};
// -------------------------------------------------------------------

class SnapshotRsp : public ::google::protobuf::MessageLite /* @@protoc_insertion_point(class_definition:SnapshotRsp) */ {
 public:
  SnapshotRsp();
  virtual ~SnapshotRsp();

  SnapshotRsp(const SnapshotRsp& from);

  inline SnapshotRsp& operator=(const SnapshotRsp& from) {
    CopyFrom(from);
    return *this;
  }

  static const SnapshotRsp& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const SnapshotRsp* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(SnapshotRsp* other);

  // implements Message ----------------------------------------------

  inline SnapshotRsp* New() const { return New(NULL); }

  SnapshotRsp* New(::google::protobuf::Arena* arena) const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const SnapshotRsp& from);
  void MergeFrom(const SnapshotRsp& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  void DiscardUnknownFields();
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  void InternalSwap(SnapshotRsp* other);
  private:
  inline ::google::protobuf::Arena* GetArenaNoVirtual() const {
    return _arena_ptr_;
  }
  inline ::google::protobuf::Arena* MaybeArenaPtr() const {
    return _arena_ptr_;
  }
  public:

  ::std::string GetTypeName() const;

  // nested types ----------------------------------------------------

  typedef SnapshotRsp_MovableSnapshot MovableSnapshot;

  // accessors -------------------------------------------------------

  // optional string taskId = 1;
  void clear_taskid();
  static const int kTaskIdFieldNumber = 1;
  const ::std::string& taskid() const;
  void set_taskid(const ::std::string& value);
  void set_taskid(const char* value);
  void set_taskid(const char* value, size_t size);
  ::std::string* mutable_taskid();
  ::std::string* release_taskid();
  void set_allocated_taskid(::std::string* taskid);

  // optional string version = 2;
  void clear_version();
  static const int kVersionFieldNumber = 2;
  const ::std::string& version() const;
  void set_version(const ::std::string& value);
  void set_version(const char* value);
  void set_version(const char* value, size_t size);
  ::std::string* mutable_version();
  ::std::string* release_version();
  void set_allocated_version(::std::string* version);

  // optional int32 code = 3;
  void clear_code();
  static const int kCodeFieldNumber = 3;
  ::google::protobuf::int32 code() const;
  void set_code(::google::protobuf::int32 value);

  // repeated .SnapshotRsp.MovableSnapshot snapshots = 4;
  int snapshots_size() const;
  void clear_snapshots();
  static const int kSnapshotsFieldNumber = 4;
  const ::SnapshotRsp_MovableSnapshot& snapshots(int index) const;
  ::SnapshotRsp_MovableSnapshot* mutable_snapshots(int index);
  ::SnapshotRsp_MovableSnapshot* add_snapshots();
  ::google::protobuf::RepeatedPtrField< ::SnapshotRsp_MovableSnapshot >*
      mutable_snapshots();
  const ::google::protobuf::RepeatedPtrField< ::SnapshotRsp_MovableSnapshot >&
      snapshots() const;

  // @@protoc_insertion_point(class_scope:SnapshotRsp)
 private:

  ::google::protobuf::internal::ArenaStringPtr _unknown_fields_;
  ::google::protobuf::Arena* _arena_ptr_;

  bool _is_default_instance_;
  ::google::protobuf::internal::ArenaStringPtr taskid_;
  ::google::protobuf::internal::ArenaStringPtr version_;
  ::google::protobuf::RepeatedPtrField< ::SnapshotRsp_MovableSnapshot > snapshots_;
  ::google::protobuf::int32 code_;
  mutable int _cached_size_;
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_SnapshotRsp_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_SnapshotRsp_2eproto();
  #endif
  friend void protobuf_AssignDesc_SnapshotRsp_2eproto();
  friend void protobuf_ShutdownFile_SnapshotRsp_2eproto();

  void InitAsDefaultInstance();
  static SnapshotRsp* default_instance_;
};
// ===================================================================


// ===================================================================

#if !PROTOBUF_INLINE_NOT_IN_HEADERS
// SnapshotRsp_MovableSnapshot

// optional fixed64 userId = 1;
inline void SnapshotRsp_MovableSnapshot::clear_userid() {
  userid_ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 SnapshotRsp_MovableSnapshot::userid() const {
  // @@protoc_insertion_point(field_get:SnapshotRsp.MovableSnapshot.userId)
  return userid_;
}
inline void SnapshotRsp_MovableSnapshot::set_userid(::google::protobuf::uint64 value) {
  
  userid_ = value;
  // @@protoc_insertion_point(field_set:SnapshotRsp.MovableSnapshot.userId)
}

// optional fixed32 heroId = 2;
inline void SnapshotRsp_MovableSnapshot::clear_heroid() {
  heroid_ = 0u;
}
inline ::google::protobuf::uint32 SnapshotRsp_MovableSnapshot::heroid() const {
  // @@protoc_insertion_point(field_get:SnapshotRsp.MovableSnapshot.heroId)
  return heroid_;
}
inline void SnapshotRsp_MovableSnapshot::set_heroid(::google::protobuf::uint32 value) {
  
  heroid_ = value;
  // @@protoc_insertion_point(field_set:SnapshotRsp.MovableSnapshot.heroId)
}

// optional fixed32 type = 3;
inline void SnapshotRsp_MovableSnapshot::clear_type() {
  type_ = 0u;
}
inline ::google::protobuf::uint32 SnapshotRsp_MovableSnapshot::type() const {
  // @@protoc_insertion_point(field_get:SnapshotRsp.MovableSnapshot.type)
  return type_;
}
inline void SnapshotRsp_MovableSnapshot::set_type(::google::protobuf::uint32 value) {
  
  type_ = value;
  // @@protoc_insertion_point(field_set:SnapshotRsp.MovableSnapshot.type)
}

// optional bytes nickname = 4;
inline void SnapshotRsp_MovableSnapshot::clear_nickname() {
  nickname_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& SnapshotRsp_MovableSnapshot::nickname() const {
  // @@protoc_insertion_point(field_get:SnapshotRsp.MovableSnapshot.nickname)
  return nickname_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void SnapshotRsp_MovableSnapshot::set_nickname(const ::std::string& value) {
  
  nickname_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:SnapshotRsp.MovableSnapshot.nickname)
}
inline void SnapshotRsp_MovableSnapshot::set_nickname(const char* value) {
  
  nickname_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:SnapshotRsp.MovableSnapshot.nickname)
}
inline void SnapshotRsp_MovableSnapshot::set_nickname(const void* value, size_t size) {
  
  nickname_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:SnapshotRsp.MovableSnapshot.nickname)
}
inline ::std::string* SnapshotRsp_MovableSnapshot::mutable_nickname() {
  
  // @@protoc_insertion_point(field_mutable:SnapshotRsp.MovableSnapshot.nickname)
  return nickname_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* SnapshotRsp_MovableSnapshot::release_nickname() {
  // @@protoc_insertion_point(field_release:SnapshotRsp.MovableSnapshot.nickname)
  
  return nickname_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void SnapshotRsp_MovableSnapshot::set_allocated_nickname(::std::string* nickname) {
  if (nickname != NULL) {
    
  } else {
    
  }
  nickname_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), nickname);
  // @@protoc_insertion_point(field_set_allocated:SnapshotRsp.MovableSnapshot.nickname)
}

// optional fixed32 hp = 5;
inline void SnapshotRsp_MovableSnapshot::clear_hp() {
  hp_ = 0u;
}
inline ::google::protobuf::uint32 SnapshotRsp_MovableSnapshot::hp() const {
  // @@protoc_insertion_point(field_get:SnapshotRsp.MovableSnapshot.hp)
  return hp_;
}
inline void SnapshotRsp_MovableSnapshot::set_hp(::google::protobuf::uint32 value) {
  
  hp_ = value;
  // @@protoc_insertion_point(field_set:SnapshotRsp.MovableSnapshot.hp)
}

// optional float speed = 6;
inline void SnapshotRsp_MovableSnapshot::clear_speed() {
  speed_ = 0;
}
inline float SnapshotRsp_MovableSnapshot::speed() const {
  // @@protoc_insertion_point(field_get:SnapshotRsp.MovableSnapshot.speed)
  return speed_;
}
inline void SnapshotRsp_MovableSnapshot::set_speed(float value) {
  
  speed_ = value;
  // @@protoc_insertion_point(field_set:SnapshotRsp.MovableSnapshot.speed)
}

// optional float effDistance = 7;
inline void SnapshotRsp_MovableSnapshot::clear_effdistance() {
  effdistance_ = 0;
}
inline float SnapshotRsp_MovableSnapshot::effdistance() const {
  // @@protoc_insertion_point(field_get:SnapshotRsp.MovableSnapshot.effDistance)
  return effdistance_;
}
inline void SnapshotRsp_MovableSnapshot::set_effdistance(float value) {
  
  effdistance_ = value;
  // @@protoc_insertion_point(field_set:SnapshotRsp.MovableSnapshot.effDistance)
}

// optional bool alive = 8;
inline void SnapshotRsp_MovableSnapshot::clear_alive() {
  alive_ = false;
}
inline bool SnapshotRsp_MovableSnapshot::alive() const {
  // @@protoc_insertion_point(field_get:SnapshotRsp.MovableSnapshot.alive)
  return alive_;
}
inline void SnapshotRsp_MovableSnapshot::set_alive(bool value) {
  
  alive_ = value;
  // @@protoc_insertion_point(field_set:SnapshotRsp.MovableSnapshot.alive)
}

// optional float x = 9;
inline void SnapshotRsp_MovableSnapshot::clear_x() {
  x_ = 0;
}
inline float SnapshotRsp_MovableSnapshot::x() const {
  // @@protoc_insertion_point(field_get:SnapshotRsp.MovableSnapshot.x)
  return x_;
}
inline void SnapshotRsp_MovableSnapshot::set_x(float value) {
  
  x_ = value;
  // @@protoc_insertion_point(field_set:SnapshotRsp.MovableSnapshot.x)
}

// optional float y = 10;
inline void SnapshotRsp_MovableSnapshot::clear_y() {
  y_ = 0;
}
inline float SnapshotRsp_MovableSnapshot::y() const {
  // @@protoc_insertion_point(field_get:SnapshotRsp.MovableSnapshot.y)
  return y_;
}
inline void SnapshotRsp_MovableSnapshot::set_y(float value) {
  
  y_ = value;
  // @@protoc_insertion_point(field_set:SnapshotRsp.MovableSnapshot.y)
}

// optional float vx = 11;
inline void SnapshotRsp_MovableSnapshot::clear_vx() {
  vx_ = 0;
}
inline float SnapshotRsp_MovableSnapshot::vx() const {
  // @@protoc_insertion_point(field_get:SnapshotRsp.MovableSnapshot.vx)
  return vx_;
}
inline void SnapshotRsp_MovableSnapshot::set_vx(float value) {
  
  vx_ = value;
  // @@protoc_insertion_point(field_set:SnapshotRsp.MovableSnapshot.vx)
}

// optional float vy = 12;
inline void SnapshotRsp_MovableSnapshot::clear_vy() {
  vy_ = 0;
}
inline float SnapshotRsp_MovableSnapshot::vy() const {
  // @@protoc_insertion_point(field_get:SnapshotRsp.MovableSnapshot.vy)
  return vy_;
}
inline void SnapshotRsp_MovableSnapshot::set_vy(float value) {
  
  vy_ = value;
  // @@protoc_insertion_point(field_set:SnapshotRsp.MovableSnapshot.vy)
}

// optional fixed64 timestamp = 13;
inline void SnapshotRsp_MovableSnapshot::clear_timestamp() {
  timestamp_ = GOOGLE_ULONGLONG(0);
}
inline ::google::protobuf::uint64 SnapshotRsp_MovableSnapshot::timestamp() const {
  // @@protoc_insertion_point(field_get:SnapshotRsp.MovableSnapshot.timestamp)
  return timestamp_;
}
inline void SnapshotRsp_MovableSnapshot::set_timestamp(::google::protobuf::uint64 value) {
  
  timestamp_ = value;
  // @@protoc_insertion_point(field_set:SnapshotRsp.MovableSnapshot.timestamp)
}

// -------------------------------------------------------------------

// SnapshotRsp

// optional string taskId = 1;
inline void SnapshotRsp::clear_taskid() {
  taskid_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& SnapshotRsp::taskid() const {
  // @@protoc_insertion_point(field_get:SnapshotRsp.taskId)
  return taskid_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void SnapshotRsp::set_taskid(const ::std::string& value) {
  
  taskid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:SnapshotRsp.taskId)
}
inline void SnapshotRsp::set_taskid(const char* value) {
  
  taskid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:SnapshotRsp.taskId)
}
inline void SnapshotRsp::set_taskid(const char* value, size_t size) {
  
  taskid_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:SnapshotRsp.taskId)
}
inline ::std::string* SnapshotRsp::mutable_taskid() {
  
  // @@protoc_insertion_point(field_mutable:SnapshotRsp.taskId)
  return taskid_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* SnapshotRsp::release_taskid() {
  // @@protoc_insertion_point(field_release:SnapshotRsp.taskId)
  
  return taskid_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void SnapshotRsp::set_allocated_taskid(::std::string* taskid) {
  if (taskid != NULL) {
    
  } else {
    
  }
  taskid_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), taskid);
  // @@protoc_insertion_point(field_set_allocated:SnapshotRsp.taskId)
}

// optional string version = 2;
inline void SnapshotRsp::clear_version() {
  version_.ClearToEmptyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline const ::std::string& SnapshotRsp::version() const {
  // @@protoc_insertion_point(field_get:SnapshotRsp.version)
  return version_.GetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void SnapshotRsp::set_version(const ::std::string& value) {
  
  version_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), value);
  // @@protoc_insertion_point(field_set:SnapshotRsp.version)
}
inline void SnapshotRsp::set_version(const char* value) {
  
  version_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), ::std::string(value));
  // @@protoc_insertion_point(field_set_char:SnapshotRsp.version)
}
inline void SnapshotRsp::set_version(const char* value, size_t size) {
  
  version_.SetNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(),
      ::std::string(reinterpret_cast<const char*>(value), size));
  // @@protoc_insertion_point(field_set_pointer:SnapshotRsp.version)
}
inline ::std::string* SnapshotRsp::mutable_version() {
  
  // @@protoc_insertion_point(field_mutable:SnapshotRsp.version)
  return version_.MutableNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline ::std::string* SnapshotRsp::release_version() {
  // @@protoc_insertion_point(field_release:SnapshotRsp.version)
  
  return version_.ReleaseNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}
inline void SnapshotRsp::set_allocated_version(::std::string* version) {
  if (version != NULL) {
    
  } else {
    
  }
  version_.SetAllocatedNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), version);
  // @@protoc_insertion_point(field_set_allocated:SnapshotRsp.version)
}

// optional int32 code = 3;
inline void SnapshotRsp::clear_code() {
  code_ = 0;
}
inline ::google::protobuf::int32 SnapshotRsp::code() const {
  // @@protoc_insertion_point(field_get:SnapshotRsp.code)
  return code_;
}
inline void SnapshotRsp::set_code(::google::protobuf::int32 value) {
  
  code_ = value;
  // @@protoc_insertion_point(field_set:SnapshotRsp.code)
}

// repeated .SnapshotRsp.MovableSnapshot snapshots = 4;
inline int SnapshotRsp::snapshots_size() const {
  return snapshots_.size();
}
inline void SnapshotRsp::clear_snapshots() {
  snapshots_.Clear();
}
inline const ::SnapshotRsp_MovableSnapshot& SnapshotRsp::snapshots(int index) const {
  // @@protoc_insertion_point(field_get:SnapshotRsp.snapshots)
  return snapshots_.Get(index);
}
inline ::SnapshotRsp_MovableSnapshot* SnapshotRsp::mutable_snapshots(int index) {
  // @@protoc_insertion_point(field_mutable:SnapshotRsp.snapshots)
  return snapshots_.Mutable(index);
}
inline ::SnapshotRsp_MovableSnapshot* SnapshotRsp::add_snapshots() {
  // @@protoc_insertion_point(field_add:SnapshotRsp.snapshots)
  return snapshots_.Add();
}
inline ::google::protobuf::RepeatedPtrField< ::SnapshotRsp_MovableSnapshot >*
SnapshotRsp::mutable_snapshots() {
  // @@protoc_insertion_point(field_mutable_list:SnapshotRsp.snapshots)
  return &snapshots_;
}
inline const ::google::protobuf::RepeatedPtrField< ::SnapshotRsp_MovableSnapshot >&
SnapshotRsp::snapshots() const {
  // @@protoc_insertion_point(field_list:SnapshotRsp.snapshots)
  return snapshots_;
}

#endif  // !PROTOBUF_INLINE_NOT_IN_HEADERS
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_SnapshotRsp_2eproto__INCLUDED
