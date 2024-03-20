QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    action.pb.cc \
    cycleclock.cc \
    client.cpp \
    createsession.cpp \
    element.cpp \
    element.pb.cc \
    graphcycles.cc \
    joinsession.cpp \
    kernel_timeout.cc \
    low_level_alloc.cc \
    main.cpp \
    mainwidget.cpp \
    packet.cpp \
    packet.pb.cc \
    packet_client.cpp \
    receiveoption.cpp \
    setpacket.cpp \
    thread_identity.cc \
    utils.cpp \
    whiteboard_new.cpp

HEADERS += \
    action.pb.h \
    client.hpp \
    createsession.h \
    cycleclock.h \
    cycleclock_config.h \
    element.hpp \
    element.pb.h \
    exception.hpp \
    graphcycles.h \
    joinsession.h \
    kernel_timeout.h \
    low_level_alloc.h \
    mainwidget.h \
    packet.hpp \
    packet.pb.h \
    receiveoption.h \
    setpacket.h \
    thread_identity.h \
    utils.hpp \
    whiteboard_new.h

FORMS += \
    createsession.ui \
    joinsession.ui \
    mainwidget.ui \
    serverwait.ui

INCLUDEPATH += D:/protobuf-25.1/src
LIBS += -LD:/output-project -lprotobuf

INCLUDEPATH += D:/abseil-cpp-20230802.1
INCLUDEPATH += D:/abseil-cpp-20230802.1/absl
INCLUDEPATH += D:/abseil-cpp-20230802.1/absl/base/internal
INCLUDEPATH += D:/abseil-cpp-20230802.1/absl/strings/internal


LIBS += -LD:/abseil-cpp-20230802.1/output/absl/log \
-LD:/abseil-cpp-20230802.1/output/absl/strings \
-LD:/abseil-cpp-20230802.1/output/absl/strings/internal \
-LD:/abseil-cpp-20230802.1/output/absl/base \
-LD:/abseil-cpp-20230802.1/output/absl/container \
-LD:/abseil-cpp-20230802.1/output/absl/crc \
-LD:/abseil-cpp-20230802.1/output/absl/debugging \
-LD:/abseil-cpp-20230802.1/output/absl/flags \
-LD:/abseil-cpp-20230802.1/output/absl/hash \
-LD:/abseil-cpp-20230802.1/output/absl/numeric \
-LD:/abseil-cpp-20230802.1/output/absl/profiling \
-LD:/abseil-cpp-20230802.1/output/absl/random \
-LD:/abseil-cpp-20230802.1/output/absl/status \
-LD:/abseil-cpp-20230802.1/output/absl/synchronization \
-LD:/abseil-cpp-20230802.1/output/absl/time \
-LD:/abseil-cpp-20230802.1/output/absl/types \
-LD:/output-project/third_party/utf8_range   \
-LD:/abseil-cpp-20230802.1/output/lib/pkgconfig


LIBS += -labsl_base                \
  -labsl_str_format_internal       \
  -labsl_strings                   \
  -labsl_strings_internal          \
  -labsl_string_view               \
  -labsl_str_format_internal       \
  -labsl_die_if_null               \
  -labsl_log_initialize            \
  -labsl_log_entry                 \
  -labsl_log_flags                 \
  -labsl_flags_commandlineflag     \
  -labsl_flags_commandlineflag_internal \
  -labsl_flags_config              \
  -labsl_flags_internal            \
  -labsl_flags_marshalling         \
  -labsl_flags_parse               \
  -labsl_flags_private_handle_accessor \
  -labsl_flags_program_name        \
  -labsl_flags_reflection          \
  -labsl_flags_usage               \
  -labsl_flags_usage_internal      \
  -labsl_failure_signal_handler    \
  -labsl_log_severity              \
  -labsl_log_internal_check_op     \
  -labsl_log_internal_conditions   \
  -labsl_log_internal_message      \
  -labsl_log_internal_nullguard    \
  -labsl_log_internal_proto        \
  -labsl_log_internal_format       \
  -labsl_log_internal_globals      \
  -labsl_log_internal_log_sink_set \
  -labsl_log_sink                  \
#  -labsl_scoped_mock_log           \
  -labsl_leak_check                \
  -labsl_raw_logging_internal      \
  -labsl_log_globals               \
  -lutf8_validity                  \
  -labsl_cord                      \
  -labsl_cordz_info                \
  -labsl_cordz_handle              \
  -labsl_cordz_functions           \
  -labsl_cord_internal             \
  -labsl_cordz_sample_token        \
  -labsl_crc_cord_state            \
  -labsl_crc_cpu_detect            \
  -labsl_crc32c                    \
  -labsl_crc_internal              \
  -labsl_exponential_biased        \
  -labsl_hashtablez_sampler        \
  -labsl_synchronization           \
  -labsl_time                      \
  -labsl_time_zone                 \
  -labsl_int128                    \
  -labsl_examine_stack             \
  -labsl_scoped_set_env            \
  -labsl_stacktrace                \
  -labsl_status                    \
  -labsl_statusor                  \
  -labsl_symbolize                 \
  -labsl_demangle_internal         \
  -labsl_debugging_internal        \
  -labsl_malloc_internal           \
  -labsl_throw_delegate            \
  -labsl_strerror                  \
  -labsl_raw_hash_set              \
  -labsl_hash                      \
  -labsl_city                      \
  -labsl_civil_time                \
  -labsl_low_level_hash            \
  -labsl_spinlock_wait             \
  -labsl_periodic_sampler          \
  -labsl_graphcycles_internal      \
  -labsl_kernel_timeout_internal   \
  -labsl_synchronization           \
  -labsl_bad_any_cast_impl         \
  -labsl_bad_optional_access       \
  -labsl_bad_variant_access        \
  -labsl_random_distributions      \
  -labsl_random_internal_distribution_test_util \
  -labsl_random_internal_platform  \
  -labsl_random_internal_pool_urbg \
  -labsl_random_internal_randen    \
  -labsl_random_internal_randen_hwaes \
  -labsl_random_internal_randen_hwaes_impl \
  -labsl_random_internal_randen_slow \
  -labsl_random_internal_seed_material \
  -labsl_random_seed_gen_exception  \
  -labsl_random_seed_sequences      \

LIBS += -LD:/abseil-cpp-20230802.1/output/absl/base -ldbghelp

#INCLUDEPATH += D:/protobuf-25.1/third_party/abseil-cpp-20230802.1/absl/base/internal
#CONFIG += link_pkgconfig
#PKGCONFIG += absl_base_internal

LIBS += -lws2_32
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images.qrc

DISTFILES += \
    action.proto \
    element.proto \
    packet.proto

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../protobuf-25.1/third_party/abseil-cpp-20230802.1/output/absl/base/ -labsl_base
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../protobuf-25.1/third_party/abseil-cpp-20230802.1/output/absl/base/ -labsl_base
else:unix: LIBS += -L$$PWD/../../protobuf-25.1/third_party/abseil-cpp-20230802.1/output/absl/base/ -labsl_base

INCLUDEPATH += $$PWD/../../protobuf-25.1/third_party/abseil-cpp-20230802.1/output/absl/base
DEPENDPATH += $$PWD/../../protobuf-25.1/third_party/abseil-cpp-20230802.1/output/absl/base

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../protobuf-25.1/third_party/abseil-cpp-20230802.1/output/absl/synchronization/ -labsl_synchronization
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../protobuf-25.1/third_party/abseil-cpp-20230802.1/output/absl/synchronization/ -labsl_synchronization
else:unix: LIBS += -L$$PWD/../../protobuf-25.1/third_party/abseil-cpp-20230802.1/output/absl/synchronization/ -labsl_synchronization

INCLUDEPATH += $$PWD/../../protobuf-25.1/third_party/abseil-cpp-20230802.1/output/absl/synchronization
DEPENDPATH += $$PWD/../../protobuf-25.1/third_party/abseil-cpp-20230802.1/output/absl/synchronization
